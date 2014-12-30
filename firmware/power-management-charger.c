/* STM32F1 Power Management for Solar Power

Battery Charging Task Pulse Algorithm

This task implements an algorithm to manage charging of a single battery.
Battery current and terminal voltage are monitored constantly.

The monitor task designates a battery to be placed under charge. This will
normally be a battery with the lowest SoC, but the monitor attempts to maintain
the battery under charge until the state is changed by the charging algorithm.

The charging task uses a "Pulse Charge" algorithm. This algorithm stops
charging the battery when the gassing voltage is reached, and allows it to rest
for a minimum period of time. The monitor task will then find another battery
in bulk charge phase that can be placed on charge. The batteries alternate
between the charge and rest phases while respecting the minimum rest time.
This is a variant of the Intermittent Charge and Interrupted Charge Control
algorithms reported in the literature. The aim is to keep the battery from
dwelling at the gassing voltage while still maintaining efficiency and
reaching full charge for all the batteries.

To bring the batteries to full charge and to improve efficiency, periods when
all other batteries are in rest phase will be given to the current battery to
maintain charging at the gassing voltage, as in the absorption phase of the
common three-phase algorithm. This will be maintained until another battery
changes to bulk phase at the expiry of its rest time. The process continues
until the average current during this absorption phase drops to the float limit
or continues for an extended period. At this point the battery is considered as
charged and only side reactions are occurring.

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

/* Local Prototypes */
static void initGlobals(void);
static void adaptDutyCycle(int16_t voltage, int16_t vLimit, uint16_t* dutyCycle);
static void calculateAverageMeasures(uint8_t index);
static int16_t voltageLimit(uint16_t limitV);

/* Local Persistent Variables */
static battery_Ch_States batteryChargingPhase[NUM_BATS];
static uint8_t chargerWatchdogCount;
static int16_t voltageAv[NUM_BATS];
static int16_t currentAv[NUM_BATS];

static uint32_t restPhaseTime[NUM_BATS];  /* Battery time in rest */
static uint32_t onTime[NUM_BATS];   /* Battery time in charge */
static uint16_t dutyCycle[NUM_BATS];
static uint16_t dutyCycleMax;
static uint32_t absorptionPhaseTime[NUM_BATS];   /* time in absorption */
static int16_t absorptionPhaseCurrent[NUM_BATS];

/*--------------------------------------------------------------------------*/
/* @brief Charging Task

This task runs over long times, implementing the charging algorithm on a
battery or batteries that have been designated for charging.
*/

