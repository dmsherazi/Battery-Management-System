/* STM32F1 Power Management for Solar Power

Defines the time and date counter to carry the real time count in seconds for
time stamping of records. Access functions are provided for setting from a UTC
string and reading to a string.

Initial 25 November 2013
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

#include "power-management-objdic.h"
#include "power-management-hardware.h"
#include "power-management-lib.h"
#include "power-management-time.h"
#include "power-management-comms.h"

static const uint8_t DaysInMonth[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

/*--------------------------------------------------------------------------*/
/* @brief Return a string containing the time and date

Based on code from Peter Dannegger found in the mikrocontroller.net forum
but 100 year exceptions not included.

@param[out] timeString char*. Returns pointer to string with formatted date.
*/

void putTimeToString(char* timeString)
{
    uint32_t timeCounter = getTimeCounter();
    uint8_t second = timeCounter % 60;
    uint8_t minute = (timeCounter/60) % 60;
    uint8_t hour = (timeCounter/3600) % 24;
    uint16_t day = (uint16_t)(timeCounter/86400);
	uint16_t year = 0;
    uint16_t dayOfYear = 365;
    uint16_t dayOfLastYear = 365;

/* Break down into years since 2000 and number of days left in the month */
	for(;;)
    {
        dayOfLastYear = dayOfYear;
		if ((year & 0x03) == 0) dayOfYear = 366;
        else  dayOfYear = 365;
		if (day < dayOfYear) break;
		day -= dayOfYear;
		year++;
	}
	year += 2000;

/* Add in Feb 29 if the last year was a leap year (quirk of above loop) */
	if((dayOfLastYear & 1) && (day > 58)) day++;

	uint8_t month;
	for(month = 1; day >= DaysInMonth[month-1]; month++)
		day -= DaysInMonth[month-1];

	uint8_t dayOfMonth = day + 1;
    char buffer[10];
    intToAscii(year, timeString);
    stringAppend(timeString,"-");
    if (month < 10) stringAppend(timeString,"0");
    intToAscii(month, buffer);
    stringAppend(timeString,buffer);
    stringAppend(timeString,"-");
    if (dayOfMonth < 10) stringAppend(timeString,"0");
    intToAscii(dayOfMonth, buffer);
    stringAppend(timeString,buffer);
    stringAppend(timeString,"T");
    intToAscii(hour, buffer);
    if (hour < 10) stringAppend(timeString,"0");
    stringAppend(timeString,buffer);
    stringAppend(timeString,":");
    intToAscii(minute, buffer);
    if (minute < 10) stringAppend(timeString,"0");
    stringAppend(timeString,buffer);
    stringAppend(timeString,":");
    intToAscii(second, buffer);
    if (second < 10) stringAppend(timeString,"0");
    stringAppend(timeString,buffer);
}

/*--------------------------------------------------------------------------*/
/* @brief Set the time variable from an ISO 8601 formatted date/time

Based on code from LalaDumm found in the mikrocontroller.net forum.
100 year exceptions not included.

@param[in] isoTime: pointer to string with formatted date.
*/

void setTimeFromString(char* timeString)
{
    char buffer[5];
	uint8_t i;
	uint32_t days = 0;

    for (i=0; i<4; i++) buffer[i] = timeString[i];
    buffer[4] = 0;
    uint16_t year = asciiToInt(buffer);
    for (i=0; i<2; i++) buffer[i] = timeString[i+5];
    buffer[2] = 0;
    uint16_t month = asciiToInt(buffer);
    for (i=0; i<2; i++) buffer[i] = timeString[i+8];
    uint16_t dayOfMonth = asciiToInt(buffer);
    for (i=0; i<2; i++) buffer[i] = timeString[i+11];
    uint16_t hour = asciiToInt(buffer);
    for (i=0; i<2; i++) buffer[i] = timeString[i+14];
    uint16_t minute = asciiToInt(buffer);
    for (i=0; i<2; i++) buffer[i] = timeString[i+17];
    uint16_t second = asciiToInt(buffer);

	/* Calculate days of years after 2000 */
    year -= 2000;
	days = (uint32_t)year * 365;
    days += (year+3)/4;

	/* Loop thru each month, adding the days */
	for (i = 0; i < month - 1; i++) days += DaysInMonth[i];

	/* Leap year? adjust February */
	if (((year & 0x03) == 0) && (month > 2)) days++;

	/* Add remaining days */
	days += dayOfMonth;

	/* Convert to seconds, add all the other stuff */
	uint32_t seconds = (days-1) * 86400L + (uint32_t)hour * 3600 +
		(uint32_t)minute * 60 + second;

	setTimeCounter(seconds);
}

