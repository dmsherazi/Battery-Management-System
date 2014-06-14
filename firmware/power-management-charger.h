/* STM32F1 Power Management for Solar Power

This header file contains defines and prototypes specific to the charging
task.

Initial 18 October 2013

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

#ifndef POWER_MANAGEMENT_CHARGER_H_
#define POWER_MANAGEMENT_CHARGER_H_

/* This defines the lowest the duty cycle is allowed to go as it may not recover
when it needs to be raised. Check that the duty cycle reduction doesn't
cause duty cycle to go to zero at any time. The lower this is, the longer
it will take the duty cycle to rise in response to changes. */
#define MIN_DUTYCYCLE   256

/* The period, 5 seconds, for the pulsed charging phase of the ICC algorithm */
#define SLOT_PERIOD     5*1024

/* Number of slots in the ICC algorithm for pulsed charging cycle. */
#define NUM_SLOTS       3

/* SoC above which charging is stopped in float phase */
#define FLOAT_BULK_SOC  95*256

#include <stdint.h>
#include <stdbool.h>

/* Local Persistent Variables */
static battery_Ch_States batteryChargingPhase[NUM_BATS];
static uint8_t chargerWatchdogCount;
static int16_t voltageAv[NUM_BATS];
static int16_t currentAv[NUM_BATS];
static uint8_t batteryUnderCharge;      /* zero if no battery charging */

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/
void prvChargerTask(void *pvParameters);

battery_Ch_States getBatteryChargingPhase(int battery);
void checkChargerWatchdog(void);

#endif

