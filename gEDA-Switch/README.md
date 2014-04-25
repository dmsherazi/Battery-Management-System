Battery Management System Switch
--------------------------------

PCB for the MOSFET switch matrix.

Digital circuitry ensures that no two batteries are connected together, which
could cause damage.

A diode matrix is used to provide power for the electronics. It ensures that
power is taken from the loads, or from the batteries if the loads are not
connected. This enables the firmware to keep track of the current drawn by
the local electronics.

The MOSFETs are driven by simple BJT-resistor drivers. This results in quite
slow switching speeds. There may be a need for faster switching e.g. high speed
PWM to reduce EMI. In this case the circuit needs to be modified to use better
drivers.

(c) K. Sarkies 25/04/2014

