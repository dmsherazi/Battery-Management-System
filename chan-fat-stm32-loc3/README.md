This is an adaptation of ChaN'v FAT filesystem library based on v0.10b.

Common files are ff.c, ff.h, ffconf.h, integer.h, diskio.h. These should be
updated when a new version of ChaN FAT library is released.

The remaining files adapt the library to the hardware being used, to FreeRTOS
and to libopencm3. Some changes to these may be necessary if the ChaN FAT API
changes.
