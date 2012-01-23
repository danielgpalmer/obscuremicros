ifneq ($(KERNELRELEASE),)
include Kbuild
else

# Normal Makefile	
KERNELDIR := /lib/modules/`uname -r`/build

all::
	$(MAKE) -C $(KERNELDIR) M=`pwd` $@
	
# Module specific targets
genbin:
	echo "X" > leds-landisk.o_shipped
endif
