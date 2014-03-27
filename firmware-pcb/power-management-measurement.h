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

/* Calibration */
/* For current the scaling factor gives a value in 1/256 Amp precision.
Subtract this from the measured value and scale by this factor.
Then after averaging scale back by 4096 to give the values used here.
Simply scale back further by 256 to get the actual (floating point)
current. Thus the results here are 256 times the current in amperes. */

#define CURRENT_OFFSET 2053

/* Current scale: amplifier gain 10.4 times ref voltage 3.280 times 256 */

#define CURRENT_SCALE 8373

/* Voltage offset: 5 (2 times local ref 2.5V) times the gain 1.833
times 256 times 4096 */

#define VOLTAGE_OFFSET 9610199

/* Voltage scale: amplifier gain 1.833 times ref voltage 3.298 times 256 */

#define VOLTAGE_SCALE 1548

/* Temperature measurement via LM335 for reference voltage 3.280V.
Scale is 3.28V over 10mV per degree C times 256.
Offset is 2.732V at 0 degrees C over 3.280 times 4096. */

#define TEMPERATURE_SCALE   328*256
#define TEMPERATURE_OFFSET  3412

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

