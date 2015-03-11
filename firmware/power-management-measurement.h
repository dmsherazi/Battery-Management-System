/* STM32F1 Power Management for Solar Power

This header file contains defines and prototypes specific to the
measurement task.

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

#ifndef POWER_MANAGEMENT_MEASUREMENT_H_
#define POWER_MANAGEMENT_MEASUREMENT_H_

#include <stdint.h>
#include <stdbool.h>
#include "power-management-objdic.h"

/* Number of A/D converter channels used */
#define N_CONV 13
/* Number of samples taken and averaged of each quantity measured */
#define N_SAMPLES 1024

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/

void prvMeasurementTask(void *pvParameters);

/* Data access functions */
int16_t getBatteryResistanceAv(int battery);
int16_t getBatteryAccumulatedCharge(int battery);
int16_t getBatteryCurrent(int battery);
int16_t getBatteryVoltage(int battery);
int16_t getLoadCurrent(int load);
int16_t getLoadVoltage(int load);
int16_t getPanelCurrent(int panel);
int16_t getPanelVoltage(int panel);
int16_t getCurrent(int intf);
int16_t getVoltage(int intf);
int32_t getTemperature(void);
void checkMeasurementWatchdog(void);

#endif

