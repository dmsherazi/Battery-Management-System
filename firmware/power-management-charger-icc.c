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
    static int16_t voltageAv[NUM_BATS];
    static int16_t currentAv[NUM_BATS];
    static uint8_t batteryUnderCharge = 0;  /* zero if no battery charging */
/* Varables for the ICC algorithm */
    static uint8_t batteryNextIndex = 1;    /* Next battery for spare slot */
    static uint8_t slotBattery = 1;         /* Slot available for battery */
    static uint8_t slotTime = 0;            /* Slot time counter */

    initGlobals();

    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        voltageAv[i] = 0;
        currentAv[i] = 0;
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
                batteryChargingPhase[i] = bulkC;
        }

/* Compute the averaged voltages and currents to manage phase changeover.
Use first order exponential filters, separate coefficients. */
        int16_t current = getBatteryCurrent(i)-getBatteryCurrentOffset(i);
        int16_t voltage = getBatteryVoltage(i);
        for (i=0; i<NUM_BATS; i++)
        {
/* Seed the filter with the most recent measurement (rather than zero) */
            if (voltageAv[i] == 0) voltageAv[i] = voltage;
            if (currentAv[i] == 0) currentAv[i] = current;
/* IIR filters with fairly short time constant */
            voltageAv[i] = voltageAv[i] +
                        ((getAlphaV()*(voltage - voltageAv[i]))>>8);
            currentAv[i] = currentAv[i] +
                        ((getAlphaC()*(current - currentAv[i]))>>8);
        }

/*************** Implement the ICC algorithm ******************/
/* Allocate the battery under charge to the battery owning the slot,
unless it happens to be enforced idle (float or rest), in which case
yield it up to the next battery under bulk charge in turn. */ 
        slotTime++;
        if (slotTime > SLOT_PERIOD/getChargerDelay())
        {   
            slotTime = 0;
            slotBattery++;
            if (slotBattery > NUM_SLOTS) slotBattery = 1;
/* Manage change from bulk to rest phase */
            if ((batteryChargingPhase[slotBattery] == bulkC) &&
                (voltageAv[slotBattery] > voltageLimit(getAbsorptionVoltage(slotBattery))))
                batteryChargingPhase[slotBattery] = restC;
/* Manage change from rest to absorption (pulsed charging) phase */
            if ((batteryChargingPhase[slotBattery] == restC) &&
                (computeSoC(voltageAv[slotBattery],getTemperature(),
                               getBatteryType(slotBattery)) < 97*256))
                batteryChargingPhase[slotBattery] = absorptionC;
/* Manage change from absorption to float phase. */
            if ((batteryChargingPhase[slotBattery] == absorptionC) &&
                (voltageAv[slotBattery] > voltageLimit(getAbsorptionVoltage(slotBattery))))
                batteryChargingPhase[slotBattery] = floatC;
/* Check if slot is needed by the battery */
            if ((batteryChargingPhase[slotBattery] == floatC) ||
                (batteryChargingPhase[slotBattery] == restC) ||
                (slotBattery > NUM_SLOTS))
            {
                uint8_t i = 0;
                for (i=0; i < NUM_BATS; i++)
                {
                    uint8_t j = i + batteryNextIndex + 1;
                    if (j > NUM_BATS) j = 1;
                    if (batteryChargingPhase[j] == bulkC)
                    {
                        batteryUnderCharge = batteryNextIndex;
                        batteryNextIndex = j;
                        break;
                    }
                }
            }
            else batteryUnderCharge = slotBattery;
        }

        uint8_t index = batteryUnderCharge-1;
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
/* Never let duty cycle go too near zero else it will not recover. Set to a
value that will allow it to grow again if needed (round-off error problem). */
        if (dutyCycle < MIN_DUTYCYCLE) dutyCycle = MIN_DUTYCYCLE;
        if (dutyCycle > dutyCycleMax) dutyCycle = dutyCycleMax;
        uint16_t dutyCycleActual = dutyCycle;
/* If the voltage drifts too high in float phase, turn off charging altogether.*/
        if ((batteryChargingPhase[index] == floatC) &&
            (voltageAv[index] > voltageLimit(getFloatVoltage(index))*260/256))
            dutyCycleActual = 0;
/* If the voltage drifts above absorption voltage in any phase, turn off
charging altogether.*/
        if (voltageAv[index] > voltageLimit(getAbsorptionVoltage(index))*260/256)
            dutyCycleActual = 0;
/* If the voltage drifts above the maximum in any phase, turn off charging
altogether as the duty cycle is not handling this case.*/
        if (voltageAv[index] > VOLTAGE_MAX*253/256)
            dutyCycleActual = 0;
        pwmSetDutyCycle(dutyCycleActual);
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

