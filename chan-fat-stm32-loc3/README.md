This is an adaptation of ChaN's FatFS FAT filesystem library based on v0.10b.

Common files are ff.c, ff.h, ffconf.h, integer.h, diskio.h. These should be
updated when a new version of ChaN FAT library is released. Ensure that any
changes to ffconf.h are reflected in the new version.

The remaining files adapt the library to the hardware being used, to FreeRTOS
and to libopencm3. Some changes to these may be necessary if the ChaN FAT API
changes.

More information is available on:

http://www.jiggerjuice.info/electronics/projects/solarbms/solarbms-software.html

(c) K. Sarkies 8/12/2014

