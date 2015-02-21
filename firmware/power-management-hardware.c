/* STM32F1 Power Management for Solar Power

Hardware Setup

This configures all peripherals for the program and provides some
convenience functions for accessing the hardware.

Along with the file board-defs.h that provides specific defines for the
processor board in use, this file abstracts the hardware from the rest
of the program and allows rapid adaption to other board archictectures.

Note that the STM32F series of ARM microcontrollers are used because of
their provision of 12 bit A/D.

Flash write code adapted from code by Damian Miller.

Initial 29 September 2013
*/

/*
 * This file is part of the battery-management-system project.
 *
 * Copyright 2013 K. Sarkies <ksarkies@internode.on.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Set this define if the SWD debug is to be used. It will disable some ports
so must be commented out for normal use. */
//#define USE_SWD

#include <stdint.h>
#include <stdbool.h>

#include "power-management-board-defs.h"
#include "power-management-hardware.h"

/* libopencm3 driver includes */
#include <libopencm3/stm32/iwdg.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/rtc.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/systick.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

extern uint32_t __configBlockStart;
extern uint32_t __configBlockEnd;

/* Local Prototypes */
static void adcSetup(void);
static void dmaAdcSetup(void);
static void iwdgSetup(void);
static void gpioSetup(void);
static void usartSetup(void);
static void clockSetup(void);
static void systickSetup();
static void pwmSetup(void);

/* Local Variables */
static uint8_t pwmCount;
static uint32_t v[NUM_CHANNEL]; /* Buffer used by DMA to dump A/D conversions */
static uint8_t adceoc;          /* A/D end of conversion flag */
static uint32_t lostCharacters; /* Number of characters lost due to queue full */

/* FreeRTOS queues and intercommunication variables defined in Comms */
extern xQueueHandle commsSendQueue, commsReceiveQueue, commsEmptySemaphore;

/* Time variable needed when systick is the timer */
static uint32_t timeCounter;

/* This is provided in the FAT filesystem library */
extern void disk_timerproc();

/*--------------------------------------------------------------------------*/
/** @brief Initialise the hardware

*/

void prvSetupHardware(void)
{
    clockSetup();
    gpioSetup();
    usartSetup();
    pwmSetup();
    dmaAdcSetup();
    adcSetup();
    systickSetup();
    rtc_auto_awake(LSE, 0x7fff);
    iwdgSetup();
}

/*--------------------------------------------------------------------------*/
/** @brief Return the A/D Conversion Results

Note the channel must be less than the number of channels in the A/D converter.

@param[in] uint8_t channel: A/D channel to be retrieved from the DMA buffer.
@returns uint32_t : last value measured by the A/D converter.
*/

uint32_t adcValue(uint8_t channel)
{
    if (channel > NUM_CHANNEL) return 0;
    return v[channel];
}

/*--------------------------------------------------------------------------*/
/** @brief Return and Reset the A/D End of Conversion Flag

This must be retrieved from the ISR as the hardware EOC doesn't always change
at the end of a conversion (when multiple conversions take place).

@returns uint8_t: boolean true if the flag was set; false otherwise.
*/

uint8_t adcEOC(void)
{
    if (adceoc > 0)
    {
        adceoc = 0;
        return 1;
    }
    return 0;
}

/*--------------------------------------------------------------------------*/
/** @brief Read and Return Interface Error Indicators

The indicator settings are read and combined, with battery1, battery2,
battery3, load1, load2, panel from lsb up, and overload followed by
undervoltage for each.

Each bit is zero if the indicator is on, 1 if it is off.

@returns uint16_t
*/

uint16_t getIndicators(void)
{
    uint16_t indicators = 0;
    indicators |= ((gpio_port_read(BATTERY1_STATUS_PORT) >> 
                                    BATTERY1_STATUS_SHIFT) & 0x03) << 0;
    indicators |= ((gpio_port_read(BATTERY2_STATUS_PORT) >> 
                                    BATTERY2_STATUS_SHIFT) & 0x03) << 2;
    indicators |= ((gpio_port_read(BATTERY3_STATUS_PORT) >> 
                                    BATTERY3_STATUS_SHIFT) & 0x03) << 4;
    indicators |= ((gpio_port_read(LOAD1_STATUS_PORT) >> 
                                    LOAD1_STATUS_SHIFT) & 0x03) << 6;
    indicators |= ((gpio_port_read(LOAD2_STATUS_PORT) >> 
                                    LOAD2_STATUS_SHIFT) & 0x03) << 8;
    indicators |= ((gpio_port_read(PANEL_STATUS_PORT) >> 
                                    PANEL_STATUS_SHIFT) & 0x03) << 10;
    return indicators;
}

