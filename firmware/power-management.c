/**
@mainpage STM32F1 Power Management for Solar Power
@version 1.0.0
@author Ken Sarkies (www.jiggerjuice.info)
@date 13 July 2013
@brief Management of solar battery charger and battery state monitor.

The power management system provides interface current, voltage (analogue),
overcurrent and undervoltage (digital) signals. This program measures these
quantities, stores and transmits them to an external PC.

Three batteries, two loads and a single solar module (panel) are provided.

A number of tasks are activated to manage the charge/discharge of the batteries
and to deal with events such as overloads or undervoltages. In addition
a command interface is established for external override controls.

The program estimates State of Charge (SoC) of each of the batteries and
tracks it using Coulomb Counting. The SoC is reset whenever the batteries
are idle for a significant period of time.

FreeRTOS provides the real time operating system.

CANopen provides the smart transducer communications through CanFestival.

The board used is the ET-STM32F103 (RBT6) or ET-ARM-STAMP (RET6).
(On this board, ADC1 channels 2 and 3 (PA2, PA3) are shared with USART2 and two
jumpers J13,J14 need to be moved to access the analogue ports. Also PA6-8 are
used by the SPI1 for the MMIC card interface, so the latter cannot be used.).

Copyright K. Sarkies <ksarkies@internode.on.net> 23 August 2013

Initial 13 July 2013
FreeRTOS 9 August 2013

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

/* libopencm3 driver includes */
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "power-management-hardware.h"
#include "power-management-objdic.h"
#include "power-management-comms.h"
#include "power-management-file.h"
#include "power-management-time.h"
#include "power-management-measurement.h"
#include "power-management-monitor.h"
#include "power-management-charger.h"
#include "power-management.h"

/* This is provided in the FAT filesystem library */
extern void disk_timerproc();

/*--------------------------------------------------------------------------*/

int main(void)
{
    setGlobalDefaults();        /* From objdic */
    prvSetupHardware();         /* From hardware */
    initComms();                /* From comms */

/* Start the communications task. */
	xTaskCreate(prvCommsTask, (signed portCHAR * ) "Communications", \
                configMINIMAL_STACK_SIZE, NULL, COMMS_TASK_PRIORITY, NULL);

/* Start the file management task. */
	xTaskCreate(prvFileTask, (signed portCHAR * ) "FileManagement", \
                configMINIMAL_STACK_SIZE, NULL, FILE_TASK_PRIORITY, NULL);

/* Start the measurement task. */
	xTaskCreate(prvMeasurementTask, (signed portCHAR * ) "Measurement", \
                configMINIMAL_STACK_SIZE, NULL, MEASUREMENT_TASK_PRIORITY, NULL);

/* Start the monitor task. */
	xTaskCreate(prvMonitorTask, (signed portCHAR * ) "Monitor", \
                configMINIMAL_STACK_SIZE, NULL, MONITOR_TASK_PRIORITY, NULL);

/* Start the charger task. */
	xTaskCreate(prvChargerTask, (signed portCHAR * ) "Charger", \
                configMINIMAL_STACK_SIZE, NULL, CHARGER_TASK_PRIORITY, NULL);

/* Start the scheduler. */
	vTaskStartScheduler();

/* Will only get here if there was not enough heap space to create the
idle task. */
	return -1;
}

/*-----------------------------------------------------------*/
/*----       ISR Overrides in libopencm3     ----------------*/
/*-----------------------------------------------------------*/
/* These trap serious execution errors and reset the application */

/*-----------------------------------------------------------*/
void nmi_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void hard_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void memory_manage_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void bus_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
void usage_fault_handler(void)
{
    scb_reset_system();
}

/*-----------------------------------------------------------*/
/*----    FreeRTOS ISR Overrides in libopencm3     ----------*/
/*-----------------------------------------------------------*/

extern void xPortPendSVHandler(void);
extern void xPortSysTickHandler(void);
extern void vPortSVCHandler( void );

/*-----------------------------------------------------------*/
void sv_call_handler(void)
{
  	vPortSVCHandler();
}

/*-----------------------------------------------------------*/

void pend_sv_handler(void)
{
  	xPortPendSVHandler();
}

/*-----------------------------------------------------------*/
/** @Brief Systick Interrupt Handler

This provides a link to the FreeRTOS systick interrupt handler.

Systick is setup in FreeRTOS according to the tick rate specified in
FreeRTOSConfig.h. That rate is set at 100Hz.

This also updates the status of any inserted SD card.

Can be used to provide a RTC.
*/
void sys_tick_handler(void)
{

	disk_timerproc();

/* updated every 1s if systick is used for real-time clock. */
/*
	static uint16_t cnttime=0;
	cnttime++;
	if (cnttime >= 100)
    {
		cnttime = 0;
		updateTimeCount();
	}
*/
  	xPortSysTickHandler();
}