void prvChargerTask(void *pvParameters)
{

    initGlobals();

    uint32_t minimumRestTime = (uint32_t)(REST_TIME*1024)/getChargerDelay();
    uint32_t floatDelay = (uint32_t)(FLOAT_DELAY*1024)/getChargerDelay();

    while (1)
    {
/* Wait until the next tick cycle */
        vTaskDelay(getChargerDelay());
/* Reset watchdog counter */
        chargerWatchdogCount = 0;

/* Compute the average current and voltage */
        uint8_t i;
        for (i=0; i<NUM_BATS; i++)
        {
            calculateAverageMeasures(i);
        }

        for (i=0; i < NUM_BATS; i++)
        {
/* If a battery is in a rest phase, increment its off-time counter,
or if in bulk phase, its on-time counter. */
            if (getBatteryChargingPhase(i) == restC) restPhaseTime[i]++;
            else if (getBatteryChargingPhase(i) == bulkC) onTime[i]++;

/* Change to bulk phase if a battery is in rest phase and its off time exceeds
the minimum. Charging will not start until the charger is allocated. */
            if ((getBatteryChargingPhase(i) == restC) &&
                (restPhaseTime[i] > minimumRestTime))
            {
                setBatteryChargingPhase(i,bulkC);
            }
        }

/* Re-initialize the algorithm if the panel voltage has fallen below 10V, which
would almost certainly indicate night time. This will prevent the algorithm from
carrying over unfinished charging states from the previous day. */
        if (getPanelVoltage(0) < 10*256) resetChargeAlgorithm();

/* Get battery under charge from the panel switch settings. This is necessary
because the battery may have been set manually rather than via autotracking
in the monitor task. */
        uint8_t battery = getPanelSwitchSetting();

/***** PULSE Algorithm */
/* battery set to zero means that the charger is unallocated or is not producing
any output. The former should only happen when the charger is manually disabled.
The autotracker should normally always allocate a battery under charge. */
        if (battery > 0)
        {

/* BULK Phase Management */
            uint8_t index = battery-1;
            if (getBatteryChargingPhase(index) == bulkC)
            {
/* Set duty cycle full on for bulk charging */
                dutyCycle[index] = 100*256;

/* Keep a record of the current so that it is available when first entering
absorption phase, to test if the current is falling. */
                absorptionPhaseTime[index] = 0;
                absorptionPhaseCurrent[index] = getCurrentAv(index);

/* Manage the change from bulk phase at the gassing limit. */
                if (getVoltageAv(index) > voltageLimit(getAbsorptionVoltage(index)))
                {
/* If no other battery is in bulk phase, change this battery to absorption
phase, otherwise change to rest phase. */
                    setBatteryChargingPhase(index,absorptionC);
                    for (i=0; i<NUM_BATS; i++)
                    {
                        if (getBatteryChargingPhase(i) == bulkC)
                        {
                            setBatteryChargingPhase(index,restC);
                            break;
                        }
                    }
/* On entering absorption phase, drop duty cycle by 50% to reduce overshoot
problems. */
                    if (getBatteryChargingPhase(index) == absorptionC)
                        dutyCycle[index] /= 2;
/* Now that the cycle is finished, reset times to start next cycle. */
                    restPhaseTime[index] = 0;
                    onTime[index] = 0;
                }
            }

/* ABSORPTION Phase Management */
/* If the battery is in the absorption phase, manage the constant voltage
charging */
            else if (getBatteryChargingPhase(index) == absorptionC)
            {
                restPhaseTime[index] = 0;
                onTime[index] = 0;
/* At the end of the absorption period, if another battery has changed to bulk
phase, pass to rest phase to allow some other battery to have its turn. This
will take effect on the next cycle of the monitor task. */
                if (absorptionPhaseTime[index] > ABSORPTION_TIME)
                {
                    for (i=0; i<NUM_BATS; i++)
                    {
                        if (getBatteryChargingPhase(i) == bulkC)
                        {
                            setBatteryChargingPhase(index,restC);
                            absorptionPhaseTime[index] = 0;
                            break;
                        }
                    }
                }
/* Check if current is above the last recorded value slightly reduced, to detect
if the current has stopped falling. Change to float phase if time exceeded. */
                if ((240*absorptionPhaseCurrent[index])/256 > getCurrentAv(index))
                {
                    absorptionPhaseTime[index]++;
                    if (absorptionPhaseTime[index] > floatDelay)
                    {
                        setBatteryChargingPhase(index,floatC);
                        absorptionPhaseTime[index] = 0;
                        absorptionPhaseCurrent[index] = 0;
                    }
                }
/* If current is dropping, reset the last recorded current */
                else
                    absorptionPhaseCurrent[index] = 0;

/* Manage the change to float phase when the current drops below the float
threshold. This is done on the averaged current as rapid response is not
essential. (Note: measured currents are negative while charging). Ignore samples
where the source voltage drops causing the battery voltage and current to fall.
When the change occurs, force the SoC to 100%. This may not be correct if
the battery is faulty with a low terminal voltage, but that case is handled
by the resetBattery function. */
                if ((-getCurrentAv(index) < getFloatStageCurrent(index)) &&
                    (getVoltageAv(index) > (245*voltageLimit(getAbsorptionVoltage(index)))/256))
                {
                    setBatteryChargingPhase(index,floatC);
                    absorptionPhaseTime[index] = 0;
                    resetBatterySoC(battery-1);
                }


/* Manage the absorption phase voltage limit. */
                adaptDutyCycle(getVoltageAv(index),getAbsorptionVoltage(index),
                               &dutyCycle[index]);
            }

/* Overcurrent protection:
Compute the peak current from duty cycle (assumes current goes from 0 to a peak)
then if the peak is greater than the battery's current limit, reduce the
maximum duty cycle. Limit the duty cycle to this.
This is done on the directly measured current for rapid response. */
            int16_t current = getBatteryCurrent(index)-getBatteryCurrentOffset(index);
            int32_t currentPeak = -((int32_t)current*100)/dutyCycle[index];
            if (currentPeak > getBulkCurrentLimit(index))
                dutyCycleMax = getBulkCurrentLimit(index)*256/currentPeak;
            else dutyCycleMax = 100*256;
            if (dutyCycle[index] > dutyCycleMax) dutyCycle[index] = dutyCycleMax;

/* Set the duty cycle. */
/* Never let duty cycle go too near zero else it will not recover. Set to a
value that will allow it to grow again if needed (round-off error problem). */
            if (dutyCycle[index] < MIN_DUTYCYCLE) dutyCycle[index] = MIN_DUTYCYCLE;

            uint16_t dutyCycleActual = dutyCycle[index];

/* If the panel voltage is too low, turn off charging. */
            if (getPanelVoltage(0) < 10*256)
                dutyCycleActual = 0;

/* If in rest or float phase, turn off charging as a precaution. */
            if ((getBatteryChargingPhase(index) == restC) ||
                (getBatteryChargingPhase(index) == floatC))
                dutyCycleActual = 0;

/* Set the actual duty cycle. */
            pwmSetDutyCycle(dutyCycleActual);
        }
    }

}

