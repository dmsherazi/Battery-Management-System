/* STM32F1 Power Management for Solar Power

Monitoring Task

This task accesses the various measured and estimated parameters of the
batteries, loads and panel to make decisions about switch settings and
load disconnect/reconnect.

The decisions determine how to connect loads and solar panel to the different
batteries in order to ensure continuous service and long battery life. The
batteries are connected to the charger at a low level of SoC, to the loads at a
high level of SoC, and are isolated for a period of time to obtain a reference
measurement of the SoC from the open circuit voltage. Loads are progressively
disconnected as the batteries pass to the low and critically low charge states.

On external command the interface currents and SoC of the batteries will be
calibrated.

On external command the task will automatically track and manage battery to load
and battery charging. Tracking will always occur but switches will not be set
until auto-tracking is enabled.

Initial 29 September 2013
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
#include <stdlib.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "power-management-board-defs.h"
#include "power-management.h"
#include "power-management-hardware.h"
#include "power-management-objdic.h"
#include "power-management-lib.h"
#include "power-management-time.h"
#include "power-management-file.h"
#include "power-management-comms.h"
#include "power-management-measurement.h"
#include "power-management-charger.h"
#include "power-management-monitor.h"

/* Local Prototypes */
static void initGlobals(void);
static int16_t computeSoC(uint32_t voltage, uint32_t temperature, battery_Type type);

/* Local Persistent Variables */
static uint8_t monitorWatchdogCount;
static bool calibrate;
/* All current, voltage, SoC, charge variables times 256. */
static uint16_t batteryCurrentSteady[NUM_BATS];
static battery_Fl_States batteryFillState[NUM_BATS];
static battery_Op_States batteryOpState[NUM_BATS];
static battery_Hl_States batteryHealthState[NUM_BATS];
static union InterfaceGroup currentOffsets;
static union InterfaceGroup currents;
static union InterfaceGroup voltages;
static int16_t lastbatteryCurrent[NUM_BATS];
static int16_t lastbatteryVoltage[NUM_BATS];
static uint8_t batteryUnderCharge;
static uint8_t batteryUnderLoad;
/* State of Charge is in percentage (times 256) */
static uint16_t batterySoC[NUM_BATS];
/* Battery charge is in Coulombs (times 256) */
static uint32_t batteryCharge[NUM_BATS];
/* Time that a battery is in the isolation state */
static uint32_t batteryIsolationTime[NUM_BATS];

/*--------------------------------------------------------------------------*/
/* @brief Monitoring Task

This task runs over long times, monitoring the state of the batteries,
deciding which oneto charge and which to load, switching them out at intervals
and when critically low, and resetting the state of charge estimation
algorithm during lightly loaded periods.
*/

