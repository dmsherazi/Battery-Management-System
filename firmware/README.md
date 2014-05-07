Battery Management System Firmware
Version: First Prototype
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

Two versions are here represented. The second refers to slightly different
voltage and current scales in the measurement header for the PCB version.
It is compiled with 'make VERSION=2'

(c) K. Sarkies 08/04/2014

TODO

1. Firmware upload - from within the program. Base on stm32flash.
2. Comms - test queue space available for each data message to send.
4. Measurement - Increase switching speed to 25kHz. May need hardware changes.
5. Charger - add acceleration to pull overvoltage back quickly.
6. Comms - additional commands for more configuration options.
7. Comms - by default do not send messages until instructed.
8. File - add file info (date, size) to GUI.
9. GUI - download feature (may cause saturation of the comms interface).

BUGS

1. Time - fix date setting.
2. Monitor - missing battery, when removed, is not handled and loads are still
     allocated to it.
3. Monitor - when battery drops voltage below critical, needs to be removed
     from loads until recharged.