/*--------------------------------------------------------------------------*/
/** @brief Make Switch Settings

The switch is set through the GPIO interface. The load or panel specified is
set either to the battery specified, or is disconnected from all batteries.

This function provides a common interface if different hardware is used.

@parameter[in] uint8_t battery: (1-3, 0 = none)
@parameter[in] uint8_t setting: load (0-1), panel 2.
*/

void setSwitch(uint8_t battery, uint8_t setting)
{
    uint16_t switchControl = gpio_port_read(SWITCH_CONTROL_PORT);
    uint16_t switchControlBits = ((switchControl >> SWITCH_CONTROL_SHIFT) & 0x3F);
/* Each two-bit field represents load 1 bits 0-1, load 2 bits 2-3
panel bits 4-5, and the setting is the battery to be connected (no two batteries
can be connected to a load/panel at the same time). The final bit pattern of
settings go into the switch control port, preserving the lower bits. */
    if ((battery <= 3) && (setting <= 2))
    {
        switchControlBits &= (~(0x03 << (setting<<1)));
        switchControlBits |= ((battery & 0x03) << (setting<<1));
        switchControl &= ~(0x3F << SWITCH_CONTROL_SHIFT);
        gpio_port_write(SWITCH_CONTROL_PORT,
                    (switchControl | (switchControlBits << SWITCH_CONTROL_SHIFT)));
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Return the Switch Settings

Each two-bit field represents load 1 bits 0-1, load 2 bits 2-3 panel bits 4-5,
and the setting is the battery (1-3) to be connected. Battery 0 = none connected.

@returns uint16_t: the switch settings from the relevant port.
*/

uint8_t getSwitchControlBits(void)
{
    return ((gpio_port_read(SWITCH_CONTROL_PORT) >> SWITCH_CONTROL_SHIFT) & 0x3F);
}

/*--------------------------------------------------------------------------*/
/** @brief Set the Interface Reset Line

@param[in] uint8_t interface: interface 0-5, being batteries 1-3, loads 1-2, module.
*/

void overCurrentReset(uint8_t interface)
{
    uint32_t port;
    uint16_t bit;
    switch (interface)
    {
        case 0:
            port = BATTERY1_OVERCURRENT_RESET_PORT;
            bit = BATTERY1_OVERCURRENT_RESET_BIT;
            break;
        case 1:
            port = BATTERY2_OVERCURRENT_RESET_PORT;
            bit = BATTERY2_OVERCURRENT_RESET_BIT;
            break;
        case 2:
            port = BATTERY3_OVERCURRENT_RESET_PORT;
            bit = BATTERY3_OVERCURRENT_RESET_BIT;
            break;
        case 3:
            port = LOAD1_OVERCURRENT_RESET_PORT;
            bit = LOAD1_OVERCURRENT_RESET_BIT;
            break;
        case 4:
            port = LOAD2_OVERCURRENT_RESET_PORT;
            bit = LOAD2_OVERCURRENT_RESET_BIT;
            break;
        case 5:
            port = PANEL_OVERCURRENT_RESET_PORT;
            bit = PANEL_OVERCURRENT_RESET_BIT;
            break;
    }
    if (interface < 6) gpio_set(port,bit);
}

/*--------------------------------------------------------------------------*/
/** @brief Release the Interface Reset Line

@param[in] uint8_t interface: interface 0-5, being batteries 1-3, loads 1-2, module.
*/

void overCurrentRelease(uint8_t interface)
{
    uint32_t port;
    uint16_t bit;
    switch (interface)
    {
        case 0:
            port = BATTERY1_OVERCURRENT_RESET_PORT;
            bit = BATTERY1_OVERCURRENT_RESET_BIT;
            break;
        case 1:
            port = BATTERY2_OVERCURRENT_RESET_PORT;
            bit = BATTERY2_OVERCURRENT_RESET_BIT;
            break;
        case 2:
            port = BATTERY3_OVERCURRENT_RESET_PORT;
            bit = BATTERY3_OVERCURRENT_RESET_BIT;
            break;
        case 3:
            port = LOAD1_OVERCURRENT_RESET_PORT;
            bit = LOAD1_OVERCURRENT_RESET_BIT;
            break;
        case 4:
            port = LOAD2_OVERCURRENT_RESET_PORT;
            bit = LOAD2_OVERCURRENT_RESET_BIT;
            break;
        case 5:
            port = PANEL_OVERCURRENT_RESET_PORT;
            bit = PANEL_OVERCURRENT_RESET_BIT;
            break;
    }
    if (interface < 6) gpio_clear(port,bit);
}

/*--------------------------------------------------------------------------*/
/** @brief Restore Saved Switch Settings

Each two-bit field represents load1 bits 0-1, load2 bits 2-3 panel bits 4-5,
and the setting is the battery (1-3) to be connected. Battery 0 = none connected.

This can be used as a raw switch setting call but is not recommended for normal
use.

@param[in] uint16_t settings: the switch settings from the relevant port.
*/

void setSwitchControlBits(uint8_t settings)
{
    uint16_t switchControl = gpio_port_read(SWITCH_CONTROL_PORT);
    switchControl &= ~(0x3F << SWITCH_CONTROL_SHIFT);
    gpio_port_write(SWITCH_CONTROL_PORT,
                (switchControl | (settings << SWITCH_CONTROL_SHIFT)));
}

/*--------------------------------------------------------------------------*/
/** @brief PWM Timer set Duty Cycle

@parameter[in] uint8_t dutyCycle: Duty cycle in percentage.
*/

void pwmSetDutyCycle(uint16_t dutyCycle)
{
    uint32_t threshold = ((PWM_PERIOD*dutyCycle)/100)>>8;
    timer_set_oc_value(TIM1, TIM_OC1, threshold);
    timer_generate_event(TIM1, TIM_EGR_UG);
}

/*--------------------------------------------------------------------------*/
/** @brief Enable/Disable USART Interrupt

@parameter[in] uint8_t enable: true to enable the interrupt, false to disable.
*/

void commsEnableTxInterrupt(uint8_t enable)
{
    if (enable) usart_enable_tx_interrupt(USART1);
    else usart_disable_tx_interrupt(USART1);
}

/*--------------------------------------------------------------------------*/
/** @brief Clock Setup

The processor system clock is established and the necessary peripheral
clocks are turned on.
*/

static void clockSetup(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();
}

/*--------------------------------------------------------------------------*/
/** @brief USART Setup

USART 1 is configured for 115200 baud, no flow control, and interrupt.
*/

static void usartSetup(void)
{
/* Enable clocks for GPIO port A (for GPIO_USART1_TX) and USART1. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_USART1);
/* Enable the USART1 interrupt. */
    nvic_enable_irq(NVIC_USART1_IRQ);
/* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port A for transmit. */
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
              GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);
/* Setup GPIO pin GPIO_USART1_RE_RX on GPIO port A for receive. */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
              GPIO_CNF_INPUT_FLOAT, GPIO_USART1_RX);
/* Setup UART parameters. */
    usart_set_baudrate(USART1, BAUDRATE);
    usart_set_databits(USART1, 8);
    usart_set_stopbits(USART1, USART_STOPBITS_1);
    usart_set_parity(USART1, USART_PARITY_NONE);
    usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
    usart_set_mode(USART1, USART_MODE_TX_RX);
/* Enable USART1 receive interrupts. */
    usart_enable_rx_interrupt(USART1);
    usart_disable_tx_interrupt(USART1);
/* Finally enable the USART. */
    usart_enable(USART1);
}

