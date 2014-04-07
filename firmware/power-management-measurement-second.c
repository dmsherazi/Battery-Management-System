/* STM32F1 Power Management for Solar Power

Defines the Measurement Task. This produces the basic measurements and avoids
all refined processing apart from scaling of raw measurements and computation
of battery parameters that need to be done in real time.

All measured quantities are stored locally for other tasks to collect.

The task performs all current and voltage measurements on the interfaces
and computes some battery parameters:
- battery resistance from step changes in current/voltage.
- accumulated charge passing to/from the batteries.

Each interface has a data structure holding the currents and voltages.

Initial 29 September 2013
Refactor 4 January 2014
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

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* libopencm3 driver includes */
#include <libopencm3/stm32/adc.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "power-management-board-defs.h"
#include "power-management.h"
#include "power-management-comms.h"
#include "power-management-hardware.h"
#include "power-management-objdic.h"
#include "power-management-monitor.h"
#include "power-management-measurement-second.h"

/* Local Prototypes */
static void initGlobals(void);

/* Local Persistent Variables */
/* All variables times 256 except resistances times 65536. */
static uint8_t measurementWatchdogCount;
static int16_t currentStepAv[NUM_BATS];  /* Estimated current average */
static int16_t voltageStepAv[NUM_BATS];  /* Estimated voltage average */
static int16_t lastBatteryCurrent[NUM_BATS];
static int16_t lastBatteryVoltage[NUM_BATS];
static int16_t batteryResistanceAv[NUM_BATS];
static int32_t accumulatedBatteryCharge[NUM_BATS];
static int16_t temperature;
static union InterfaceGroup currents;
static union InterfaceGroup voltages;

/*--------------------------------------------------------------------------*/
/* @brief Measurement Task

This measures the interface currents and voltages and passes them to other
tasks for processing and storage.

Certain other actions are needed at the fastest rate:
- Averaging of measurements;
- Detection of sudden current changes to find ohmic resistance;
- Estimation of battery parameters;
- Implementation of Coulomb Counting for SoC.

A/D measurements are performed in a single conversion and stored via DMA.
*/

