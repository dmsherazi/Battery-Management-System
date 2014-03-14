/* STM32F1 Power Management for Solar Power

This file defines the CANopen object dictionary variables made available to an
external PC and to other processing modules which may be CANopen devices or
tasks running on the same microcontroller.

Note these definitions cannot be placed in the header file.
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

#include "FreeRTOS.h"
#include "power-management-objdic.h"
#include "power-management-hardware.h"

/* Byte pattern that indicates if a valid NVM config data block is present */
#define VALID_BLOCK                 0xD5

/* The rate at which the charger algorithm is updated (1ms ticks) */
#define CHARGER_DELAY               ((portTickType)500/portTICK_RATE_MS)

/* The rate at which the monitoring is updated (1ms ticks) */
#define MONITOR_DELAY               ((portTickType)1000/portTICK_RATE_MS)

/* The default rate at which the samples are taken (1ms ticks) */
#define MEASUREMENT_DELAY           ((portTickType)500/portTICK_RATE_MS)

/* Delay to allow measurements to settle during the calibration sequence (1ms ticks) */
#define CALIBRATION_DELAY           ((portTickType)5000/portTICK_RATE_MS)

union ConfigGroup configDataBlock __attribute__ ((section (".configBlock"))) = {{0xA5}};
union ConfigGroup configData;

/*--------------------------------------------------------------------------*/
/** @brief Initialise Global Configuration Variables

This determines if configuration variables are present in NVM, and if so
reads them in. The first entry is checked against a preprogrammed value to
determine if the block is a valid configuration block. This allows the program
to determine whether to use the block stored in FLASH or to use defaults.
*/

void setGlobalDefaults(void)
{
    flashReadData((uint32_t*)configDataBlock.data,
                   configData.data,sizeof(configData.config));
    if (configData.config.validBlock == VALID_BLOCK) return;
    configData.config.measurementDelay = MEASUREMENT_DELAY;
    configData.config.monitorDelay = MONITOR_DELAY;
    configData.config.calibrationDelay = CALIBRATION_DELAY;
    configData.config.chargerDelay = CHARGER_DELAY;
    configData.config.alphaR = 100;           /* about 0.4 */
    configData.config.alphaV = 256;           /* No Filter */
    configData.config.alphaC = 180;           /* about 0.7, only for detecting float state. */
    configData.config.batteryCapacity[0] = 54;
    configData.config.batteryCapacity[1] = 80;
    configData.config.batteryCapacity[2] = 54;
    configData.config.batteryType[0] = wetT;
    configData.config.batteryType[1] = gelT;
    configData.config.batteryType[2] = wetT;
    configData.config.monitorStrategy = 0xFF;
/* Set default battery parameters */
    uint8_t i=0;
    for (i=0; i<NUM_BATS; i++)
    {
        if (configData.config.batteryType[i] == wetT)
        {
            configData.config.absorptionVoltage[i] = 3712;        /* 14.5V */
            configData.config.floatVoltage[i] = 3379;             /* 13.2V */
        }
        else if (configData.config.batteryType[i] == agmT)
        {
            configData.config.absorptionVoltage[i] = 3660;        /* 14.3V */
            configData.config.floatVoltage[i] = 3481;             /* 13.6V */
        }
        else if (configData.config.batteryType[i] == gelT)
        {
            configData.config.absorptionVoltage[i] = 3610;        /* 14.1V */
            configData.config.floatVoltage[i] = 3532;             /* 13.8V */
        }
        configData.config.floatStageCurrentScale[i] = 33;
        configData.config.bulkCurrentLimitScale[i] = 5;
    }
/* Zero the offsets. */
    for (i=0; i<NUM_IFS; i++) configData.config.currentOffsets.data[i] = 0;
/* Don't track unless instructed externally */
    configData.config.autoTrack = false;
    configData.config.debugMessageSend = false;
}

/*--------------------------------------------------------------------------*/
/** @brief Write Configuration Data Block to Flash

Refer to the linker script for allocation of the config data block on a
page boundary. If this is not done, a page erase may destroy valid data or code.

The current datablock is written to flash with the first entry set to a value
that indicates whether the block is a valid programmed configuration block.

@returns result code. 0 success, 1 fail.
*/

