/* STM32F1 Power Management for Solar Power

Battery Charging Task

This task implements an algorithm to manage charging of batteries. Battery
current and terminal voltage are monitored constantly. If either rise beyond
the preset limits, charging is cut off and held off until the quantity drops
below the limit.

The charging task uses the Interrupted Charge Control algorithm. This
algorithm stops charging the battery when the gassing voltage is reached, and
allows it to rest until the terminal voltage falls to a specified lower limit.
Then it applies charge for a few seconds alternating with rest periods until
the terminal voltage again reaches the gassing voltage. Then the battery is
disconnected from the charger until the terminal voltage drops below a
specified limit, at which point the charger reverts to bulk phase.

1. In the bulk phase charge at maximum rate until the voltage reaches the
   gassing voltage. A slotted time scheme with 5 second intervals allows other
   batteries to receive their charge if needed. If other batteries do not need
   their allocated time slot then reallocate it to other bulk charging
   batteries, if any, in a round-robin scheme.
2. Rest until the voltage falls to the equivalent of 97% SoC.
3. Charge for 5 seconds at full rate.
4. Rest for 10 seconds.
5. Stop when the gassing voltage is reached.

The monitor designates a battery under charge. This will be a battery with a
low SoC. The charging algorithm uses this to prioritise the charging of the
batteries. The designated battery will be charged exclusively while in bulk
phase. When it enters the rest phase the other batteries will be charged
without priority.

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

static uint8_t batteryNextIndex;    /* Next battery for spare slot */
static uint8_t slotBattery;         /* Slot available for battery */
static uint8_t slotTime;            /* Slot time counter */

/* Include the common code here so that everything compiles as a unit */
#include "power-management-charger-common.inc"

/*--------------------------------------------------------------------------*/
/* @brief Initialise Local Variables

*/

void initLocalsICC(void)
{
    batteryNextIndex = 1;
    slotBattery = 1;
    slotTime = 0;
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        voltageAv[i] = 0;
        currentAv[i] = 0;
    }
/* Turn on charger by setting PWM to maximum. */
    pwmSetDutyCycle(100*256);
}

/*--------------------------------------------------------------------------*/
/* @brief Execute one cycle of the ICC Algorithm

*/

void chargerControlICC(uint8_t battery)
{
/* Compute the average current and voltage */
    calculateAverageMeasures();

    slotTime++;

/* Battery will only be zero if manually set or if power source is absent. */
    if (battery > 0)
    {
/* If the designated battery under charge is in bulk phase, allocate it in all
slots to ensure it gets priority charge. */
        if (getBatteryChargingPhase(battery-1) == bulkC)
            batteryUnderCharge = battery;
/* else allocate the battery under charge to the battery owning the slot,
unless it happens to be enforced idle (float or rest), in which case
yield it up to the next battery under bulk charge in turn. */ 
        else
        {
/* Wait until the end of the slot. */
            if (slotTime > SLOT_PERIOD/getChargerDelay())
            {
                slotTime = 0;
                slotBattery++;
                if (slotBattery > NUM_SLOTS) slotBattery = 1;
/* Check if slot is needed by the battery.
If the slot's allocated battery is not in use, check other batteries.
If they are in bulk phase, grab the slot.
This code should allow an even allocation of batteries in bulk charge
over time. */
                if ((getBatteryChargingPhase(slotBattery-1) == floatC) ||
                    (getBatteryChargingPhase(slotBattery-1) == restC))
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
                else batteryUnderCharge = slotBattery;
            }
        }
/* Set the switches to connect the panel to the selected battery if it
is in an active charging phase, otherwise turn it off to prevent overvoltage
during the slot. */
        uint8_t index = batteryUnderCharge;
        if ((getBatteryChargingPhase(index) == absorptionC) ||
            (getBatteryChargingPhase(index) == bulkC))
            setSwitch(batteryUnderCharge,PANEL);
        else setSwitch(0,PANEL);
    }

    uint8_t index = 0;
    for (index=0; index < NUM_BATS; index++)
    {
/* Manage change from absorption to float phase. */
        if ((getBatteryChargingPhase(index) == absorptionC) &&
            (voltageAv[index] > voltageLimit(getAbsorptionVoltage(index))))
            setBatteryChargingPhase(index,floatC);
/* Manage change from rest to absorption (pulsed charging) phase */
        if ((getBatteryChargingPhase(index) == restC) &&
            (computeSoC(voltageAv[index],getTemperature(),
                           getBatteryType(index)) < 97*256))
            setBatteryChargingPhase(index,absorptionC);
/* Manage change from bulk to rest phase */
        if ((getBatteryChargingPhase(index) == bulkC) &&
            (voltageAv[index] > voltageLimit(getAbsorptionVoltage(index))))
            setBatteryChargingPhase(index,restC);
    }
}