void prvMeasurementTask(void *pvParameters)
{
    uint8_t i,j;
    int32_t av[N_CONV];
	uint8_t channel_array[N_CONV];
    initGlobals();

/* Setup the array of selected channels for conversion */
	channel_array[0] = ADC_CHANNEL_BATTERY1_CURRENT;    /* Battery 1 */
	channel_array[1] = ADC_CHANNEL_BATTERY1_VOLTAGE;
	channel_array[2] = ADC_CHANNEL_BATTERY2_CURRENT;    /* Battery 2 */
	channel_array[3] = ADC_CHANNEL_BATTERY2_VOLTAGE;
	channel_array[4] = ADC_CHANNEL_BATTERY3_CURRENT;    /* Battery 3 */
	channel_array[5] = ADC_CHANNEL_BATTERY3_VOLTAGE;
	channel_array[6] = ADC_CHANNEL_LOAD1_CURRENT;       /* Load 1 */
	channel_array[7] = ADC_CHANNEL_LOAD1_VOLTAGE;
	channel_array[8] = ADC_CHANNEL_LOAD2_CURRENT;       /* Load 2 */
	channel_array[9] = ADC_CHANNEL_LOAD2_VOLTAGE;
	channel_array[10] = ADC_CHANNEL_PANEL_CURRENT;      /* Panel */
	channel_array[11] = ADC_CHANNEL_PANEL_VOLTAGE;
	channel_array[12] = ADC_CHANNEL_TEMPERATURE;        /* Temperature */
	adc_set_regular_sequence(ADC1, N_CONV, channel_array);

/* Reset averages for first run */
	for (j = 0; j < N_CONV; j++) av[j] = 0;

	while (1)
	{
        iwdgReset();
/* A/D conversions */
/* Wait until the next tick cycle */
		vTaskDelay(getMeasurementDelay() );
/* Reset watchdog counter */
        measurementWatchdogCount = 0;
/* Fire off a burst of conversions and average the results.
This averages out variations due to PWM provided they are not high frequency. */
        uint16_t burst = 0;
        for (burst=0; burst < N_SAMPLES; burst++)
        {
            adc_start_conversion_regular(ADC1);
/* Check if conversion ended */
            while (adcEOC() == 0) taskYIELD();
/* Sum over the sample set with scaling and offset for the interfaces and
temperature */
        	for (j = 0; j < N_CONV-1; j+=2)
            {
                av[j] += adcValue(j);
                av[j+1] += adcValue(j+1);
            }
            av[12] += adcValue(12);
        }

/* Process the averaged results */
/* Compute averages from the burst, scale and offset to the real quantities,
and reset the averages for the next cycle. */
        for (i=0; i<NUM_IFS; i++)
        {
            uint8_t k = i+i;
            currents.data[i] = ((av[k]/N_SAMPLES-CURRENT_OFFSET)*CURRENT_SCALE)/4096;
            voltages.data[i] = (av[k+1]/N_SAMPLES*VOLTAGE_SCALE+VOLTAGE_OFFSET)/4096;
            av[k] = 0;
            av[k+1] = 0;
        }
        temperature = ((av[12]/N_SAMPLES-TEMPERATURE_OFFSET)*TEMPERATURE_SCALE)/4096;
        av[12] = 0;

        for (i=0; i<NUM_BATS; i++)
        {
/* Compute the batteries' charge state by integration of current flow over time.
Currents are in amperes (x256). Divide by the measurement interval in seconds so
that charge is in Coulombs (x256). Limit charge to battery's capacity. Sign is
determined by the fact that positive current flows out of the batteries. */
            int32_t batteryCurrent = currents.dataArray.battery[i]-
                                        getBatteryCurrentOffset(i);
            accumulatedBatteryCharge[i] -= batteryCurrent*
                    (int32_t)(1000/(portTICK_RATE_MS*getMeasurementDelay()));

/* Check if a significant change in battery current occurred (more than about
400mA) and use this to estimate the resistance of the battery.
Note that battery resistance is scaled by 65536 due to its low real value.
The algorithm computes the averaged voltage and current steps and divides
them. This automatically weights the contributions of larger steps to
imcrease their overall effect over the smaller steps. The voltage and current
averaged are linear unbiassed estimators. */
            int32_t batteryVoltage = voltages.dataArray.battery[i];
            int32_t voltageStep = abs(batteryVoltage-lastBatteryVoltage[i]);
            int32_t currentStep = abs(batteryCurrent-lastBatteryCurrent[i]);
            if (currentStep > 100)
            {
/* Apply a weighted IIR filter to estimate the average voltage and current. */
/* The forgetting factor getAlphaR() (<1) is multiplied by 256 to make integer,
therefore the correction factors need to be divided back by 256. */
/* Seed the filter with the most recent measurements (rather than zero) */
                if (voltageStepAv[i] == 0) voltageStepAv[i] = voltageStep;
                voltageStepAv[i] = voltageStepAv[i] +
                         ((getAlphaR()*(voltageStep - voltageStepAv[i])) >> 8);
                if (currentStepAv[i] == 0) currentStepAv[i] = currentStep;
                currentStepAv[i] = currentStepAv[i] +
                         ((getAlphaR()*(currentStep - currentStepAv[i])) >> 8);
                if (currentStepAv[i] > 100)
                    batteryResistanceAv[i] = (voltageStepAv[i]<<16)/currentStepAv[i];
            }
            lastBatteryVoltage[i] = batteryVoltage;
            lastBatteryCurrent[i] = batteryCurrent;
        }
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Initialise Global Variables
*/

static void initGlobals(void)
{
    uint8_t i = 0;
    for (i=0; i<NUM_BATS; i++)
    {
        lastBatteryVoltage[i] = 0;
        lastBatteryCurrent[i] = 0;
        batteryResistanceAv[i] = 0;
        accumulatedBatteryCharge[i] = 0;
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Battery Resistance Average Estimate

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBatteryResistanceAv(int battery)
{
    return batteryResistanceAv[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Battery Accumulated Charge

When the charge accumulated has been read, it is reset to zero.
Need to protect this from interrupt and context switching.

Only one task should access this, otherwise there will be a major
synchronization problem. It is intended for passing the accumulation of charge
to the task that is monitoring the battery state.

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBatteryAccumulatedCharge(int battery)
{
    taskENTER_CRITICAL();
    int16_t accumulatedCharge = accumulatedBatteryCharge[battery];
    accumulatedBatteryCharge[battery] = 0;
    taskEXIT_CRITICAL();
    return accumulatedCharge;
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Measured Battery Current

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBatteryCurrent(int battery)
{
    return currents.dataArray.battery[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Measured Battery Voltage

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBatteryVoltage(int battery)
{
    return voltages.dataArray.battery[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Measured Load Current

@param[in] load: 0..NUM_LOADS-1
*/

int16_t getLoadCurrent(int load)
{
    return currents.dataArray.load[load];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Measured Load Voltage

@param[in] load: 0..NUM_LOADS-1
*/

int16_t getLoadVoltage(int load)
{
    return voltages.dataArray.load[load];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Measured Panel Current

@param[in] panel: 0..NUM_PANELS-1
*/

int16_t getPanelCurrent(int panel)
{
    return currents.dataArray.panel[panel];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Measured Panel Voltage

@param[in] panel: 0..NUM_PANELS-1
*/

int16_t getPanelVoltage(int panel)
{
    return voltages.dataArray.panel[panel];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Current through an Interface

@param[in] intf: 0..NUM_IFS-1
*/

int16_t getCurrent(int intf)
{
    return currents.data[intf];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Voltage at an Interface

@param[in] intf: 0..NUM_IFS-1
*/

int16_t getVoltage(int intf)
{
    return voltages.data[intf];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Temperature

*/

int32_t getTemperature(void)
{
    return temperature;
}

/*--------------------------------------------------------------------------*/
/** @brief Check the watchdog state

The watchdog counter is decremented. If it reaches zero then the task is reset.
*/

void checkMeasurementWatchdog(void)
{
    if (measurementWatchdogCount++ > 10*getMeasurementDelay()/getWatchdogDelay())
    {
        vTaskDelete(prvMeasurementTask);
    	xTaskCreate(prvMeasurementTask, (signed portCHAR * ) "Measurement", \
                configMINIMAL_STACK_SIZE, NULL, MEASUREMENT_TASK_PRIORITY, NULL);
        sendStringLowPriority("D","Measurement Restarted");
        recordString("D","Measurement Restarted");
    }
}