uint32_t writeConfigBlock(void)
{
    configData.config.validBlock = VALID_BLOCK;
    return flashWriteData((uint32_t*)configDataBlock.data,
                          configData.data, sizeof(configData.config));
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Battery Type

@param[in] battery: 0..NUM_BATS-1
*/

battery_Type getBatteryType(int battery)
{
    return configData.config.batteryType[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Battery Capacity

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBatteryCapacity(int battery)
{
    return configData.config.batteryCapacity[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Battery Bulk Current Limit

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getBulkCurrentLimit(int battery)
{
    return configData.config.batteryCapacity[battery]*256/
            configData.config.bulkCurrentLimitScale[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Battery Float Current Cutoff

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getFloatStageCurrent(int battery)
{
    return configData.config.batteryCapacity[battery]*256/
            configData.config.floatStageCurrentScale[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Absorption Phase Voltage Limit

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getAbsorptionVoltage(int battery)
{
    return configData.config.absorptionVoltage[battery];
}
/*--------------------------------------------------------------------------*/
/** @brief Provide the Float Phase Voltage Limit

@param[in] battery: 0..NUM_BATS-1
*/

int16_t getFloatVoltage(int battery)
{
    return configData.config.floatVoltage[battery];
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Forgetting factor for Charger Voltage Smoothing

*/

int16_t getAlphaV(void)
{
    return configData.config.alphaV;
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Forgetting factor for Charger Current Smoothing

*/

int16_t getAlphaC(void)
{
    return configData.config.alphaC;
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Forgetting factor for Resistance Estimator Smoothing

*/

int16_t getAlphaR(void)
{
    return configData.config.alphaR;
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Current Offset

@parameter uint8_t interface: the interface 0...NUM_IFS.
*/

int16_t getCurrentOffset(uint8_t interface)
{
    return configData.config.currentOffsets.data[interface];
}

/*--------------------------------------------------------------------------*/
/** @brief Set the Current Offset

@parameter uint8_t interface: the interface 0...NUM_IFS.
@parameter uint16_t current offset value
*/

void setCurrentOffset(uint8_t interface, int16_t offset)
{
    configData.config.currentOffsets.data[interface] = offset;
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Charging Time Interval

This is the time between decision updates.
*/

portTickType getChargerDelay(void)
{
    return configData.config.chargerDelay;
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Measurement Time Interval

This is the time between measurement updates.
*/

portTickType getMeasurementDelay(void)
{
    return configData.config.measurementDelay;
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Monitor Time Interval

This is the time between decision updates.
*/

portTickType getMonitorDelay(void)
{
    return configData.config.monitorDelay;
}

/*--------------------------------------------------------------------------*/
/** @brief Provide the Calibration Time Interval

This is the time between measurement updates.
*/

portTickType getCalibrationDelay(void)
{
    return configData.config.calibrationDelay;
}

/*--------------------------------------------------------------------------*/
/** @brief Recording switch

True if recording has been requested, false otherwise.
*/

bool isRecording(void)
{
    return configData.config.recording;
}

/*--------------------------------------------------------------------------*/
/** @brief Automatic Tracking switch

True if automatic tracking has been requested, false otherwise.
*/

bool isAutoTrack(void)
{
    return configData.config.autoTrack;
}

/*--------------------------------------------------------------------------*/
/** @brief Automatic Tracking switch

True if automatic tracking has been requested, false otherwise.
*/

uint8_t getMonitorStrategy(void)
{
    return configData.config.monitorStrategy;
}

/*--------------------------------------------------------------------------*/
/** @brief Return a status word showing software controls.

bit 0 if autoTrack,
bit 1 if recording,
bit 2 if calibrating

@returns uint16_t status of controls
*/
uint16_t getControls(void)
{
    uint8_t controls = 0;
    if (configData.config.autoTrack) controls |= 1<<0;
    if (configData.config.recording) controls |= 1<<1;
    if (configData.config.measurementSend) controls |= 1<<3;
    if (configData.config.debugMessageSend) controls |= 1<<4;
    return controls;
}


