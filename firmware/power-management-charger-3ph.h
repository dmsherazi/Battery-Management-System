/* STM32F1 Power Management for Solar Power

This header file contains defines and prototypes specific to the charging
task three phase algorithm.

Initial 02 September 2014

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

#ifndef POWER_MANAGEMENT_CHARGER_3PH_H_
#define POWER_MANAGEMENT_CHARGER_3PH_H_

#include <stdint.h>
#include <stdbool.h>

/* This defines the lowest the duty cycle is allowed to go as it may not recover
when it needs to be raised. Check that the duty cycle reduction doesn't
cause duty cycle to go to zero at any time. The lower this is, the longer
it will take the duty cycle to rise in response to changes. */
#define MIN_DUTYCYCLE   256

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/

void initLocals3PH(void);
void chargerControl3PH(uint8_t battery);

#endif

