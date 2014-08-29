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

/* Local Prototypes */
static void initGlobals(void);
static int16_t voltageLimit(uint16_t limitV);
static void adaptDutyCycle(int16_t voltage, int16_t vLimit, uint16_t* dutyCycle);

/* Local Persistent Variables */
static battery_Ch_States batteryChargingPhase[NUM_BATS];
static int16_t chargingMeasure[NUM_BATS];       /* ad-hoc state measure */
static uint8_t chargerWatchdogCount;
static uint16_t absorptionPhaseTime[NUM_BATS];   /* time in absorption */
static int16_t absorptionPhaseCurrent[NUM_BATS];
static int16_t voltageAv[NUM_BATS];
static int16_t currentAv[NUM_BATS];
static uint8_t batteryUnderCharge = 0;  /* zero if no battery charging */
static uint16_t bulkCurrent = 0;

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
            {
                batteryChargingPhase[i] = bulkC;
            }
/* Also if battery is in absorption phase and is not being charged. */
            if ((batteryChargingPhase[i] == absorptionC) &&
                (i+1 != batteryUnderCharge))
            {
                batteryChargingPhase[i] = bulkC;
            }
        }

/* Compute the averaged voltages and currents to manage phase switchover.
Use first order exponential filters, separate coefficients. */
        for (i=0; i<NUM_BATS; i++)
        {
            int16_t current = getBatteryCurrent(i)-getBatteryCurrentOffset(i);
            int16_t voltage = getBatteryVoltage(i);
/* Seed the filter with the most recent measurement (rather than zero) */
            if (voltageAv[i] == 0) voltageAv[i] = voltage;
            if (currentAv[i] == 0) currentAv[i] = current;
/* IIR filters with fairly short time constant */
            voltageAv[i] = voltageAv[i] +
                        ((getAlphaV()*(voltage - voltageAv[i]))>>8);
            currentAv[i] = currentAv[i] +
                        ((getAlphaC()*(current - currentAv[i]))>>8);
        }

/* If autotracking then get battery under charge from the monitor, otherwise
get it from the manual settings. */
    if (isAutoTrack()) battery = getBatteryUnderCharge();
    else battery = getManualSwitchSetting();

/* If this is different to the battery currently under charge, change over
and reset the measurements and duty cycle. Nothing should happen if no
charger is allocated. */
        if (battery != batteryUnderCharge)
        {
            batteryUnderCharge = battery;   /* Set new battery to charge */
            if (batteryUnderCharge > 0)
            {
                voltageAv[batteryUnderCharge-1] = 0;
                currentAv[batteryUnderCharge-1] = 0;
                dutyCycle = 50*256;
            }
        }

/* Set the panel switch to the selected battery if tracking is automatic
(otherwise it is set externally and should not be changed here). */
        if (isAutoTrack()) setSwitch(batteryUnderCharge,PANEL);

/* Start of three phase algorithm */
        if (batteryUnderCharge > 0)
        {

            uint8_t index = batteryUnderCharge-1;
/* Manage the change from bulk to absorption phase. */
            if (voltageAv[index] > voltageLimit(getAbsorptionVoltage(index)))
                batteryChargingPhase[index] = absorptionC;  /* Change. */

/* Check that the battery doesn't remain too long in the absorption phase while
the current is not falling (within a 5% error). 0.5 hour max. Then go to float.
NOTE: in the following all measured currents are negative while charging. */
            if ((batteryChargingPhase[index] == absorptionC) &&
                ((absorptionPhaseCurrent[index]*240)/256 > currentAv[index]))
            {
                absorptionPhaseTime[index]++;
                if (absorptionPhaseTime[index] > 1800000/getChargerDelay())
                {
                    batteryChargingPhase[index] = floatC;
                    absorptionPhaseTime[index] = 0;
                    absorptionPhaseCurrent[index] = 0;
                }
            }
            else if (batteryChargingPhase[index] == bulkC)
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
            if ((batteryChargingPhase[index] == absorptionC) &&
                (-currentAv[index] < getFloatStageCurrent(index)))
            {
                batteryChargingPhase[index] = floatC;
                resetBatterySoC(batteryUnderCharge-1);
            }

/* Manage the change to bulk phase when the terminal voltage drops below the
absorption threshold, and the duty cycle reaches 100%. This can happen when the
charger voltage drops, as in a solar panel application. */
            if (batteryChargingPhase[index] == absorptionC)
            {
                if ((voltageAv[index] < voltageLimit(getAbsorptionVoltage(index))*230/256) &&
                    (dutyCycle == dutyCycleMax))
                {
                    batteryChargingPhase[index] = bulkC;
                }
            }

/* Manage the float phase voltage limit. */
            if (batteryChargingPhase[index] == floatC)
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
            int32_t currentPeak = -((int32_t)current*100)/dutyCycle;
            if (currentPeak > getBulkCurrentLimit(index))
                dutyCycleMax = getBulkCurrentLimit(index)*256/currentPeak;
            else dutyCycleMax = 100*256;

/* Set the duty cycle. */
/* Never let duty cycle go too near zero else it will not recover. Set to a
value that will allow it to grow again if needed (round-off error problem). */
            if (dutyCycle < MIN_DUTYCYCLE) dutyCycle = MIN_DUTYCYCLE;
            if (dutyCycle > dutyCycleMax) dutyCycle = dutyCycleMax;
/* dutyCycle is a persistent variable. dutyCycleActual carries local changes */
            uint16_t dutyCycleActual = dutyCycle;
/* If the voltage drifts too high in float phase, turn off charging altogether.*/
            if ((batteryChargingPhase[index] == floatC) &&
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

