all: toolchains
	$(MAKE) -C arm-softfloat-eabi

.PHONY: clean toolchains

clean:
	$(MAKE) -C arm-softfloat-eabi clean

toolchains:
	cd toolchains && ./mkchain.sh arm-softfloat-eabi
	cd toolchains && ./mkchain.sh h8300-elf
	cd toolchains && ./mkchain.sh sh-elf
	cd toolchains && ./mkchain.sh rx-elf
