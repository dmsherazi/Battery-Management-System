/* STM32F1 Power Management for Solar Power

Battery Charging Task

This task implements an algorithm to manage charging of batteries. Battery
current and terminal voltage are monitored constantly. If either rise beyond
the preset limits, charging is cut off and held off until the quantity drops
below the limit.

The charging task uses the Intermittent Charge algorithm. This algorithm stops
charging the battery when the gassing voltage is reached, and allows it to rest
until the terminal voltage falls to a specified lower limit. Then it reapplies
charge until the gassing voltage is again reached. This cycle continues until
the average current over the cycle period drops to the float limit or the duty
cycle continues for an extended period. At this point the battery is charged
and only side reactions are occurring.

The monitor designates a battery under charge. This will normally be a battery
with the lowest SoC. The charging algorithm uses this to prioritise the
charging of the batteries.

Initial 4 September 2014
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
#include "power-management-charger-ic.h"

static uint8_t batteryNextIndex;    /* Next battery for spare slot */
static uint32_t bulkTime;           /* length of a charge cycle */
static uint32_t restTime;           /* length of a rest cycle */
static uint32_t cycleTime;          /* length of a cycle */
static int32_t accumulatedCharge;   /* accumulated charge in a cycle */
static int32_t averageCurrent;      /* average current over the cycle */
static uint8_t batteryUnderCharge;

/*--------------------------------------------------------------------------*/
/* @brief Initialise Local Variables

*/

void initLocalsIC(void)
{
    bulkTime = 1;
    restTime = 0;
    accumulatedCharge = 0;
    averageCurrent = 0;
    batteryNextIndex = 1;
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
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

void chargerControlIC(uint8_t battery)
{
/* Compute the average current and voltage */
    calculateAverageMeasures();

    uint8_t index = battery-1;
/* Compute the average current over a charge/rest cycle for the battery
under charge only. */
    if (getBatteryChargingPhase(index) == bulkC)
    {
/* When a cycle is completed the average current over the entire cycle is found.
The accumulated charge and rest time are zeroed ready for the next cycle. */
        if (bulkTime == 0)      /* Have just completed a cycle */
        {
            cycleTime += restTime;
            if (cycleTime > 0) averageCurrent = accumulatedCharge/cycleTime;
            else averageCurrent = 0;
            accumulatedCharge = 0;
            restTime = 0;
        }
/* Build up the accumulated charge over the bulk phase period. */
        accumulatedCharge += getBatteryCurrent(index)-getBatteryCurrentOffset(index);
        bulkTime++;
    }
/* When the charging phase passes to the rest phase, keep the time in bulk
phase and reset the bulk timer to signal when the cycle is ended. */
    else if (getBatteryChargingPhase(index) == restC)
    {
        if (restTime == 0)
        {
            cycleTime = bulkTime;
            bulkTime = 0;
        }
        restTime++;
    }

/* Manage the change to float phase when the current drops below the float
threshold. (Note: measured currents are negative while charging).
When the change occurs, force the SoC to 100%. This may not be correct if
the battery is faulty with a low terminal voltage, but that case is handled
by the resetBattery function.
Also change to float if the cycle time has gone too long. */
    if ((-averageCurrent < getFloatStageCurrent(index)) ||
        (restTime > (FLOAT_DELAY*1024)/getChargerDelay()))
    {
        setBatteryChargingPhase(index, floatC);
        resetBatterySoC(index);
    }

/* Battery will only be zero if manually set or if power source is absent. */
    if (battery > 0)
    {
/* If the designated battery under charge is in bulk phase, allocate it to
ensure it gets priority charge. */
        if (getBatteryChargingPhase(index) == bulkC)
            batteryUnderCharge = battery;
/* else allocate the battery under charge to the next battery under bulk charge
in turn. */ 
        else
        {
            uint8_t i = 0;
            for (i=0; i < NUM_BATS; i++)
            {
                uint8_t j = i + batteryNextIndex + 1;
                if (j > NUM_BATS) j -= NUM_BATS;
                if (getBatteryChargingPhase(j-1) == bulkC)
                {
                    batteryUnderCharge = j;
                    batteryNextIndex = j;
                    break;
                }
            }
/* If no battery found to take the slot, turn off charger. */
            if (i >= NUM_BATS) batteryUnderCharge = 0;
        }
/* Set the switches to connect the panel to the selected battery if it
is in an active charging phase, otherwise turn it off. */
        uint8_t index = batteryUnderCharge-1;
        if (getBatteryChargingPhase(index) == bulkC)
            setSwitch(batteryUnderCharge,PANEL);
        else setSwitch(0,PANEL);
    }

    for (index=0; index < NUM_BATS; index++)
    {
/* Manage change from rest to bulk phase */
        if ((getBatteryChargingPhase(index) == restC) &&
            (computeSoC(getVoltageAv(index),getTemperature(),
                           getBatteryType(index)) < REST_VOLTAGE))
            setBatteryChargingPhase(index,absorptionC);
/* Manage change from bulk to rest phase */
        if ((getBatteryChargingPhase(index) == bulkC) &&
            (getVoltageAv(index) > voltageLimit(getAbsorptionVoltage(index))))
            setBatteryChargingPhase(index,restC);
    }
}


