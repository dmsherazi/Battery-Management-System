/* STM32F1 Power Management for Solar Power

This header file contains defines and prototypes specific to the
file management task.

Initial 1 October 2013
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

#ifndef POWER_MANAGEMENT_FILE_H_
#define POWER_MANAGEMENT_FILE_H_

#define FILE_QUEUE_SIZE             256
#define FILE_SEND_TIMEOUT          ((portTickType)2000/portTICK_RATE_MS)

#define MAX_OPEN_FILES              2

/*--------------------------------------------------------------------------*/
/* Prototypes */
/*--------------------------------------------------------------------------*/
void prvFileTask(void *pvParameters);
uint8_t recordString(char* ident, char* string);
uint8_t recordDual(char* ident, int32_t param1, int32_t param2);
uint8_t recordSingle(char* ident, int32_t param1);
void sendFileCommand(char command, uint8_t length, uint8_t *parameters);

#endif

