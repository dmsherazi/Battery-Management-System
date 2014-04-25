Battery Management System Interconnect
--------------------------------------

PCB for the interconnection of all interfaces and the switch electronics.

The switch provides power for the electronics. This is converted to the 3.3V
needed and is distributed to all interfaces. A couple of digital ssi gates are
used to disconnect switches when the interfaces signal overcurrent or low
voltage. The former are only used for the loads and the latter only for the
solar module.

There is an interface for logic level serial connection for use with an RS232
adapter, and a JTAG (SWI) debug connector. If this debug is used, the firmware
must be recompiled to prevent the program using the JTAG pins (this disables
some software overcurrent/overvoltage detection).

(c) K. Sarkies 25/04/2014

