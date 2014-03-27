/* STM32F1 Power Management for Solar Power

This header file contains defines and prototypes specific to the overall
program.

Initial 13 July 2013
FreeRTOS 9 August 2013
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

#ifndef POWER_MANAGEMENT_H_
#define POWER_MANAGEMENT_H_

/*--------------------------------------------------------------------------*/
/* Task Priorities */
/*--------------------------------------------------------------------------*/

#define WATCHDOG_TASK_PRIORITY      ( tskIDLE_PRIORITY + 0 )
#define FILE_TASK_PRIORITY          ( tskIDLE_PRIORITY + 0 )
#define CHARGER_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )
#define MONITOR_TASK_PRIORITY       ( tskIDLE_PRIORITY + 1 )
#define COMMS_TASK_PRIORITY         ( tskIDLE_PRIORITY + 2 )
#define MEASUREMENT_TASK_PRIORITY   ( tskIDLE_PRIORITY + 3 )

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/

#endif

