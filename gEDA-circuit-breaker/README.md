Battery Management System Circuit Breaker
-----------------------------------------

PCB for the interfaces between the external power sources and loads, and the
internal switch.

The power passes through a current sense resistor and voltage is measured with
an offset differential amplifier, but there is no direct connection between the
power and circuit commons. This is commoned elsewhere (in the switch).

The circuit breaker passes a signal back to the interconnect board where it
is used for hardware isolation of the particular switch. For this reason the
correct power source and circuit interfaces must be used together, otherwise
the circuit breaker will be ineffective. Similarly for the low voltage
detection. The circuit breaker is based on those proposed by R Paisley
http://home.cogeco.ca/~rpaisley4/CircuitBreaker05.html

(c) K. Sarkies 25/04/2014

