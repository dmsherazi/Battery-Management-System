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

/* SoC above which charging is stopped in float phase */
#define FLOAT_BULK_SOC  95*256

/* Time to wait before passing to float. 2 hours, in seconds. */
#define FLOAT_DELAY     7200

/* Minimum time that the battery is in a rest phase, in seconds. */
#define REST_OFF_TIME 30

/* Minimum time that the battery is in the absorption phase, in seconds. */
#define ABSORPTION_OFF_TIME 90

/* This defines the lowest the duty cycle is allowed to go as it may not recover
when it needs to be raised. Check that the duty cycle reduction doesn't
cause duty cycle to go to zero at any time. The lower this is, the longer
it will take the duty cycle to rise in response to changes. */
#define MIN_DUTYCYCLE   256

#include <stdint.h>
#include <stdbool.h>

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/
void prvChargerTask(void *pvParameters);
void checkChargerWatchdog(void);
int16_t getVoltageAv(int index);
int16_t getCurrentAv(int index);
battery_Ch_States getBatteryChargingPhase(int index);
void resetChargeAlgorithm();
void setBatteryChargingPhase(int index, battery_Ch_States chargePhase);

#endif

