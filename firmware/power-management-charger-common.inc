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

/* Local Prototypes */
static void initGlobals(void);
static int16_t voltageLimit(uint16_t limitV);
static void adaptDutyCycle(int16_t voltage, int16_t vLimit, uint16_t* dutyCycle);
static void calculateAverageMeasures(void);
/* These must be defined in the including module */
static void chargerControl(uint8_t battery);
static void initLocals(void);

/* Local Persistent Variables */
static int16_t voltageAv[NUM_BATS];
static int16_t currentAv[NUM_BATS];
static battery_Ch_States batteryChargingPhase[NUM_BATS];
static uint8_t chargerWatchdogCount;
static uint8_t batteryUnderCharge;

/*--------------------------------------------------------------------------*/
/* @brief Charging Task

This task runs over long times, implementing the charging algorithm on a
battery or batteries that have been designated for charging.
*/

void prvChargerTask(void *pvParameters)
{

    initGlobals();
    initLocals();

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
/* Compute the average current and voltage */
        calculateAverageMeasures();

        uint8_t battery;                /* Preferred Battery */
/* if autotracking then get battery under charge from the monitor, otherwise
get it from the manual panel switch settings. */
        if (isAutoTrack()) battery = getBatteryUnderCharge();
        else battery = getManualSwitchSetting();


        chargerControl(battery);

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
        voltageAv[i] = 0;
        currentAv[i] = 0;
    }
    chargerWatchdogCount = 0;
}

/*--------------------------------------------------------------------------*/
/** @brief Compute Averaged Voltages and Currents

Compute averaged voltages and currents to manage phase switchover.
Use first order exponential filters, separate coefficients.
Do this for all batteries. Missing ones will be ignored anyway.
*/

static void calculateAverageMeasures(void)
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
/** @brief Access the Battery Charging Phase Variable

@param[in] battery: 0..NUM_BATS-1
*/

battery_Ch_States getBatteryChargingPhase(int battery)
{
    return batteryChargingPhase[battery];
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


