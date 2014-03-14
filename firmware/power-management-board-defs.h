/* STM32F1 Power Management for Solar Power

Definitions for the Selected Microcontroller Board.

Either the ET-STM32F103 or the ET-STAMP-STM board can be used.

Copyright K. Sarkies <ksarkies@internode.on.net> 23 September 2013

Initial 23 September 2013

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

#ifndef BOARD_H_
#define BOARD_H_

/* A/D Converter Channels */

#if defined USE_ET_STM32F103

#define ADC_CHANNEL_BATTERY1_CURRENT    0
#define ADC_CHANNEL_BATTERY1_VOLTAGE    1
#define ADC_CHANNEL_BATTERY2_CURRENT    2
#define ADC_CHANNEL_BATTERY2_VOLTAGE    3
#define ADC_CHANNEL_BATTERY3_CURRENT    10
#define ADC_CHANNEL_BATTERY3_VOLTAGE    11
#define ADC_CHANNEL_PANEL_CURRENT       4
#define ADC_CHANNEL_PANEL_VOLTAGE       5
#define ADC_CHANNEL_LOAD1_CURRENT       12
#define ADC_CHANNEL_LOAD1_VOLTAGE       13
#define ADC_CHANNEL_LOAD2_CURRENT       14
#define ADC_CHANNEL_LOAD2_VOLTAGE       15
#define ADC_CHANNEL_TEMPERATURE         6

#elif defined USE_ET_STAMP_STM32

#define ADC_CHANNEL_BATTERY1_CURRENT    0
#define ADC_CHANNEL_BATTERY1_VOLTAGE    1
#define ADC_CHANNEL_BATTERY2_CURRENT    2
#define ADC_CHANNEL_BATTERY2_VOLTAGE    3
#define ADC_CHANNEL_BATTERY3_CURRENT    4
#define ADC_CHANNEL_BATTERY3_VOLTAGE    5
#define ADC_CHANNEL_PANEL_CURRENT       6
#define ADC_CHANNEL_PANEL_VOLTAGE       7
#define ADC_CHANNEL_LOAD1_CURRENT       10
#define ADC_CHANNEL_LOAD1_VOLTAGE       11
#define ADC_CHANNEL_LOAD2_CURRENT       12
#define ADC_CHANNEL_LOAD2_VOLTAGE       13
#define ADC_CHANNEL_TEMPERATURE         14

#endif

/* Controls and Status Signals */

#if defined USE_ET_STM32F103

/* The switch control code assumes the control pairs are in order */
#define SWITCH_CONTROL_PORT             GPIOB
#define SWITCH_CONTROL_SHIFT            8

/* The overcurrent reset ports */
#define BATTERY1_OVERCURRENT_RESET_PORT GPIOC
#define BATTERY1_OVERCURRENT_RESET_BIT  GPIO11
#define BATTERY2_OVERCURRENT_RESET_PORT GPIOA
#define BATTERY2_OVERCURRENT_RESET_BIT  GPIO7
#define BATTERY3_OVERCURRENT_RESET_PORT GPIOB
#define BATTERY3_OVERCURRENT_RESET_BIT  GPIO0
#define LOAD1_OVERCURRENT_RESET_PORT    GPIOB
#define LOAD1_OVERCURRENT_RESET_BIT     GPIO3
#define LOAD2_OVERCURRENT_RESET_PORT    GPIOB
#define LOAD2_OVERCURRENT_RESET_BIT     GPIO7
#define PANEL_OVERCURRENT_RESET_PORT    GPIOC
#define PANEL_OVERCURRENT_RESET_BIT     GPIO8

#define BATTERY1_STATUS_PORT            GPIOC
#define BATTERY1_STATUS_SHIFT           9
#define BATTERY2_STATUS_PORT            GPIOC
#define BATTERY2_STATUS_SHIFT           12
#define BATTERY3_STATUS_PORT            GPIOC
#define BATTERY3_STATUS_SHIFT           6
#define PANEL_STATUS_PORT               GPIOB
#define PANEL_STATUS_SHIFT              14
#define LOAD1_STATUS_PORT               GPIOB
#define LOAD1_STATUS_SHIFT              1
#define LOAD2_STATUS_PORT               GPIOB
#define LOAD2_STATUS_SHIFT              4