/*--------------------------------------------------------------------------*/
/** @brief Initialise Global Variables

Set the charger default parameters.
*/

void initGlobals(void)
{
    chargerWatchdogCount = 0;
    uint8_t i;
    for (i=0; i<NUM_BATS; i++)
    {
        voltageAv[i] = 0;
        currentAv[i] = 0;
        absorptionPhaseTime[i] = 0;
        absorptionPhaseCurrent[i] = 0;
        dutyCycle[i] = 100*256;              /* percentage times 256 */
    }
    resetChargeAlgorithm();
    dutyCycleMax = 100*256;
}

/*--------------------------------------------------------------------------*/
/** @brief Reset Battery Parameters when Algorithm changed

This should be called only when the algorithm is changed to a different one.
The charger parameters are reset and the battery charge state is modified to
suit the algorithm.

param[in] chargeAlgorithm. The new charge algorithm.
*/

void resetChargeAlgorithm()
{
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        restPhaseTime[i] = 0;
        onTime[i] = 0;
/* Set battery state to rest if left in absorption pahse. */
        if (getBatteryChargingPhase(i) == absorptionC)
            setBatteryChargingPhase(i,restC);
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Averaged Battery Terminal Voltage

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getVoltageAv(int index)
{
    return voltageAv[index];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Averaged Battery Terminal Current

@param[in] index: 0..NUM_BATS-1
*/

int16_t getCurrentAv(int index)
{
    return currentAv[index];
}

/*--------------------------------------------------------------------------*/
/** @brief Return the Battery Charging Phase Variable

@param[in] index: 0..NUM_BATS-1
@result battery_Ch_States chargePhase
*/

battery_Ch_States getBatteryChargingPhase(int index)
{
    return batteryChargingPhase[index];
}

/*--------------------------------------------------------------------------*/
/** @brief Set the Battery Charging Phase Variable

@param[in] index: 0..NUM_BATS-1
@param[in] battery_Ch_States chargePhase
*/

void setBatteryChargingPhase(int index, battery_Ch_States chargePhase)
{
    batteryChargingPhase[index] = chargePhase;
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
    if (voltage > vLimitAdjusted)
    {
/* If the voltage drifts too far above absorption voltage, drop the duty cycle
by about 30%. This can happen if the battery is fully charged and absorption
phase is entered. The dutycycle may not otherwise recover quickly enough. */
        if (voltage > (268*vLimitAdjusted)/256)
            newDutyCycle = (newDutyCycle*90)>>7;
/* Otherwise reduce by about 10% */
        else newDutyCycle = (newDutyCycle*115)>>7;
    }
    else
    {
/* Increase by about 9% */
        newDutyCycle = (newDutyCycle*140)>>7;
    }
    *dutyCycle = newDutyCycle;
}

/*--------------------------------------------------------------------------*/
/** @brief Compute Averaged Voltage and Current

Compute averaged voltage and current for a battery to manage phase switchover.
Use first order exponential filters, separate coefficients. Missing batteries
will be ignored.

@param[in] uint8_t i: index to battery table.
*/

void calculateAverageMeasures(uint8_t i)
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

