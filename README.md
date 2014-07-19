Battery Management System
-------------------------

This project provides software for monitoring, control and charging of batteries
for a solar powered installation.

The motivation to build this arose from the poor performance and lack of
adequate control, lack of configuration for different battery types, and poor
user interface of commercially available BMS, at least for those reasonably
priced.

The system allows for up to three batteries to be managed separately. This
ensures that failed batteries may be isolated without adversely affecting the
other batteries as could happen if batteries were connected in parallel. Also
note that if batteries were connected in parallel, the same type and age of
battery must be used, which would necessitate changing all batteries if one were
to fail.

The system also allows for up to two loads so that critical low current loads
may be kept running after high current loads are disconnected in the event
that all batteries fall to critically low charge levels.

The system uses several methods of tracking the State of Charge of each battery,
This is a notoriously difficult parameter to estimate. In a system with three
batteries one may be isolated long enough to allow the terminal voltage to
settle to a level that will allow SoC to be computed accurately.

In the long term it is intended to allow a variety of configurations for
charging and monitoring of batteries.

A wide range of state information is stored on a removable memory, and also
transmitted over a serial link to allow a GUI to display the state accurately
and in detail.

A GUI is provided to allow control and observation of operations.

Each interface has an overcurrent circuit breaker and low voltage detector, as
well as providing current and voltage measurements.

A central controller based on STM32F103 controls all interfaces and the MOSFET
switches directly.

A data processing program is also provided to generate reports of various types
from the recorded performance data.

More information is availale on:

http://www.jiggerjuice.info/electronics/projects/solarbms/solarbms-overview.html

(c) K. Sarkies 15/03/2014

TODO

1. Improvement of MOSFET switch drivers to reduce EMI at higher rates.
2. Development of a 433MHz transponder set to convert serial transmissions 3.3V
   and RS232 levels, to RF signalling.
3. Development of a 50W linear regulator to add to the solar panel side. This
   will allow avoidance of switching while limiting the battery charge voltage.
   Ideally this would be controlled through the processor DAC but that would
   need major hardware changes.

