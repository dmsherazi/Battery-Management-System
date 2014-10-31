/* STM32F1 Power Management for Solar Power

This header file contains defines and prototypes specific to the hardware
initialization and management.

Copyright K. Sarkies <ksarkies@internode.on.net> 23 August 2013

Initial 29 September 2013
*/

/*
 * This file is part of the power-management project.
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

#ifndef POWER_MANAGEMENT_HARDWARE_H_
#define POWER_MANAGEMENT_HARDWARE_H_

#include <stdint.h>
#include <stdbool.h>

/* Number of A/D converter channels available (STM32F103) */
#define NUM_CHANNEL 16

/* Timer parameters */
/* register value representing a PWM period of 50 microsec (5 kHz) */
#define PWM_PERIOD      14400

/* USART */
#define BAUDRATE        38400

/* Watchdog Timer Timeout Period in ms */
#define IWDG_TIMEOUT_MS 1500

/* Flash. Largest page size compatible with most families used.
(note only STM32F1xx,  STM32F05x have compatible memory organization). */
#define FLASH_PAGE_SIZE 2048

/*--------------------------------------------------------------------------*/
/* Interface Prototypes */
/*--------------------------------------------------------------------------*/
void prvSetupHardware(void);
uint32_t adcValue(uint8_t channel);
uint8_t adcEOC(void);
uint16_t getIndicators(void);
void setSwitch(uint8_t battery, uint8_t setting);
uint8_t getSwitchControlBits(void);
void setSwitchControlBits(uint8_t settings);
void overCurrentReset(uint8_t interface);
void overCurrentRelease(uint8_t interface);
void pwmSetDutyCycle(uint16_t dutyCycle);
void commsEnableTxInterrupt(uint8_t enable);
void flashReadData(uint32_t *flashBlock, uint8_t *data, uint16_t size);
uint32_t flashWriteData(uint32_t *flashBlock, uint8_t *data, uint16_t size);
uint32_t getTimeCounter();
void setTimeCounter(uint32_t time);
void updateTimeCount(void);
void iwdgReset(void);

#endif

