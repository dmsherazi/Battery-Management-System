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
#include "power-management-charger-3ph.h"
#include "power-management-charger-pulse.h"
#include "power-management-objdic.h"

/* Local Prototypes */
static void initGlobals(void);

/* Local Persistent Variables */
static battery_Ch_States batteryChargingPhase[NUM_BATS];
static uint8_t chargerWatchdogCount;
static int16_t voltageAv[NUM_BATS];
static int16_t currentAv[NUM_BATS];

/*--------------------------------------------------------------------------*/
/* @brief Charging Task

This task runs over long times, implementing the charging algorithm on a
battery or batteries that have been designated for charging.
*/

void prvChargerTask(void *pvParameters)
{

    initGlobals();
    initLocals3PH();
    initLocalsPulse();

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

        if (getChargeAlgorithm() == threePH)
            chargerControl3PH(battery);
        else if (getChargeAlgorithm() == pulse)
            chargerControlPulse(battery);

    }
}

/*--------------------------------------------------------------------------*/
/** @brief Initialise Global Variables

Set the charger default parameters.
*/

static void initGlobals(void)
{
    chargerWatchdogCount = 0;
    uint8_t i;
    for (i=0; i<NUM_BATS; i++)
    {
        voltageAv[i] = 0;
        currentAv[i] = 0;
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Reset Battery Parameters when Algorithm changed

This should be called only when the algorithm is changed to a different one.
The charger parameters are reset and the battery charge state is modified to
suit the algorithm.

The current charge algorithm in use is not checked, so this could be used to
reset the algorithm state if necessary.

param[in] chargeAlgorithm. The new charge algorithm.
*/

void resetChargeAlgorithm(charge_algorithm chargeAlgorithm)
{
    if (chargeAlgorithm == threePH)
    {
        initLocals3PH();
    }
    else if (chargeAlgorithm == ic)
    {
        initLocalsPulse();
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Averaged Battery Terminal Voltage

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getVoltageAv(int battery)
{
    return voltageAv[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Averaged Battery Terminal Current

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getCurrentAv(int battery)
{
    return currentAv[battery];
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

/*--------------------------------------------------------------------------*/
/** @brief Correct the Voltage Limit for Temperature

Based on an heuristic measure from battery data.
*/

int16_t voltageLimit(uint16_t limitV)
{
    int32_t voltageOffset = (1984*(6835-getTemperature())) >> 16;
    return limitV + voltageOffset;
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

void adaptDutyCycle(int16_t voltage, int16_t vLimit, uint16_t* dutyCycle)
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
/** @brief Compute Averaged Voltages and Currents

Compute averaged voltages and currents to manage phase switchover.
Use first order exponential filters, separate coefficients.
Do this for all batteries. Missing ones will be ignored anyway.
*/

void calculateAverageMeasures(void)
{
    uint8_t i;
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
}


