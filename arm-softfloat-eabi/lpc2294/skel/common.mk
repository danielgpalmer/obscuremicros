TCPREFIX = ../../../toolchains/inst/arm-softfloat-eabi/bin
CC      = $(TCPREFIX)/arm-softfloat-eabi-gcc
LD      = $(TCPREFIX)/arm-softfloat-eabi-ld
AR      = $(TCPREFIX)/arm-softfloat-eabi-ar
AS      = $(TCPREFIX)/arm-softfloat-eabi-as
CP      = $(TCPREFIX)/arm-softfloat-eabi-objcopy
OD      = $(TCPREFIX)/arm-softfloat-eabi-objdump
GDB     = $(TCPREFIX)/arm-softfloat-eabi-gdb

CFLAGS  = -I./ -c -fno-common -O0 -ggdb --std=gnu99
AFLAGS  = -mapcs-32
LFLAGS  =  -nostartfiles  -Wl,-Map=output.map
CPFLAGS = -O binary
ODFLAGS = -x --syms

FATFSCFLAGS = -I ./fatfs/src/
