# Basic makefile K Sarkies

PROJECT	    = power-management
VERSION    ?= 3
PREFIX	    = arm-none-eabi
CC		    = $(PREFIX)-gcc
LD		    = $(PREFIX)-gcc
OBJCOPY	    = $(PREFIX)-objcopy
OBJDUMP	    = $(PREFIX)-objdump
GDB		    = $(PREFIX)-gdb
NM 		    = $(PREFIX)-nm

MCU             = cortex-m3
CHIP            = STM32F10x_128k_20k
BOARD           = ET_STAMP_STM32
F_XTAL          = 8000000
SYSCLOCK_CL     = SYSCLK_FREQ_72MHz=72000000

LIBRARY_DIR     = /home/ksarkies/Development-Software/arm-library
DRIVERS_DIR	    = $(LIBRARY_DIR)/libopencm3-examples/libopencm3
DRIVERS_SRC     = $(DRIVERS_DIR)/lib/stm32/f1
DRIVERS_INC	    = $(DRIVERS_DIR)/include
FREERTOS_DIR    = $(LIBRARY_DIR)/FreeRTOSV8.2.1/FreeRTOS
FREERTOS_DEV	= $(FREERTOS_DIR)/Source/portable/GCC/ARM_CM3
FREERTOS_INC	= $(FREERTOS_DIR)/Source/include
FREERTOS_SRC	= $(FREERTOS_DIR)/Source
FREERTOS_MMG	= $(FREERTOS_DIR)/Source/portable/MemMang
FATFSDIR        = ../chan-fat-stm32-loc3

VPATH      += $(FREERTOS_SRC)/ $(FREERTOS_DEV)/ $(FREERTOS_MMG) $(FATFSDIR)

# Inclusion of header files
INCLUDES	= $(patsubst %,-I%,$(FREERTOS_INC) $(DRIVERS_INC) $(FATFSDIR))

CDEFS      += -DHSE_VALUE=$(F_XTAL)UL
CDEFS      += -D$(SYSCLOCK_CL)
CDEFS      += -DUSE_$(BOARD)
CDEFS      += -DVERSION=$(VERSION)

CFLAGS	   += -Os -g -Wall -Wextra -Wno-unused-variable -I. $(INCLUDES) -fno-common -mthumb -MD
CFLAGS	   += -DGCC_ARMCM3 -ffunction-sections -fdata-sections
CFLAGS	   += -mcpu=cortex-m3 -DSTM32F1 -msoft-float
CFLAGS	   += $(CDEFS)

LDSCRIPT    = stm32-h103RET6.ld

LDFLAGS	   += -I . -lc -T$(LDSCRIPT) -L$(DRIVERS_DIR)/lib -nostartfiles -Wl,--gc-sections
LDFLAGS	   += -lopencm3_stm32f1
LDFLAGS	   += -specs=nosys.specs

CFILES	    = $(PROJECT).c $(PROJECT)-comms.c $(PROJECT)-file.c
CFILES     += $(PROJECT)-monitor.c $(PROJECT)-hardware.c
CFILES     += $(PROJECT)-lib.c $(PROJECT)-time.c $(PROJECT)-objdic.c
CFILES     += $(PROJECT)-measurement.c $(PROJECT)-watchdog.c
CFILES     += ff.c sd_spi_loc3_stm32_freertos.c fattime.c freertos.c
CFILES     += tasks.c list.c queue.c timers.c port.c heap_1.c
CFILES     += $(PROJECT)-charger.c

OBJS		= $(CFILES:.c=.o)

all: $(PROJECT).elf $(PROJECT).bin $(PROJECT).hex $(PROJECT).lss $(PROJECT).list $(PROJECT).sym

$(PROJECT).elf: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(PROJECT).hex: $(PROJECT).elf
	$(OBJCOPY) -O ihex  $< $@

$(PROJECT).bin: $(PROJECT).elf
	$(OBJCOPY) -O binary  $< $@

$(PROJECT).lss: $(PROJECT).elf
	$(OBJDUMP) -h -S $< > $@

$(PROJECT).list: $(PROJECT).elf
	$(OBJDUMP) -S $< > $@

$(PROJECT).sym: $(PROJECT).elf
	$(NM) -n $< > $@

clean:
	rm *.elf *.o *.d *.hex *.list *.sym *.bin *.lss

# Using CC and CFLAGS will cause any object files to be built implicitely if they are missing.
# We are searching an archive library opencm3_stm32f1.a which has been precompiled, so we don't need
# to recompile the DRIVERS source.
# The libopencm3 source has been added in case the archive doesn't have them, VPATH finds them.

