Battery Management System Firmware
Version: First Prototype
----------------------------------

The firmware is written for an STM32F103 ARM Cortex M3 microcontroller and uses
specific hardware published here.

The firmware uses libopencm3 for hardware drivers, FreeRTOS for task management
and ChaN FAT for managing the local storage filesystem.

Several tasks are defined to manage:

- command and data communications,
- storage,
- battery state monitoring,
- charge control,
- measurement,
- program execution watchdog and error recovery.

It is designed to be adapted to allow CANopen to manage communications with an
external PC, in case the project is further developed.

The hardware component values set the various scaling factors for voltages and
currents, and these may change for different hardware versions.

Three versions are here represented. Each refers to slightly different
voltage and current scales in the measurement header for the PCB version.
It is compiled with 'make VERSION=x' where x = 2 or 3. Version 1 is obsolete.

Two charger algorithms are provided: three phase and interrupted charge control.
It is compiled with 'make CHARGER=x" where x = ORIG, 3PH or ICC. The ORIG
algorithm is three phase, now deprecated as development is now centered on 3PH.

(c) K. Sarkies 30/08/2014

TODO

1. Firmware upload - from within the program. Base on stm32flash.
2. Comms - test queue space available for each data message to send.
4. Measurement - Increase switching speed to 25kHz. May need hardware changes.
5. Charger - add acceleration to pull overvoltage back quickly.
6. Comms - additional commands for more configuration options.
7. Comms - by default do not send messages until instructed, to save power.
8. File - add file info (date) to GUI.
9. GUI - download feature (may cause saturation of the comms interface).

BUGS


