A solar power battery management system that manages the charging of up to three
batteries from a bank of solar panels. Two load priorities are managed, with the
low priority loads automatically disconnected when the battery charge falls
below critical on all batteries. The high priority loads are maintained as long
as possible.

System state is reported extensively and recording to SD card is available.

A GUI is provided to allow control and observation of operations.

Each interface has an overcurrent circuit breaker and low voltage detector, as
well as providing current and voltage measurements.

A central controller based on STM32F103 controls all interfaces and the MOSFET
switches directly.

A data processing program is also provided to provide reports of various types
from the recorded performance data.