/*--------------------------------------------------------------------------*/
/** @brief PWM Timer Setup

Setup GPIO Port A8 for TIM1_CH1 PWM output to generate a signal of a given duty
cycle. No interrupt is required.
*/

static void pwmSetup(void)
{
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_TIM1);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
              GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO8);

/* Reset TIM1 peripheral. */
    timer_reset(TIM1);

/* Set Timer global mode:
 - No division
 - Alignment centre (up/down counting), mode 1 (interrupt on downcount only).
 - Direction up (when centre mode is set it is read only, changes by hardware)
*/
    timer_set_mode(TIM1, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_CENTER_1, TIM_CR1_DIR_UP);

/* Set Timer output compare mode:
 - Channel 1, PWM mode 2 (output low when CNT <= CCR1, high otherwise)
*/
    timer_set_oc_mode(TIM1, TIM_OC1, TIM_OCM_PWM1);
    timer_enable_oc_output(TIM1, TIM_OC1);
    timer_enable_break_main_output(TIM1);

/* The ARR (auto-preload register) sets the PWM period to 50 microseconds from
the 72 MHz clock.*/
    timer_enable_preload(TIM1);
    timer_set_period(TIM1, PWM_PERIOD);

/* The CCR1 (capture/compare register) sets the PWM duty cycle to default 50% */
    pwmSetDutyCycle(50);

