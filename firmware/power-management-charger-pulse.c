/* STM32F1 Power Management for Solar Power

Battery Charging Task Pulse Algorithm

This task implements an algorithm to manage charging of batteries. Battery
current and terminal voltage are monitored constantly.

The charging task uses the Pulse Charge algorithm. This algorithm stops
charging the battery when the gassing voltage is reached, and allows it to rest
for a minimum period of time. Another battery in bulk charge phase can then be
placed on charge. The batteries alternate between the charging and rest phases
while respecting the minimum rest time. This cycle continues until the average 
current over the cycle period drops to the float limit or continues for an
extended period. At this point the battery is considered as charged and only
side reactions are occurring.

The monitor designates a battery under charge. This will normally be a battery
with the lowest SoC. The charging algorithm uses this to prioritise the
charging of the batteries.

Initial 2 October 2014
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
#include "power-management-charger-pulse.h"

static uint32_t offTime[NUM_BATS];  /* Battery time in rest */
static uint32_t onTime[NUM_BATS];   /* Battery time in charge */
static int32_t averageCurrent[NUM_BATS];    /* average current over the cycle */
static int64_t cumulatedCurrent[NUM_BATS];
static uint8_t batteryUnderCharge;

/*--------------------------------------------------------------------------*/
/* @brief Initialise Local Variables

*/

void initLocalsPulse(void)
{
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        offTime[i] = 0;
        onTime[i] = 0;
        averageCurrent[i] = 0;
        cumulatedCurrent[i] = 0;
/* Set battery state if inappropriate. If in absorption phase from the 3PH
algorithm then move to rest phase. */
        if (getBatteryChargingPhase(i) == absorptionC)
            setBatteryChargingPhase(i,restC);
    }
/* Turn on charger by setting PWM to maximum. */
    pwmSetDutyCycle(100*256);
}

/*--------------------------------------------------------------------------*/
/* @brief Execute one cycle of the ICC Algorithm

*/

void chargerControlPulse(uint8_t battery)
{
/* Compute the average current and voltage */
    calculateAverageMeasures();

/* Battery will only be zero if power source is absent or manually
disconnected. */
    if (battery > 0)
    {
/* If a battery is in rest phase, increment its off-time counter,
or if in bulk phase, its on-time counter and add to the cumulated current. */
        uint8_t i = 0;
        for (i=0; i < NUM_BATS; i++)
        {
            if (getBatteryChargingPhase(i) == restC)
                offTime[i]++;
            else if (getBatteryChargingPhase(i) == bulkC)
            {
                onTime[i]++;
                cumulatedCurrent[i] += getCurrentAv(i);
            }
        }
/* Manage change from bulk to rest phase. Set battery on charge to none.
As the battery has now finished its charging phase, compute the average
current over the last full cycle. */
        if (batteryUnderCharge > 0)
        {
            uint8_t index = batteryUnderCharge-1;
            if ((getBatteryChargingPhase(index) == bulkC) &&
                (getVoltageAv(index) >
                 voltageLimit(getAbsorptionVoltage(index))))
            {
                setBatteryChargingPhase(index,restC);
                batteryUnderCharge = 0;
                averageCurrent[index] = cumulatedCurrent[index];
/* If computed average current is below float threshold, or time exceeded,
go to float phase */
                if ((averageCurrent[index] < getFloatStageCurrent(index)) ||
                    (offTime[index] > (uint32_t)(FLOAT_DELAY*1024)/getChargerDelay()))
                {
                    averageCurrent[index] = 0;
                    setBatteryChargingPhase(index,floatC);
                }
            }
        }
/* If the charger isn't allocated, check all batteries starting at designated,
for one in bulk phase. */
        if (batteryUnderCharge == 0)
        {
            uint8_t i = 0;
            for (i=0; i < NUM_BATS; i++)
            {
                uint8_t j = i + battery;
                if (j > NUM_BATS) j -= NUM_BATS;
/* Allocate the charger to the battery if it is in bulk phase, or if it
is in rest phase and off time exceeds the minimum. */
                if ((getBatteryChargingPhase(j-1) == bulkC) ||
                   ((getBatteryChargingPhase(j-1) == restC) &&
                    (offTime[j-1] > (uint32_t)(MINIMUM_OFF_TIME*1024)/getChargerDelay())))
                {
                    batteryUnderCharge = j;
                    setBatteryChargingPhase(j-1,bulkC);
                    break;
                }
            }
        }

/* Set the switches to connect the panel to the selected battery if it
is in an active charging phase, otherwise turn it off. */
        if (getBatteryChargingPhase(batteryUnderCharge-1) == bulkC)
            setSwitch(batteryUnderCharge,PANEL);
        else setSwitch(0,PANEL);
    }
}

