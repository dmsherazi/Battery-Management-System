Battery Management System Interconnect
--------------------------------------

PCB for the interconnection of all interfaces and the switch electronics.

The switch provides power for the electronics. This is converted to the 3.3V
needed and is distributed to all interfaces. A couple of digital ssi gates are
used to disconnect switches when the interfaces signal overcurrent or low
voltage. The former are only used for the loads and the latter only for the
solar module. The batteries are not disconnected on overload (although the
firmware could disconnect them) as the loads will be the cause of such
overcurrents and the disconnection of batteries would be redundant. There is no
protection for internal faults that could overload the batteries.

There is an interface for logic level serial connection for use with an RS232
adapter, and a JTAG (SWI) debug connector. If this debug is used, the firmware
must be recompiled and reloaded to prevent the program capturing the JTAG pins
(this disables some software overcurrent/overvoltage detection).

The PCB has a 50 pin socket for the ET-STAMP-STM32 module from Futurlec. 25
pin SIL headers can also be obtained from them to make up a plug-in socket.

A schematic for an on-board STM32F processor is also given here, but this is not
used for this particular PCB version.

(c) K. Sarkies 02/04/2014

Todo:
1. Redesign card with on-board processor.

