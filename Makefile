all: toolchains
	$(MAKE) -C arm-softfloat-eabi
	$(MAKE) -C h8300-elf

.PHONY: clean toolchains

clean:
	$(MAKE) -C arm-softfloat-eabi clean
	$(MAKE) -C h8300-elf clean

toolchains:
	cd toolchains && ./mkchain.sh arm-softfloat-eabi
	cd toolchains && ./mkchain.sh h8300-elf
	cd toolchains && ./mkchain.sh sh-elf
	#cd toolchains && ./mkchain.sh rx-elf
