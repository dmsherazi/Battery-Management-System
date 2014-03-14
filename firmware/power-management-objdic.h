/* STM16F1 Power Management for Solar Power

This file defines the CANopen object dictionary variables made available to an
external PC and to other processing modules which may be CANopen devices or
tasks running on the same microcontroller.
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

#include "FreeRTOS.h"

#ifndef POWER_MANAGEMENT_OBJDIC_H_
#define POWER_MANAGEMENT_OBJDIC_H_

#define NUM_BATS    3
#define NUM_LOADS   2
#define NUM_PANELS  1
#define NUM_IFS     NUM_BATS+NUM_LOADS+NUM_PANELS
#define LOAD_1      0
#define LOAD_2      1
#define PANEL       2

typedef enum {wetT=0, gelT=1, agmT=2} battery_Type;
typedef enum {normalF=0, lowF=1, criticalF=2} battery_Fl_States;
typedef enum {loadedO=0, chargingO=1, isolatedO=2, missingO=3} battery_Op_States;
typedef enum {bulkC=0, absorptionC=1, floatC=2, equalizationC=3} battery_Ch_States;

/* Represent the measured data arrays in a union as separate or combined */
struct Interface
{
    int16_t battery[NUM_BATS];
    int16_t load[NUM_LOADS];
    int16_t panel[NUM_PANELS];
};

/* These offsets are for battery 1-3, load 1-2 and panel, in order */
union InterfaceGroup
{
    int16_t data[NUM_IFS];
    struct Interface dataArray;
};

/*--------------------------------------------------------------------------*/
/****** Object Dictionary Items *******/
/* Configuration items, updated externally, are stored to NVM */

/*--------------------------------------------------------------------------*/
struct Config
{
/* Valid data block indicator */
    uint8_t validBlock;
/* Battery characteristics and model parameters */
    uint16_t batteryCapacity[NUM_BATS];
    battery_Type batteryType[NUM_BATS];
    int16_t absorptionVoltage[NUM_BATS];
    int16_t floatVoltage[NUM_BATS];
    int16_t floatStageCurrentScale[NUM_BATS];
    int16_t bulkCurrentLimitScale[NUM_BATS];
    int16_t alphaR;         /* forgetting factor for battery resistance estimator */
    int16_t alphaV;         /* forgetting factor for battery voltage smoothing */
    int16_t alphaC;         /* forgetting factor for battery current smoothing */
/* Control Variables */
    bool measurementSend;       /* Controls if measurements are transmitted */
    bool debugMessageSend;      /* Controls if debug messages are transmitted */
    bool recording;             /* Controls recording of performance data */
    bool autoTrack;             /* Allows automatic management of batteries */
    uint8_t monitorStrategy;
/* Delay Variables */
    portTickType measurementDelay;
    portTickType monitorDelay;
    portTickType calibrationDelay;
    portTickType chargerDelay;
/* System Parameters */
    union InterfaceGroup currentOffsets;
};

/* Map the configuration data also as a block of words.
Block size equal to a FLASH page (2048 bytes) to avoid erase problems.
Needed for reading and writing from Flash. */
#define CONFIG_BLOCK_SIZE       2048
union ConfigGroup
{
    uint8_t data[CONFIG_BLOCK_SIZE];
    struct Config config;
};

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/

void setGlobalDefaults(void);
uint32_t writeConfigBlock(void);

battery_Type getBatteryType(int battery);
int16_t getBatteryCapacity(int battery);
int16_t getBulkCurrentLimit(int battery);
int16_t getFloatStageCurrent(int battery);
int16_t getAbsorptionVoltage(int battery);
int16_t getFloatVoltage(int battery);
int16_t getAlphaV(void);
int16_t getAlphaC(void);
int16_t getAlphaR(void);
int16_t getCurrentOffset(uint8_t interface);
void setCurrentOffset(uint8_t interface, int16_t offset);
portTickType getChargerDelay(void);
portTickType getMeasurementDelay(void);
portTickType getMonitorDelay(void);
portTickType getCalibrationDelay(void);
bool isRecording(void);
bool isAutoTrack(void);
uint8_t getMonitorStrategy(void);
uint16_t getControls(void);

#endif