/* Force an update to load the shadow registers */
    timer_generate_event(TIM1, TIM_EGR_UG);

/* Start the Counter. */
    timer_enable_counter(TIM1);
}

/*--------------------------------------------------------------------------*/
/** @brief Independent Watchdog Timer Reset

Must be called by an active task before the timer period expires.
*/

void iwdgReset(void)
{
    iwdg_reset();
}

/*--------------------------------------------------------------------------*/
/** @brief Independent Watchdog Timer Setup

*/

static void iwdgSetup(void)
{
    iwdg_set_period_ms(IWDG_TIMEOUT_MS);
    iwdg_start();
}

/*--------------------------------------------------------------------------*/
/** @brief GPIO Setup

Setup GPIO Ports A, B, C for all peripherals.
*/

static void gpioSetup(void)
{
/* Enable all GPIO clocks. */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
    rcc_periph_clock_enable(RCC_AFIO);

#ifndef USE_SWD
/* Disable SWD and JTAG to allow full use of the ports PA13, PA14, PA15 */
    gpio_primary_remap(AFIO_MAPR_SWJ_CFG_JTAG_OFF_SW_OFF,0);
#endif

/* PA inputs analogue for currents, voltages and ambient temperature */
#ifdef PA_ANALOGUE_INPUTS
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG,
                PA_ANALOGUE_INPUTS);
#endif
/* PA inputs analogue for currents, voltages and ambient temperature */
#ifdef PA_ANALOGUE_INPUTS
    gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG,
                PC_ANALOGUE_INPUTS);
#endif
/* PA outputs digital */
#ifdef PA_DIGITAL_OUTPUTS
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                PA_DIGITAL_OUTPUTS);
    gpio_clear(GPIOA, PA_DIGITAL_OUTPUTS);
#endif
/* PB outputs digital */
#ifdef PB_DIGITAL_OUTPUTS
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                PB_DIGITAL_OUTPUTS);
    gpio_clear(GPIOB, PB_DIGITAL_OUTPUTS);
#endif
/* PC outputs digital */
#ifdef PC_DIGITAL_OUTPUTS
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL,
                PC_DIGITAL_OUTPUTS);
    gpio_clear(GPIOC, PC_DIGITAL_OUTPUTS);
#endif
/* PA inputs digital. Set pull up/down configuration to pull up. */
#ifdef PA_DIGITAL_INPUTS
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                PA_DIGITAL_INPUTS);
    gpio_set(GPIOA,PA_DIGITAL_INPUTS);
#endif
/* PB inputs digital. Set pull up/down configuration to pull up. */
#ifdef PB_DIGITAL_INPUTS
    gpio_set_mode(GPIOB, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                PB_DIGITAL_INPUTS);
    gpio_set(GPIOB,PB_DIGITAL_INPUTS);
#endif
/* PC inputs digital. Set pull up/down configuration to pull up. */
#ifdef PC_DIGITAL_INPUTS
    gpio_set_mode(GPIOC, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN,
                PC_DIGITAL_INPUTS);
    gpio_set(GPIOC,PC_DIGITAL_INPUTS);
#endif
}

/*--------------------------------------------------------------------------*/
/** @brief DMA Setup

Enable DMA 1 Channel 1 to take conversion data from ADC 1, and also ADC 2 when
the ADC is used in dual mode. The ADC will dump a burst of data to memory each
time, and we need to grab it before the next conversions start. This must be
called after each transfer to reset the memory buffer to the beginning.
*/

