/* STM32F1 Power Management for Solar Power

Battery Charging Task

This task implements an algorithm to manage charging of batteries. Battery
current and terminal voltage are monitored constantly. If either rise beyond
the preset limits, charging is cut off and held off until the quantity drops
below the limit.

This file contains common code for different algorithms. It must be included
within the source file for each algorithm.

Initial 14 June 2014
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

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "power-management.h"
#include "power-management-comms.h"
#include "power-management-file.h"
#include "power-management-hardware.h"
#include "power-management-measurement.h"
#include "power-management-monitor.h"
#include "power-management-charger.h"
#include "power-management-charger-icc.h"
#include "power-management-charger-3ph.h"
#include "power-management-objdic.h"

/* Local Prototypes */
static void initGlobals(void);

/* Local Persistent Variables */
static battery_Ch_States batteryChargingPhase[NUM_BATS];
static uint8_t chargerWatchdogCount;

/*--------------------------------------------------------------------------*/
/* @brief Charging Task

This task runs over long times, implementing the charging algorithm on a
battery or batteries that have been designated for charging.
*/

void prvChargerTask(void *pvParameters)
{

    initGlobals();
    initLocalsICC();
    initLocals3PH();

	while (1)
	{
/* Wait until the next tick cycle */
        vTaskDelay(getChargerDelay());
/* Reset watchdog counter */
        chargerWatchdogCount = 0;

/* Change each battery to bulk phase when it is in float phase and the
terminal voltage drops below a charging restart threshold (95%). */
        uint8_t i;
        for (i=0; i<NUM_BATS; i++)
        {
            if ((batteryChargingPhase[i] == floatC) &&
                (getBatterySoC(i) < FLOAT_BULK_SOC))
                batteryChargingPhase[i] = bulkC;
        }

/* get battery under charge from the manual panel switch settings. */
        uint8_t battery = getPanelSwitchSetting();

        if (isICC()) chargerControlICC(battery);
        else chargerControl3PH(battery);

    }
}

/*--------------------------------------------------------------------------*/
/** @brief Initialise Global Variables

Set the charger default parameters.
*/

static void initGlobals(void)
{
    chargerWatchdogCount = 0;
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Battery Charging Phase Variable

@param[in] battery: 0..NUM_BATS-1
*/

battery_Ch_States getBatteryChargingPhase(int battery)
{
    return batteryChargingPhase[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Set the Battery Charging Phase Variable

@param[in] battery: 0..NUM_BATS-1
*/

void setBatteryChargingPhase(int battery, battery_Ch_States chargePhase)
{
    batteryChargingPhase[battery] = chargePhase;
}

/*--------------------------------------------------------------------------*/
/** @brief Check the watchdog state

The watchdog counter is decremented. If it reaches zero then the task is reset.
*/

void checkChargerWatchdog(void)
{
    if (chargerWatchdogCount++ > 10*getChargerDelay()/getWatchdogDelay())
    {
        vTaskDelete(prvChargerTask);
	    xTaskCreate(prvChargerTask, (signed portCHAR * ) "Charger", \
                    configMINIMAL_STACK_SIZE, NULL, CHARGER_TASK_PRIORITY, NULL);
        sendStringLowPriority("D","Charger Restarted");
        recordString("D","Charger Restarted");
    }
}


