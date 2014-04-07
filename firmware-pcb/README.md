Battery Management System Firmware
Version: Second Prototype
----------------------------------

The firmware is written for an STM32F103 ARM Cortex M3 microcontroller and uses
specific hardware published here.

The firmware uses libopencm3 for hardware drivers, FreeRTOS and ChaN FAT for
managing the local storage filesystem.

Several tasks are defined to manage:

- command and data communications,
- storage,
- battery state monitoring,
- charge control,
- measurement,
- program execution watchdog and error recovery.

It is designed to use CANopen to manage communications with an external PC, in
case the project is further developed.

The hardware component values set the various scaling factors for voltages and
currents, and these may change for different hardware versions.

This prototype uses PCBs with slight differences in the interfaces for
measurement and switch PWM.

(c) K. Sarkies 7/04/2014