static void dmaAdcSetup(void)
{
    /* Enable DMA1 Clock */
    rcc_periph_clock_enable(RCC_DMA1);
    dma_channel_reset(DMA1,DMA_CHANNEL1);
    dma_set_priority(DMA1,DMA_CHANNEL1,DMA_CCR_PL_LOW);
/* We want all 32 bits from the ADC to include ADC2 data */
    dma_set_memory_size(DMA1,DMA_CHANNEL1,DMA_CCR_MSIZE_32BIT);
    dma_set_peripheral_size(DMA1,DMA_CHANNEL1,DMA_CCR_PSIZE_32BIT);
    dma_enable_memory_increment_mode(DMA1,DMA_CHANNEL1);
    dma_set_read_from_peripheral(DMA1,DMA_CHANNEL1);
/* The register to target is the ADC1 regular data register */
    dma_set_peripheral_address(DMA1,DMA_CHANNEL1,(uint32_t) &ADC_DR(ADC1));
/* The array v[] receives the converted output */
    dma_set_memory_address(DMA1,DMA_CHANNEL1,(uint32_t) v);
    dma_set_number_of_data(DMA1,DMA_CHANNEL1,NUM_CHANNEL);
    dma_enable_channel(DMA1,DMA_CHANNEL1);
}

/*--------------------------------------------------------------------------*/
/** @brief ADC Setup

ADC1 is setup for scan mode. Single conversion does all selected
channels once through then stops. DMA enabled to collect data.
*/

static void adcSetup(void)
{
    /* Enable clocks for ADCs */
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_AFIO);
    rcc_periph_clock_enable(RCC_ADC1);
/* ADC clock should be maximum 14MHz, so divide by 8 from 72MHz. */
    rcc_set_adcpre(RCC_CFGR_ADCPRE_PCLK2_DIV8);
    nvic_enable_irq(NVIC_ADC1_2_IRQ);
    /* Make sure the ADC doesn't run during config. */
    adc_off(ADC1);
    /* Configure ADC1 for multiple conversion. */
    adc_enable_scan_mode(ADC1);
    adc_set_single_conversion_mode(ADC1);
    adc_enable_external_trigger_regular(ADC1, ADC_CR2_EXTSEL_SWSTART);
    adc_set_right_aligned(ADC1);
    adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_28DOT5CYC);
    adc_enable_dma(ADC1);
    adc_enable_eoc_interrupt(ADC1);
/* Power on and calibrate */
    adc_power_on(ADC1);
    /* Wait for ADC starting up. */
    uint32_t i;
    for (i = 0; i < 800000; i++)    /* Wait a bit. */
        __asm__("nop");
    adc_reset_calibration(ADC1);
    adc_calibration(ADC1);
}

/*--------------------------------------------------------------------------*/
/** @brief Systick Setup

Setup SysTick Timer for 1 millisecond interrupts, also enables Systick and
Systick-Interrupt
*/

static void systickSetup()
{
    /* 72MHz / 8 => 9,000,000 counts per second */
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);

    /* 9000000/9000 = 1000 overflows per second - every 1ms one interrupt */
    /* SysTick interrupt every N clock pulses: set reload to N-1 */
    systick_set_reload(8999);

    systick_interrupt_enable();

    /* Start counting. */
    systick_counter_enable();
}
/*--------------------------------------------------------------------------*/
/** @brief Read a data block from Flash memory

Adapted from code by Damian Miller.

@param[in] uint32_t *flashBlock: address of Flash page start
@param[in] uint32_t *dataBlock: pointer to data block to write
@param[in] uint16_t size: length of data block
*/

