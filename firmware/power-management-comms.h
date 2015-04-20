/* STM32F1 Power Management for Solar Power

This header file contains defines and prototypes specific to the
communications task. A number of convenience functions are
provided for sending different formats.

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

#ifndef POWER_MANAGEMENT_COMMS_H_
#define POWER_MANAGEMENT_COMMS_H_

#include <stdint.h>
#include <stdbool.h>

#define COMMS_QUEUE_SIZE            512
#define COMMS_SEND_DELAY            ((portTickType)1000/portTICK_RATE_MS)
#define COMMS_SEND_TIMEOUT          ((portTickType)2000/portTICK_RATE_MS)

#define COMMS_FILE_TIMEOUT          ((portTickType)1000/portTICK_RATE_MS)

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/
void prvCommsTask(void *pvParameters);
void initComms(void);
void dataMessageSend(char* ident, int32_t parm1, int32_t parm2);
void dataMessageSendLowPriority(char* ident, int32_t param1, int32_t param2);
void sendResponse(char* ident, int32_t parameter);
void sendResponseLowPriority(char* ident, int32_t parameter);
void sendDebugResponse(char* ident, int32_t parameter);
void sendString(char* ident, char* string);
void sendStringLowPriority(char* ident, char* string);
void sendDebugString(char* ident, char* string);

#endif