void prvMonitorTask(void *pvParameters)
{
    initGlobals();

    uint16_t decisionStatus = 0;    

/* Short delay to allow measurement task to produce results */
	vTaskDelay(MONITOR_STARTUP_DELAY );

/* Determine SoC and capacity to start. */
    uint8_t i;
    for (i=0; i<NUM_BATS; i++)
    {
        batterySoC[i] = computeSoC(getBatteryVoltage(i),getTemperature(),
                                   getBatteryType(i));
        batteryCharge[i] = getBatteryCapacity(i)*36*batterySoC[i];
    }

/* Main loop */
	while (1)
	{
/*------------- CALIBRATION -----------------------*/
/* Perform a calibration sequence to zero the currents (see docs).
Also the State of Charge is estimated from the Open Circuit Voltages, so the
system should have been left in a quiescent state for at least an hour. */

        if (calibrate)
        {
/* Keep aside to restore after calibration. */
            uint8_t switchSettings = getSwitchControlBits();
            int16_t results[NUM_TESTS][NUM_IFS];    /* result for all 7 tests */
            uint8_t test;
            uint8_t i;

/* Zero the offsets. */
            for (i=0; i<NUM_IFS; i++) currentOffsets.data[i] = 0;

/* Set switches and collect the results */
            for (test=0; test<NUM_TESTS ; test++)
            {
/* First turn off all switches */
                for (i=0; i<NUM_LOADS+NUM_PANELS; i++) setSwitch(0,i);
/* Connect load 2 to each battery in turn. */
                if (test < NUM_BATS) setSwitch(test+1,1);
/* Then connect load 1 to each battery in turn. Last test is all
switches off to allow the panel to be measured. */
                else if (test < NUM_TESTS-1) setSwitch(test-NUM_BATS+1,0);
/* Delay a few seconds to let the measurements settle. Current should settle
quickly but terminal voltage may take some time, which could slightly affect
some currents. */
                vTaskDelay(getCalibrationDelay());
/* Check to see if a battery is missing as a result of setting loads. */
                for (i=0; i<NUM_BATS; i++)
                {
                    if (((getIndicators() >> 2*i) & 0x02) == 0)
                    {
                        batteryHealthState[i] = missingH;
                        batterySoC[i] = 0;
                    }
                }
/* Reset watchdog counter */
                monitorWatchdogCount = 0;
                for (i=0; i<NUM_IFS; i++) results[test][i] = getCurrent(i);
/* Send a progress update */
                dataMessageSend("pQ",0,test);
            }

/* Estimate the offsets only when they are less than a threshold. Find the lowest
value for each interface. */
            for (i=0; i<NUM_IFS; i++)
            {
                currentOffsets.data[i] = OFFSET_START_VALUE;
/* Run through each test to find the minimum. This is the offset. */
                for (test=0; test<NUM_TESTS; test++)
                {
                    int16_t current = results[test][i];
/* Get the minimum if within the threshold. */
                    if (current > CALIBRATION_THRESHOLD)
                    {
                        if (current < currentOffsets.data[i])
                            currentOffsets.data[i] = current;
                    }
                }
/* If not changed, then the measurements were invalid, so set to zero. */
                if (currentOffsets.data[i] == OFFSET_START_VALUE)
                    currentOffsets.data[i] = 0;
/* Remove offset from the results */
                for (test=0; test<NUM_TESTS; test++)
                {
                    results[test][i] -= currentOffsets.data[i];
                }        
            }
/* Run through all tests and batteries to find the maximum. This is the
quiescent current */
            int16_t quiescentCurrent = -100;
            for (i=0; i<NUM_BATS; i++)
            {
                if (batteryHealthState[i] != missingH)
                {
                    for (test=0; test<NUM_TESTS; test++)
                    {
                        int16_t current = results[test][i];
/* Get the maximum if within threshold. */
                        if (current > CALIBRATION_THRESHOLD)
                        {
                            if (current > quiescentCurrent)
                                quiescentCurrent = current;
                        }
                    }
                }
            }
            dataMessageSend("pQ",quiescentCurrent,7);

/* Restore switches and report back */
            setSwitchControlBits(switchSettings);
            dataMessageSend("dS",switchSettings,0);
/* Compute the SoC from the OCV. Note the conditions for which OCV gives an
accurate estimate of SoC. */
/* Zero counters, and reset battery states */
            for (i=0; i<NUM_BATS; i++)
            {
                if (batteryHealthState[i] != missingH)
                {
                    batterySoC[i] = computeSoC(getBatteryVoltage(i),
                                               getTemperature(),getBatteryType(i));
                    batteryCharge[i] = getBatteryCapacity(i)*36*batterySoC[i];
                    batteryCurrentSteady[i] = 0;
                    batteryIsolationTime[i] = 0;
                    batteryOpState[i] = isolatedO;
                }
            }
            batteryUnderLoad = 0;
            batteryUnderCharge = 0;
/* Write the offsets to FLASH */
            for (i=0; i<NUM_IFS; i++) setCurrentOffset(i,currentOffsets.data[i]);
            writeConfigBlock();
/* Ensure that calibration doesn't happen on the next cycle */
            calibrate = false;
        }

/*------------- RECORD AND REPORT STATE --------------*/
/* Send off the current set of measurements */
        char id[4];
        id[0] = 'd';
        id[3] = 0;
/* Send out a time string */
        char timeString[20];
        putTimeToString(timeString);
        sendString("pH",timeString);
        recordString("pH",timeString);
        for (i=0; i<NUM_BATS; i++)
        {
            id[2] = '1'+i;
/* Send out battery terminal measurements. */
            id[1] = 'B';
            dataMessageSend(id,
                        getBatteryCurrent(i)-currentOffsets.data[i],
                        getBatteryVoltage(i));
            recordDual(id,
                        getBatteryCurrent(i)-currentOffsets.data[i],
                        getBatteryVoltage(i));
/* Send out battery state of charge. */
            id[1] = 'C';
            sendResponse(id,batterySoC[i]);
            recordSingle(id,batterySoC[i]);
/* Send out battery operational, fill, charging and health status indication. */
            uint16_t states = (batteryOpState[i] & 0x03) |
                             ((batteryFillState[i] & 0x03) << 2) |
                             ((getBatteryChargingPhase(i) & 0x03) << 4) |
                             ((batteryHealthState[i] & 0x03) << 6);
            id[1] = 'O';
            sendResponse(id,states);
            recordSingle(id,states);
        }
/* Send out load terminal measurements. */
        id[1] = 'L';
        for (i=0; i<NUM_LOADS; i++)
        {
            id[2] = '1'+i;
            dataMessageSend(id,
                        getLoadCurrent(i)-currentOffsets.data[NUM_BATS+i],
                        getLoadVoltage(i));
            recordDual(id,
                        getLoadCurrent(i)-currentOffsets.data[NUM_BATS+i],
                        getLoadVoltage(i));
        }
/* Send out panel terminal measurements. */
        id[1] = 'M';
        for (i=0; i<NUM_PANELS; i++)
        {
            id[2] = '1'+i;
            dataMessageSend(id,
                        getPanelCurrent(i)-currentOffsets.data[NUM_BATS+NUM_LOADS+i],
                        getPanelVoltage(i));
            recordDual(id,
                        getPanelCurrent(i)-currentOffsets.data[NUM_BATS+NUM_LOADS+i],
                        getPanelVoltage(i));
        }
/* Send out temperature measurement. */
        sendResponse("dT",getTemperature());
        recordSingle("dT",getTemperature());
/* Send out control variables - isAutoTrack(), recording, calibrate */
        sendResponse("dD",getControls());
        recordSingle("dD",getControls());
        sendResponse("ds",(int)getSwitchControlBits());
        recordSingle("ds",(int)getSwitchControlBits());
/* Send switch and decision settings during tracking */
        if (isAutoTrack())
        {
            sendResponse("dd",decisionStatus);
            recordSingle("dd",decisionStatus);
        }
/* Read the interface fault indicators and send out */
        sendResponse("dI",getIndicators());

/*------------- COMPUTE BATTERY STATE -----------------------*/
/* Check to see if a battery is missing. This can happen if a battery put under
load has been removed. Missing batteries not under load will not show as
missing due to the nature of the circuitry, so existing missing status is not
removed here; this must be done externally. */
        for (i=0; i<NUM_BATS; i++)
        {
            if (((getIndicators() >> 2*i) & 0x02) == 0)
            {
                batteryHealthState[i] = missingH;
                batterySoC[i] = 0;
            }
        }
/* Find number of batteries present */
        uint8_t numBats = NUM_BATS;
        for (i=0; i<NUM_BATS; i++)
        {
            if (batteryHealthState[i] == missingH) numBats--;
        }
/* Access each battery charge accumulated since the last time, and update the SoC. */
        for (i=0; i<NUM_BATS; i++)
        {
            if (batteryHealthState[i] != missingH)
            {
                batteryCharge[i] += getBatteryAccumulatedCharge(i);
                uint32_t chargeMax = getBatteryCapacity(i)*36*100*256;
                if (batteryCharge[i] > chargeMax) batteryCharge[i] = chargeMax;
                batterySoC[i] = batteryCharge[i]/(getBatteryCapacity(i)*36);
            }
        }
/* Collect battery charge fill state estimations. */
        for (i=0; i<NUM_BATS; i++)
        {
            if (batteryHealthState[i] != missingH)
            {
                uint16_t batteryAbsVoltage = abs(getBatteryVoltage(i));
                batteryFillState[i] = normalF;
                if ((batteryAbsVoltage < LOW_VOLTAGE) || (batterySoC[i] < LOW_SOC))
                    batteryFillState[i] = lowF;
                else if ((batteryAbsVoltage < CRITICAL_VOLTAGE) ||
                         (batterySoC[i] < CRITICAL_SOC))
                    batteryFillState[i] = criticalF;
            }
        }
/* Rank the batteries by charge state. Bubble sort to have highest SoC first
pushing all missing batteries to the low end (SoC = 0 set above). */
        uint8_t k;
        uint16_t temp;
        uint8_t batteryFillStateSort[NUM_BATS];        
        for (i=0; i<NUM_BATS; i++) batteryFillStateSort[i] = i+1;
        for (i=0; i<NUM_BATS-1; i++)
        {
            for (k=0; k<NUM_BATS-i-1; k++)
            {
              if (batterySoC[batteryFillStateSort[k]-1] <
                  batterySoC[batteryFillStateSort[k+1]-1])
              {
                temp = batteryFillStateSort[k];
                batteryFillStateSort[k] = batteryFillStateSort[k+1];
                batteryFillStateSort[k+1] = temp;
              }
            }
        }
/* Find the batteries with the longest and shortest isolation times. */
        uint8_t longestBattery = 0;
        uint8_t shortestBattery = 0;
        uint32_t longestTime = 0;
        uint32_t shortestTime = 0xFFFFFFFF;
        for (i=0; i<NUM_BATS; i++)
        {
            if (batteryHealthState[i] != missingH)
            {
                if (batteryIsolationTime[i] > longestTime)
                {
                    longestTime = batteryIsolationTime[i];
                    longestBattery = i+1;
                }
                if (batteryIsolationTime[i] < shortestTime)
                {
                    shortestTime = batteryIsolationTime[i];
                    shortestBattery = i+1;
                }
            }
        }

/*------------- BATTERY MANAGEMENT DECISIONS -----------------------*/
/* Work through the battery states to allocate loads to the highest SoC,
accounting for isolated batteries which we wish to keep isolated if possible.
The priority is to allocate all loads to a "normal" charged battery even if it
must be taken out of its isolation period. Failing that, repeat for low state
batteries to hold the priority loads. */
/* The code is for any number of batteries, but fixed for two loads and one
panel. */
/* @TODO update code for more panels (chargers) and loads. */
        uint8_t highestBattery = batteryFillStateSort[0];
        uint8_t lowestBattery = batteryFillStateSort[numBats-1];
/* decisionStatus is a variable used to record the reason for any decision */
        decisionStatus = 0;

/* One battery: just allocate load and charger to it */
        if (numBats == 1)
        {
            batteryUnderCharge = batteryFillStateSort[0];
            batteryUnderLoad = batteryFillStateSort[0];
/* If the battery is in float and higher than 95%, stop charging */
            if ((getBatteryChargingPhase(batteryUnderCharge-1) == floatC) &&
                (batterySoC[batteryUnderCharge-1] > FLOAT_CHARGE_SOC))
            {
                batteryUnderCharge = 0;
            }
            decisionStatus = 0x100;
        }
/* Two batteries: just allocate load to highest and charger to lowest. */
        else if (numBats == 2)
        {
            decisionStatus = 0x200;
/* Deallocate the charger if the battery has reached float stage */
            if (getBatteryChargingPhase(batteryUnderCharge-1) == floatC)
                batteryUnderCharge = 0;
/* (1) If the charger is unallocated, set to the lowest SoC battery, if this
battery is not in float state with SoC > 95%. If no suitable battery is found
leave the charger unallocated. */
            if (batteryUnderCharge == 0)
            {
                decisionStatus |= 0x01;
                for (i=0; i<numBats; i++)
                {
                    uint8_t battery = batteryFillStateSort[numBats-i-1];
                    if (getBatteryChargingPhase(battery-1) != floatC)
                    {
                        decisionStatus |= 0x02;
                        batteryUnderCharge = battery;
                        break;
                    }
                    if (batterySoC[battery-1] < FLOAT_CHARGE_SOC)
                    {
                        decisionStatus |= 0x04;
                        batteryUnderCharge = battery;
                        break;
                    }
                }
/* However if this has allocated the charger to the loaded battery, then
reallocate the loaded battery. This will allow the charger to swap back and
forth as the loaded battery droops and the charging battery completes charge. */
                if (batteryUnderLoad == batteryUnderCharge)
                    batteryUnderLoad = 0;
            }
/* (2) If the loads are unallocated, set to the highest SoC unallocated battery.
Avoid the battery that has been idle for the longest time, and also the battery
under charge if the strategies require it. */
            if (batteryUnderLoad == 0)
            {
                decisionStatus |= 0x10;
                for (i=0; i<numBats; i++)
                {
                    batteryUnderLoad = batteryFillStateSort[i];
                    if (!(getMonitorStrategy() & SEPARATE_LOAD))
                    {
                        decisionStatus |= 0x20;
                        break;
                    }
                    if (batteryUnderLoad != batteryUnderCharge)
                    {
                        decisionStatus |= 0x40;
                        break;
                    }
                }
            }
/* If the battery under load is on a low or critical battery, allocate to the
charging battery regardless. */
            if (batteryFillState[batteryUnderLoad-1] != normalF)
            batteryUnderLoad = batteryUnderCharge;
        }
/* More than two batteries: manage isolation and charge state. */
        else if (numBats > 2)
        {
/*--- All batteries normal fill state. Isolated battery already allocated. ---*/
            if (batteryFillState[lowestBattery-1] == normalF)
            {
/* Deallocate the charger if the battery has reached float stage */
                if (getBatteryChargingPhase(batteryUnderCharge-1) == floatC)
                    batteryUnderCharge = 0;
/* (1) If the charger is unallocated, set to the lowest SoC battery, if this
battery is not in float state with SoC > 95%. If no suitable battery is found
leave the charger unallocated. */
                decisionStatus = 0x300;
                if (batteryUnderCharge == 0)
                {
                    decisionStatus |= 0x01;
                    for (i=0; i<numBats; i++)
                    {
                        uint8_t battery = batteryFillStateSort[numBats-i-1];
                        if (getBatteryChargingPhase(battery-1) != floatC)
                        {
                            decisionStatus |= 0x02;
                            batteryUnderCharge = battery;
                            break;
                        }
                        if (batterySoC[battery-1] < FLOAT_CHARGE_SOC)
                        {
                            decisionStatus |= 0x04;
                            batteryUnderCharge = battery;
                            break;
                        }
                    }
/* However if this has allocated the charger to the loaded battery, then
reallocate the loaded battery. This will allow the charger to swap back and
forth as the loaded battery droops and the charging battery completes charge. */
                    if (batteryUnderLoad == batteryUnderCharge)
                        batteryUnderLoad = 0;
                }
/* (2) If the loads are unallocated, set to the highest SoC unallocated battery.
Avoid the battery that has been idle for the longest time, and also the battery
under charge if the strategies require it. */
                if (batteryUnderLoad == 0)
                {
                    decisionStatus |= 0x10;
                    for (i=0; i<numBats; i++)
                    {
                        batteryUnderLoad = batteryFillStateSort[i];
                        if (!((getMonitorStrategy() & PRESERVE_ISOLATION) &&
                            (batteryUnderLoad == longestBattery)))
                        {
                            if (!(getMonitorStrategy() & SEPARATE_LOAD))
                            {
                                decisionStatus |= 0x20;
                                break;
                            }
                            if (batteryUnderLoad != batteryUnderCharge)
                            {
                                decisionStatus |= 0x40;
                                break;
                            }
                        }
                    }
                }
            }
/*--- At least one battery is normal, and some others are low or critical. ---*/
            else if (batteryFillState[highestBattery-1] == normalF)
            {
                decisionStatus = 0x400;
/* (1) If the charger is unallocated or is allocated to a normal battery,
set to the lowest SoC unallocated battery regardless if it is isolated. */
                uint8_t weakestBattery = batteryFillStateSort[numBats-1];                
                if ((batteryUnderCharge == 0) || 
                    (batteryFillState[batteryUnderCharge-1] == normalF))
                {
                    decisionStatus |= 0x01;
                    batteryUnderCharge = weakestBattery;
                }
/* (2) But if the lowest battery is critical we'd better work on that
regardless. */
                if (batteryFillState[weakestBattery-1] == criticalF)
                {
                    decisionStatus |= 0x02;
                    batteryUnderCharge = weakestBattery;
                }
/* (3) If the loads are unallocated or if the battery under load is low or
critical, set to the highest SoC unallocated battery ... */
                if ((batteryUnderLoad == 0) ||
                    (batteryFillState[batteryUnderLoad-1] != normalF))
                {
                    decisionStatus |= 0x10;
                    for (i=0; i<numBats; i++)
                    {
                        batteryUnderLoad = batteryFillStateSort[i];
                        if (batteryUnderLoad != longestBattery)
                        {
                            if (!(getMonitorStrategy() & SEPARATE_LOAD)) break;
                            if (batteryUnderLoad != batteryUnderCharge) break;
                        }
                    }
/* ... however if it still ends up on a low battery then there is only one
normal battery which is also isolated, so this must be overridden (this should
not the charging battery if our logic is correct so far). */
                    if ((batteryUnderLoad == 0) ||
                        (batteryFillState[batteryUnderLoad-1] != normalF))
                    {
                        decisionStatus |= 0x20;
                        batteryUnderLoad = batteryFillStateSort[0];
                    }
                }
            }
/*--- Otherwise all batteries are low or critical. ---*/
/* Allocate charger and loads as best we can. If the battery under load is
critical we must turn off the low priority loads. */
            else
            {
                decisionStatus = 0x500;
                batteryUnderLoad = batteryFillStateSort[0];
                batteryUnderCharge = batteryFillStateSort[numBats-1];
            }
        }

/*--------------END BATTERY MANAGEMENT DECISIONS ------------------*/

/* If charging voltage is lower than the battery voltage, turn off charging
altogether. This will allow more flexibility in managing isolation during night
periods. */
        if (getBatteryVoltage(batteryUnderCharge-1) > getPanelVoltage(0))
        {
            batteryUnderCharge = 0;
        }
/* Work out changes in battery operational states. */
        for (i=0; i<NUM_BATS; i++)
        {
            uint8_t lastOpState = batteryOpState[i];
            if (batteryHealthState[i] != missingH)
            {
                batteryOpState[i] = isolatedO; /* reset operational state */
                if ((batteryUnderLoad > 0) && (i == batteryUnderLoad-1))
                {
                    batteryOpState[i] = loadedO;
                }
                if ((batteryUnderCharge > 0) && (i == batteryUnderCharge-1))
                {
                    batteryOpState[i] = chargingO;
                }
/* Restart isolation timer if not isolated or if charger and loads on same
battery (isolation is not possible due to leakage of charging current to other
batteries). Do not reset to zero so that the current isolated timer can handover
later. */
                if ((batteryOpState[i] != isolatedO) ||
                    (batteryUnderLoad == batteryUnderCharge))
                    batteryIsolationTime[i] = 10;
            }
        }
/* Set Switches. Turn off low priority loads if batteries are all critical */
        if (isAutoTrack())
        {
            setSwitch(batteryUnderLoad,LOAD_2);
            if (batteryFillState[batteryUnderLoad-1] == criticalF)
            {
                setSwitch(0,LOAD_1);
            }
            else
            {
                setSwitch(batteryUnderLoad,LOAD_1);
            }
            setSwitch(batteryUnderCharge,PANEL);
        }

/*---------------- RESET SoC AFTER IDLE TIME --------------------*/
/* Compute state of charge estimates from OCV if currents are low for an hour.
The steady current indicator is incremented on each cycle that the current is
below a threshold of about 80mA. */
        uint32_t monitorHour = 3600*1000/(portTICK_RATE_MS*getMonitorDelay());
        for (i=0; i<NUM_BATS; i++)
        {
            if (batteryHealthState[i] != missingH)
            {
                if (abs(getBatteryCurrent(i)) < 30)
                    batteryCurrentSteady[i]++;
                else
                    batteryCurrentSteady[i] = 0;
                if (batteryCurrentSteady[i] > monitorHour)
                {
                    batterySoC[i] = computeSoC(getBatteryVoltage(i),
                                               getTemperature(),getBatteryType(i));
                    batteryCurrentSteady[i] = 0;
                }
/* Update the isolation time of each battery. If a battery has been isolated for
over 8 hours, compute SoC and drop isolation time back to zero to allow other
batteries to go isolated. */
                batteryIsolationTime[i]++;
                if (batteryIsolationTime[i] > 8*monitorHour)
                {
                    batterySoC[i] = computeSoC(getBatteryVoltage(i),
                                               getTemperature(),getBatteryType(i));
                    batteryIsolationTime[i] = 0;
                }
            }
        }

/* Wait until the next tick cycle */
		vTaskDelay(getMonitorDelay());
/* Reset watchdog counter */
        monitorWatchdogCount = 0;
    }
}

