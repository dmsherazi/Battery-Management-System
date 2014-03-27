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

(c) K. Sarkies 15/03/2014

TODO

1. Firmware upload - from within the program. Base on stm32flash
2. Monitor - move load to another battery after SoC has fallen to 90%.
3. Comms - test queue space available for each data message to send.
4. Measurement - Increase switching speed to 25kHz. May need hardware changes.
5. Charger - add acceleration to pull overvoltage back quickly.
6. Time - fix date setting.
7. Comms - additional commands for more configuration options.
8. Comms - by default do not send messages until instructed.

