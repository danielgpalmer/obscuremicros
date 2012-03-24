TOP = $(shell pwd)
TCPREFIX = ../../toolchains/inst/h8300-elf/bin
CC      = $(TCPREFIX)/h8300-elf-gcc
LD      = $(TCPREFIX)/h8300-elf-ld
AR      = $(TCPREFIX)/h8300-elf-ar
AS      = $(TCPREFIX)/h8300-elf-as
CP      = $(TCPREFIX)/h8300-elf-objcopy
OD      = $(TCPREFIX)/h8300-elf-objdump
GDB     = $(TCPREFIX)/h8300-elf-gdb