/*--------------------------------------------------------------------------*/
/** @brief Initialise Global Variables to Defaults

*/

static void initGlobals(void)
{
    calibrate = false;
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
/* Determine capacity */
        batterySoC[i] = computeSoC(getBatteryVoltage(i),getTemperature(),
                                   getBatteryType(i));
        batteryCharge[i] = getBatteryCapacity(i)*36*batterySoC[i];
        batteryCurrentSteady[i] = 0;
        batteryIsolationTime[i] = 0;
/* Start with all batteries isolated */
        batteryOpState[i] = isolatedO;
        batteryHealthState[i] = goodH;
    }
    batteryUnderLoad = 0;
    batteryUnderCharge = 0;
/* These will be in FLASH, or set to zero if not */
    for (i=0; i<NUM_IFS; i++) currentOffsets.data[i] = getCurrentOffset(i);
}

/*--------------------------------------------------------------------------*/
/** @brief Compute SoC from OC Battery Terminal Voltage and Temperature

This model covers the Gel and Wet cell batteries.
Voltage is referred to the value at 48.9C so that one table can be used.
Refer to documentation for the model formula derived.

@param uint32_t voltage. Measured open circuit voltage. Volts times 256
@param uint32_t temperature. Temperature degrees C times 256
@return int16_t Percentage State of Charge times 256.
*/

