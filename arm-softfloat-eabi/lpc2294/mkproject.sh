#!/bin/bash

set -e

if [ $# -ne 1 ]; then
	echo "usage; $0 <new project name>";
	exit 0;
fi;


if [ -e $1 ]; then
	echo "directory already exists";
	exit 1;
fi;

mkdir $1;

LINKEDSOURCES="crt-ram.s crt.s \
syscalls.c syscalls.h \
lpc21xx.h lpc22xx.h \
core.h core.c config.h .gdbinit \
interrupts.c interrupts.h \
uart.c uart.h lpcUART.h \
timer.c timer.h \
rtc.c rtc.h \
adc.c adc.h \
wdt.c wdt.h \
i2c.c i2c.h \
exceptions.h exceptions.c \
common.mk peripherals.mk sys.mk startup.mk \
mkrtcpreload.sh \
ldscript-intram.x ldscript-intflash.x ldscript-extram.x"

COPYSOURCES="Makefile main.c"

cd $1;

for SOURCE in $LINKEDSOURCES; do
	ln -s ../skel/$SOURCE $SOURCE;
done;

for SOURCE in $COPYSOURCES; do
	cp ../skel/$SOURCE ./;
done


ls -l
make