void flashReadData(uint32_t *flashBlock, uint8_t *dataBlock, uint16_t size)
{
    uint16_t n;
    uint32_t *flashAddress= flashBlock;

    for(n=0; n<size; n += 4)
    {
        *(uint32_t*)dataBlock = *(flashAddress++);
        dataBlock += 4;
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Program a data block to Flash memory

Adapted from code by Damian Miller.

@param[in] uint32_t *flashBlock: address of Flash page start
@param[in] uint32_t *dataBlock: pointer to data block to write
@param[in] uint16_t size: length of data block
@returns uint32_t result code: 0 success, bit 0 address out of range,
bit 2: programming error, bit 4: write protect error, bit 7 compare fail.
*/

uint32_t flashWriteData(uint32_t *flashBlock, uint8_t *data, uint16_t size)
{
    uint16_t n;

    uint32_t pageStart = (uint32_t)flashBlock;
    uint32_t flashAddress = pageStart;
    uint32_t pageAddress = pageStart;
    uint32_t flashStatus = 0;

    /*check if pageStart is in proper range*/
    if((pageStart < __configBlockStart) || (pageStart >= __configBlockEnd))
        return 1;

    /*calculate current page address*/
    if(pageStart % FLASH_PAGE_SIZE)
        pageAddress -= (pageStart % FLASH_PAGE_SIZE);

    flash_unlock();

    /*Erasing page*/
    flash_erase_page(pageAddress);
    flashStatus = flash_get_status_flags();
    if(flashStatus != FLASH_SR_EOP)
        return flashStatus;

    /*programming flash memory*/
    for(n=0; n<size; n += 4)
    {
        /*programming word data*/
        flash_program_word(flashAddress+n, *((uint32_t*)(data + n)));
        flashStatus = flash_get_status_flags();
        if(flashStatus != FLASH_SR_EOP)
            return flashStatus;

        /*verify if correct data is programmed*/
        if(*((uint32_t*)(flashAddress+n)) != *((uint32_t*)(data + n)))
            return 0x80;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
/** @brief Read the Time

@returns uint32_t. Time value.
*/

uint32_t getTimeCounter()
{
#if (RTC_SOURCE == RTC)
    return rtc_get_counter_val();
#else
    return timeCounter;
#endif
}

/*--------------------------------------------------------------------------*/
/** @brief Set the Time

@param[in] time uint32_t. Time value to set.
*/

void setTimeCounter(uint32_t time)
{
#if (RTC_SOURCE == RTC)
    rtc_set_counter_val(time);
#else
    timeCounter = time;;
#endif
}

/*--------------------------------------------------------------------------*/
/** @brief Update the time counter value

This is called from the systick ISR every second if it is used as a RTC.
*/

void updateTimeCount(void)
{
    timeCounter++;
}
/*--------------------------------------------------------------------------*/
                /* ISRs */
/*--------------------------------------------------------------------------*/
/** @brief USART Interrupt

Find out what interrupted and get or send data as appropriate.
*/

void usart1_isr(void)
{

/* Check if we were called because of RXNE. */
    if (usart_get_flag(USART1,USART_SR_RXNE))
    {
/* Pull in received character. If buffer full we'll just drop it */
        char inCharacter = (char) usart_recv(USART1);
        if (xQueueSendToBackFromISR(commsReceiveQueue,&inCharacter,NULL) == errQUEUE_FULL)
            lostCharacters++;
    }
/* Check if we were called because of TXE. */
    if (usart_get_flag(USART1,USART_SR_TXE))
    {
/* If the queue is empty, disable the tx interrupt until something is sent. */
        char data;
        if (xQueueReceiveFromISR(commsSendQueue,&data,NULL))
            usart_send(USART1, data);
        else
        {
            int wokenTask;
            usart_disable_tx_interrupt(USART1);
            xSemaphoreGiveFromISR(commsEmptySemaphore,&wokenTask);    /* Flag as empty */
        }
    }
}

/*--------------------------------------------------------------------------*/
/** @brief ADC ISR

Respond to ADC EOC at end of scan.

The EOC status is lost when DMA reads the data register, so use a global
variable.
*/

void adc1_2_isr(void)
{
    adceoc = 1;
/* Clear DMA to restart at beginning of data array */
    dmaAdcSetup();
}

/*-----------------------------------------------------------*/
/*----       ISR Overrides in libopencm3     ----------------*/
/*-----------------------------------------------------------*/
/* These trap serious execution errors and reset the application by means
of the CM3 system reset command. */

/*-----------------------------------------------------------*/
void nmi_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void hard_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void memory_manage_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void bus_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void usage_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
/*----    FreeRTOS ISR Overrides in libopencm3     ----------*/
/*-----------------------------------------------------------*/

extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);
extern void vPortSVCHandler( void );

/*-----------------------------------------------------------*/
void sv_call_handler(void)
{
    vPortSVCHandler();
}

/*-----------------------------------------------------------*/

void pend_sv_handler(void)
{
    xPortPendSVHandler();
}

/*-----------------------------------------------------------*/
/** @Brief Systick Interrupt Handler

This provides a link to the FreeRTOS systick interrupt handler.

Systick is setup in FreeRTOS according to the tick rate specified in
FreeRTOSConfig.h. That rate is set at 100Hz.

This also updates the status of any inserted SD card every 10 ms.

Can be used to provide a RTC.
*/
void sys_tick_handler(void)
{

    static uint16_t timer=0;
    timer++;
    if (timer >= 10)
    {
        timer=0;
        disk_timerproc();
    }

/* updated every 1s if systick is used for real-time clock. */
    static uint16_t cnttime=0;
    cnttime++;
    if (cnttime >= 100)
    {
        cnttime = 0;
        updateTimeCount();
    }
    xPortSysTickHandler();
}