static int16_t computeSoC(uint32_t voltage, uint32_t temperature, battery_Type type)
{
    int32_t soc;
    uint32_t v100, v50, v25;
    if (type == wetT) v100 = 3242;
    else v100 = 3280;
/* Difference between top temperature 48.9C and ambient, times 64. */
    uint32_t tDiff = (12518-temperature) >> 2;
/* Correction factor to apply to measured voltages, times 65536. */
    uint32_t vFactor = 65536-((42*tDiff*tDiff) >> 20);
/* Open circuit voltage referred to 48.9C */
    uint32_t ocv = (voltage*65536)/vFactor;
/* SoC for Wet cell and part of Gel cell */
    soc = (100*(65536 - 320*(v100-ocv))) >> 8;
    if (type == gelT)
    {
        v50 = 3178;
        if (ocv < v50)
        {
            v25 = 3075;
            if (ocv > v25) soc = (100*(65536 - 640*(v50-ocv))) >> 8;
            else soc = (100*(65536 - 320*(v25-ocv))) >> 8;
        }
    }
    if (soc > 100*256) soc = 100*256;
    return soc;
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Battery Current Offset

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBatteryCurrentOffset(int battery)
{
    return currentOffsets.dataArray.battery[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Access the Battery State of Charge

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBatterySoC(int battery)
{
    return batterySoC[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Reset the Battery State of Charge to 100%

This is done by the charging task when the battery enters float phase.
If the current SoC is less than 100%, report the battery as faulty.

@param[in] battery: 0..NUM_BATS-1
*/

void resetBatterySoC(int battery)
{
    if (batterySoC[battery] < 25600) batteryFillState[battery] = faultyF;
/* SoC is computed from the charge so this is the quantity changed. */
    batteryCharge[battery] = 25600*getBatteryCapacity(battery)*36;
}

/*--------------------------------------------------------------------------*/
/** @brief Request Calibration Sequence

*/

void startCalibration()
{
    calibrate = true;
}

/*--------------------------------------------------------------------------*/
/** @brief Change missing status of batteries

@param[in] battery: 0..NUM_BATS-1
@param[in] missing: boolean
*/

void setBatteryMissing(int battery, bool missing)
{
    if (missing) batteryHealthState[battery] = missingH;
    else batteryHealthState[battery] = goodH;
}

/*--------------------------------------------------------------------------*/
/** @brief Check the watchdog state

The watchdog counter is decremented. If it reaches zero then the task is reset.
*/

void checkMonitorWatchdog(void)
{
    if (monitorWatchdogCount++ > 10*getMonitorDelay()/getWatchdogDelay())
    {
        vTaskDelete(prvMonitorTask);
	    xTaskCreate(prvMonitorTask, (signed portCHAR * ) "Monitor", \
                    configMINIMAL_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL);
        sendStringLowPriority("D","Monitor Restarted");
        recordString("D","Monitor Restarted");
    }
}

