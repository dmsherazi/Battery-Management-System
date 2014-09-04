/* STM32F1 Power Management for Solar Power

Battery Charging Task Task

This task implements an algorithm to manage charging of batteries. Battery
current and terminal voltage are monitored constantly. If either rise beyond
the preset limits, charging is cut off and held off until the quantity drops
below the limit.

Average current is computed and used to determine when the algorithm is to
pass into the float stage. Float stage is normally set when the current goes
below c/50 but for aging cells this may need to be adjusted. The algorithm never
actually maintains a float charge. Rather the charger is removed and reapplied
later, passing through both bulk and absorption stages.

The management task designates all batteries to be charged with different
priority levels. When charging is removed from the priority battery, either
temporarily or when completed, the second priority battery is switched in.

The charge algorithm is:
1. Charge at maximum rate C/4 until the voltage reaches the specified limit.
2. Charge at the voltage limit until the current drops to the low
   threshold or timeout occurs.
3. For wet cells, charge briefly at the equalization voltage.

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
#include "power-management-charger-3ph.h"

/* Local Persistent Variables */
static uint16_t dutyCycle;      /* percentage times 256 */
static uint16_t dutyCycleMax;
static uint16_t absorptionPhaseTime[NUM_BATS];   /* time in absorption */
static int16_t absorptionPhaseCurrent[NUM_BATS];

/* Include the common code here so that everything compiles as a unit */
#include "power-management-charger-common.inc"

/*--------------------------------------------------------------------------*/
/* @brief Initialise Local Variables

*/

void initLocals3PH(void)
{
    dutyCycle = 50*256;                    /* percentage times 256 */
    dutyCycleMax = 100*256;
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        absorptionPhaseTime[i] = 0;
        absorptionPhaseCurrent[i] = 0;
        voltageAv[i] = 0;
        currentAv[i] = 0;
/* Set battery state if inappropriate. If in rest phase from the ICC
algorithm then move to absorption phase. */
        if (getBatteryChargingPhase(i) == restC)
            setBatteryChargingPhase(i,absorptionC);
    }
}

/*--------------------------------------------------------------------------*/
/* @brief Execute one cycle of the Three Phase Charger Algorithm

*/

void chargerControl3PH(uint8_t battery)
{
/* Compute the average current and voltage */
        calculateAverageMeasures();

/* If this is different to the battery currently under charge, change over
and reset the duty cycle. Nothing should happen if no charger is allocated. */
    if (battery != batteryUnderCharge)
    {
        batteryUnderCharge = battery;   /* Set new battery to charge */
        if (batteryUnderCharge > 0) dutyCycle = 50*256;
    }

/* Set the Switches to connect the panel to the selected battery if in
autotrack mode, otherwise leave settings on manual. */
    if (isAutoTrack()) setSwitch(batteryUnderCharge,PANEL);

    if (batteryUnderCharge > 0)
    {

        uint8_t index = batteryUnderCharge-1;

/* Manage the change from bulk to absorption phase. */
        if (voltageAv[index] > voltageLimit(getAbsorptionVoltage(index)))
            setBatteryChargingPhase(index,absorptionC);  /* Change. */

/* Check that the battery doesn't remain too long in the absorption phase while
the current is not falling (within a 5% error). 0.5 hour max. Then go to float.
NOTE: in the following all battery currents are negative while charging. */
        if ((getBatteryChargingPhase(index) == absorptionC) &&
            ((absorptionPhaseCurrent[index]*240)/256 > currentAv[index]))
        {
            absorptionPhaseTime[index]++;
            if (absorptionPhaseTime[index] > FLOAT_DELAY/getChargerDelay())
            {
                setBatteryChargingPhase(index,floatC);
                absorptionPhaseTime[index] = 0;
                absorptionPhaseCurrent[index] = 0;
            }
        }
        else if (getBatteryChargingPhase(index) == bulkC)
        {
            absorptionPhaseTime[index] = 0;
            absorptionPhaseCurrent[index] = currentAv[index];
        }

/* Manage the change to float phase when the current drops below the float
threshold. This is done on the averaged current as rapid response is not
essential. (Note: measured currents are negative while charging).
When the change occurs, force the SoC to 100%. This may not be correct if
the battery is faulty with a low terminal voltage, but that case is handled
by the resetBattery function. */
        if ((getBatteryChargingPhase(index) == absorptionC) &&
            (-currentAv[index] < getFloatStageCurrent(index)))
        {
            setBatteryChargingPhase(index, floatC);
            resetBatterySoC(batteryUnderCharge-1);
        }

/* Manage the change to bulk phase when the terminal voltage drops below the
absorption threshold, and the duty cycle reaches 100%. This can happen when the
charger voltage drops, as in a solar panel application. */
        if ((getBatteryChargingPhase(index) == absorptionC) &&
            (voltageAv[index] < voltageLimit(getAbsorptionVoltage(index))*240/256) &&
            (dutyCycle == dutyCycleMax))
            setBatteryChargingPhase(index,bulkC);

/* Manage the float phase voltage limit. */
        if (getBatteryChargingPhase(index) == floatC)
        {
            adaptDutyCycle(voltageAv[index],getFloatVoltage(index),&dutyCycle);
        }

/* Manage the absorption phase voltage limit. */
        adaptDutyCycle(voltageAv[index],getAbsorptionVoltage(index),&dutyCycle);

/* Overcurrent protection:
Compute the peak current from duty cycle (assumes current goes from 0 to a peak)
then if the peak is greater than the battery's current limit, reduce the
maximum duty cycle. Limit the duty cycle to this.
This is done on the directly measured current for rapid response. */
        int16_t current = getBatteryCurrent(index)-getBatteryCurrentOffset(index);
        if (dutyCycle < MIN_DUTYCYCLE) dutyCycle = MIN_DUTYCYCLE;
        int32_t currentPeak = -((int32_t)current*100)/dutyCycle;
        if (currentPeak > getBulkCurrentLimit(index))
            dutyCycleMax = getBulkCurrentLimit(index)*256/currentPeak;
        else dutyCycleMax = 100*256;

/* Set the duty cycle. */
/* Never let duty cycle go too near zero else it will not recover. Set to a
value that will allow it to grow again if needed (round-off error problem). */
        if (dutyCycle < MIN_DUTYCYCLE) dutyCycle = MIN_DUTYCYCLE;
        if (dutyCycle > dutyCycleMax) dutyCycle = dutyCycleMax;
        uint16_t dutyCycleActual = dutyCycle;
/* If the voltage drifts too high in float phase, turn off charging altogether.*/
        if ((getBatteryChargingPhase(index) == floatC) &&
            (voltageAv[index] > voltageLimit(getFloatVoltage(index))*260/256))
            dutyCycleActual = 0;
/* If the voltage drifts above absorption voltage in any phase, turn off
charging altogether..*/
        if (voltageAv[index] > voltageLimit(getAbsorptionVoltage(index))*260/256)
            dutyCycleActual = 0;
/* If the voltage drifts above the maximum in any phase, turn off charging
altogether as the duty cycle is not handling this case..*/
        if (voltageAv[index] > VOLTAGE_MAX*253/256)
            dutyCycleActual = 0;
        pwmSetDutyCycle(dutyCycleActual);
    }
}


