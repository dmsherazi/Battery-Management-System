Battery Management System Circuit Breaker
-----------------------------------------

PCB for the interfaces between the external power sources and loads, and the
internal switch.

The current passes through a current sense resistor, and voltage is measured
with an offset differential amplifier, but there is no direct on-board
connection between the power and circuit commons. This is commoned elsewhere
(in the switch).

The circuit breaker passes a signal back to the interconnect board where it
is used for hardware isolation of the particular switch. For this reason the
correct power source and circuit interfaces must be used together, otherwise
the circuit breaker will be ineffective. Similarly for the low voltage
detection. The circuit breaker is based on those proposed by R Paisley
http://home.cogeco.ca/~rpaisley4/CircuitBreaker05.html

(c) K. Sarkies 31/08/2014

TODO:

1. The reference voltage to the current monitor is taken from the 2.5V
reference for the voltage measurement circuit. However this latter uses the
battery ground rather than the local ground, and a small bias voltage can
be present. Ideally the current monitor should have its own reference voltage.
