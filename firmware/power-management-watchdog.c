/* STM32F1 Power Management for Solar Power

Defines the Watchdog Task. This task monitors other tasks for aberrant
behaviour. Each task runs through its program and then waits a specified period
of time. The tasks are required to reset a timeout variable on each pass.
The watchdog task then monitors these variables to detect if the associated
task has stopped running for any reason. That task is then reset (deleted and
recreated).

the tasks monitored are the monitor, charger and measurement tasks. The comms
and file tasks operate in response to queued messages and these will normally
block indefinitely waiting for input. As such they are not monitored in this
way.

The watchdog task activates a hardware IWDG watchdog timer to protect itself.

Initial 15 March 2014
*/

/*
 * This file is part of the battery-management-system project.
 *
 * Copyright 2014 K. Sarkies <ksarkies@internode.on.net>
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

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"

#include "power-management-board-defs.h"
#include "power-management-objdic.h"
#include "power-management-hardware.h"
#include "power-management-monitor.h"
#include "power-management-measurement.h"
#include "power-management-charger.h"
#include "power-management-file.h"
#include "power-management-comms.h"

/* Local Prototypes */

/* Local Persistent Variables */

/*--------------------------------------------------------------------------*/
/* @brief Watchdog Task

Each call to the check functions in the task APIs should restart the task if a
timeout has occurred.
*/

void prvWatchdogTask(void *pvParameters)
{
	while (1)
	{
/* Reset the hardware independent watchdog timer */
        iwdgReset();
		vTaskDelay(getWatchdogDelay());
        checkChargerWatchdog();
        checkMeasurementWatchdog();
        checkMonitorWatchdog();
    }
}