#elif defined USE_ET_STAMP_STM32

/* The switch control code assumes the control pairs are in order */
#define SWITCH_CONTROL_PORT             GPIOC
#define SWITCH_CONTROL_SHIFT            6

/* The overcurrent reset ports */
#define BATTERY1_OVERCURRENT_RESET_PORT GPIOA
#define BATTERY1_OVERCURRENT_RESET_BIT  GPIO13
#define BATTERY2_OVERCURRENT_RESET_PORT GPIOA
#define BATTERY2_OVERCURRENT_RESET_BIT  GPIO14
#define BATTERY3_OVERCURRENT_RESET_PORT GPIOA
#define BATTERY3_OVERCURRENT_RESET_BIT  GPIO15
#define LOAD1_OVERCURRENT_RESET_PORT    GPIOC
#define LOAD1_OVERCURRENT_RESET_BIT     GPIO12
#define LOAD2_OVERCURRENT_RESET_PORT    GPIOC
#define LOAD2_OVERCURRENT_RESET_BIT     GPIO13
#define PANEL_OVERCURRENT_RESET_PORT    GPIOC
#define PANEL_OVERCURRENT_RESET_BIT     GPIO5

#define BATTERY1_STATUS_PORT            GPIOB
#define BATTERY1_STATUS_SHIFT           0
#define BATTERY2_STATUS_PORT            GPIOA
#define BATTERY2_STATUS_SHIFT           11
#define BATTERY3_STATUS_PORT            GPIOB
#define BATTERY3_STATUS_SHIFT           4
#define PANEL_STATUS_PORT               GPIOB
#define PANEL_STATUS_SHIFT              6
#define LOAD1_STATUS_PORT               GPIOB
#define LOAD1_STATUS_SHIFT              8
#define LOAD2_STATUS_PORT               GPIOB
#define LOAD2_STATUS_SHIFT              10

#endif

/* GPIO Port Settings */

#if defined USE_ET_STM32F103

#define PA_ANALOGUE_INPUTS              GPIO0 | GPIO1 | GPIO2 | GPIO3 /*| GPIO4 | GPIO5 |
                                               GPIO6*/
#define PA_DIGITAL_OUTPUTS              /*GPIO7 | GPIO8 | */ GPIO11
#define PB_DIGITAL_OUTPUTS              GPIO0 | GPIO3 | GPIO6 | GPIO8 | GPIO9 | GPIO10 |\
                                        GPIO11 | GPIO12 | GPIO13
#define PB_DIGITAL_INPUTS               GPIO1 | GPIO2 | GPIO4 | GPIO5 | GPIO14 | GPIO15
#define PC_ANALOGUE_INPUTS              GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4
#define PC_DIGITAL_INPUTS               /*GPIO6 |*/ GPIO7 | GPIO9 | GPIO10 | GPIO12 | GPIO13

#elif defined USE_ET_STAMP_STM32

#define PA_ANALOGUE_INPUTS              GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 | \
                                                GPIO6 | GPIO7
#define PA_DIGITAL_INPUTS               GPIO11 | GPIO12
#define PA_DIGITAL_OUTPUTS              GPIO8 | GPIO13 | GPIO14 | GPIO15
#define PC_ANALOGUE_INPUTS              GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4
#define PC_DIGITAL_OUTPUTS              GPIO5 | GPIO6 | GPIO7 | GPIO8 | GPIO9 | GPIO10 |\
                                                GPIO11 | GPIO12 | GPIO13
#define PB_DIGITAL_INPUTS               GPIO0 | GPIO1 | GPIO2 | GPIO3 | GPIO4 | GPIO5 |\
                                                GPIO6 | GPIO7 | GPIO8 | GPIO9 | GPIO10 | \
                                                GPIO11
#endif

#endif

