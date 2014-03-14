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
 * This file is part of the power-management project.
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

#include "power-management-comms.h"

#include "power-management-hardware.h"
#include "power-management-measurement.h"
#include "power-management-monitor.h"
#include "power-management-charger.h"

/* Local Prototypes */
static void initGlobals(void);
static int16_t voltageLimit(uint16_t limitV);
static void adaptDutyCycle(int16_t voltage, int16_t vLimit, uint16_t* dutyCycle);

/* Global Variables */
static battery_Ch_States batteryChargingPhase[NUM_BATS];
static int16_t chargingMeasure[NUM_BATS];       /* ad-hoc state measure */

/* Local Variables */

/*--------------------------------------------------------------------------*/
/* @brief Charging Task

This task runs over long times, implementing the charging algorithm on a
battery that have been designated for charging.
*/

void prvChargerTask(void *pvParameters)
{
    uint16_t dutyCycle = 50*256;                    /* percentage times 256 */
    uint16_t dutyCycleMax = 100*256;
    uint8_t battery = 0;
    static uint16_t absorptionPhaseTime[NUM_BATS];   /* time in absorption */
    static int16_t absorptionPhaseCurrent[NUM_BATS];
    static int16_t voltageAv[NUM_BATS];
    static int16_t currentAv[NUM_BATS];
    static uint8_t batteryUnderCharge = 0;  /* zero if no battery charging */
    static uint16_t bulkCurrent = 0;

    initGlobals();

    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        voltageAv[i] = 0;
        currentAv[i] = 0;
        absorptionPhaseTime[i] = 0;
        absorptionPhaseCurrent[i] = 0;
        chargingMeasure[i] = 0;
    }

	while (1)
	{
        sendStringLowPriority("DD","Charger");

/* Wait until the next tick cycle */
	    vTaskDelay(getChargerDelay());

/* Get the battery being charged, if any, from the switch settings.
These monitor task will set these to select the battery to charge. */
        uint8_t switchSettings = getSwitchControlBits();
        battery = (switchSettings >> 4) & 0x03;
/* If this is different to the battery currently under charge, change over. */
        if (battery != batteryUnderCharge)  /* Change of battery to charge */
        {
            batteryUnderCharge = battery;   /* Set new battery to charge */
            voltageAv[battery-1] = 0;
            currentAv[battery-1] = 0;
            dutyCycle = 50*256;
        }

/* If a battery is allocated to a load, reset its charging phase to "bulk".
Only do this if it is not also under charge, as in such a case the charger may
be supplying the load current. */
        uint8_t batteryUnderLoad1 = switchSettings & 0x03;
        if (batteryUnderLoad1 != battery)
            batteryChargingPhase[batteryUnderLoad1-1] = bulkC;
        uint8_t batteryUnderLoad2 = (switchSettings >> 2) & 0x03;
        if (batteryUnderLoad2 != battery)
            batteryChargingPhase[batteryUnderLoad2-1] = bulkC;

/* Compute the averaged voltages and currents to manage phase switchover.
Use first order exponential filters, separate coefficients. */
        if (battery > 0)
        {
            uint8_t index = battery-1;
            int16_t current = getBatteryCurrent(index)-getBatteryCurrentOffset(index);
            int16_t voltage = getBatteryVoltage(index);
/* Seed the filter with the most recent measurement (rather than zero) */
            if (voltageAv[index] == 0) voltageAv[index] = voltage;
            if (currentAv[index] == 0) currentAv[index] = current;
/* IIR filters with fairly short time constant */
            voltageAv[index] = voltageAv[index] +
                        ((getAlphaV()*(voltage - voltageAv[index]))>>8);
            currentAv[index] = currentAv[index] +
                        ((getAlphaC()*(current - currentAv[index]))>>8);

/* Manage the float phase voltage limit. */
            if (batteryChargingPhase[index] == floatC)
                adaptDutyCycle(voltageAv[index],getFloatVoltage(index),&dutyCycle);

/* Manage the absorption phase voltage limit. */
            adaptDutyCycle(voltageAv[index],getAbsorptionVoltage(index),&dutyCycle);

/* Manage the change from bulk to absorption phase. */
            if (voltageAv[index] > voltageLimit(getAbsorptionVoltage(index)))
                batteryChargingPhase[index] = absorptionC;  /* Change. */

/* Check that battery doesn't remain too long in absorption phase while the
current is not falling (within a 5% error). 0.5 hour max. Then go to float. */
            if (batteryChargingPhase[index] == absorptionC)
            {
                if (((absorptionPhaseCurrent[index]*240)/256 < currentAv[index]))
                {
                    absorptionPhaseTime[index]++;
                    if (absorptionPhaseTime[index] > 1800000/getChargerDelay())
                    {
                        batteryChargingPhase[index] = floatC;
                        absorptionPhaseTime[index] = 0;
                        absorptionPhaseCurrent[index] = 0;
                    }
                }
                else
                {
                    absorptionPhaseTime[index] = 0;
                    absorptionPhaseCurrent[index] = currentAv[index];
                }
            }

/* Manage the change to float phase when the current drops below the float
threshold. This is done on the averaged current as rapid response is not
essential. (Note: measured currents are negative while charging). */
            if ((batteryChargingPhase[index] == absorptionC) &&
                (-currentAv[index] < getFloatStageCurrent(index)))
                batteryChargingPhase[index] = floatC;

/* Manage the change to bulk phase when the terminal voltage drops below the
absorption threshold, and the duty cycle reaches 100%. */
            if ((batteryChargingPhase[index] == absorptionC) &&
                (voltageAv[index] < voltageLimit(getAbsorptionVoltage(index))) &&
                (dutyCycle == dutyCycleMax))
                batteryChargingPhase[index] = bulkC;

/* Overcurrent protection:
Compute the peak current from duty cycle (assumes current goes from 0 to a peak)
then if the peak is greater than the battery's current limit, reduce the
maximum duty cycle. Limit the duty cycle to this.
This is done on the directly measured current for rapid response. */
            if (dutyCycle < MIN_DUTYCYCLE) dutyCycle = MIN_DUTYCYCLE;
            int32_t currentPeak = -((int32_t)current*100)/dutyCycle;
            if (currentPeak > getBulkCurrentLimit(index))
                dutyCycleMax = getBulkCurrentLimit(index)*256/currentPeak;
            else dutyCycleMax = 100*256;

/* Set the duty cycle. */
/* Never let duty cycle go near zero else it will not recover. Set to a value
that will allow it to grow again if needed (round-off error problem). */
            if (dutyCycle < MIN_DUTYCYCLE) dutyCycle = MIN_DUTYCYCLE;
            if (dutyCycle > dutyCycleMax) dutyCycle = dutyCycleMax;
            pwmSetDutyCycle(dutyCycle);

/* Compute an ad-hoc charging phase variable.
This is used for comparison between batteries' charging needs. */

/* Bulk phase: relate to 70% charged at absorption voltage from 0% at an
arbitrary low voltage (about 10.5V). */
/*            if (batteryChargingPhase[index] == bulkC)
            {
                uint32_t vLimit = voltageLimit(getAbsorptionVoltage(index));
                chargingMeasure[index] = (70*(voltageAv[index] - 2700))/
                                            (vLimit - 2700);
                bulkCurrent = -currentAv[index];
            }*/
/* Absorption phase: relate to 70% at bulk current limit to 100% at float stage.
The bulk current has been recorded from the time that bulk phase ended. */
/*            else if (batteryChargingPhase[index] == absorptionC)
            {
                chargingMeasure[index] = 70 + (30*(bulkCurrent + currentAv[index]))/
                                       (bulkCurrent - getFloatStageCurrent(index));
            }*/
        }
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Initialise Global Variables

Set the charger default parameters.
*/

static void initGlobals(void)
{
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        batteryChargingPhase[i] = bulkC;
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Correct the Voltage Limit for Temperature

Based on an heuristic measure from battery data.
*/

static int16_t voltageLimit(uint16_t limitV)
{
    int32_t voltageOffset = (1984*(6835-getTemperature())) >> 16;
    return limitV + voltageOffset;
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Battery charging phase Variable

@param[in] battery: 0..NUM_BATS-1
*/

battery_Ch_States getBatteryChargingPhase(int battery)
{
    return batteryChargingPhase[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Battery charging phase Variable

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getChargingMeasure(int battery)
{
    return chargingMeasure[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Charging Voltage Control

Adapt the PWM Duty Cycle to bring the voltage to its limit point.
The limit voltage is adjusted according to temperature.

@param[in] int16_t voltage: The measured voltage.
@param[in] int16_t vLimit: The limit voltage target.
@param[in] int16_t dutyCycle: The current duty cycle.
@returns int16_t The adjusted duty cycle.
*/

static void adaptDutyCycle(int16_t voltage, int16_t vLimit, uint16_t* dutyCycle)
{
    uint32_t newDutyCycle = *dutyCycle;
    int16_t vLimitAdjusted = voltageLimit(vLimit);
    int16_t voltageDiff = (voltage - vLimitAdjusted);
    if (voltageDiff > 0)
    {
/* Speed up the return to the voltage limit as the difference is greater */
/*        newDutyCycle = (newDutyCycle*((vLimitAdjusted-11*256)*115)/(voltage-11*256))>>7;*/
        newDutyCycle = (newDutyCycle*115)>>7;
    }
    else
    {
/* Increase by about 9% */
        newDutyCycle = (newDutyCycle*140)>>7;
    }
    *dutyCycle = newDutyCycle;
}

