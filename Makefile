.PHONY: toolchains

toolchains:
	cd toolchains && ./mkchain.sh arm-softfloat-eabi
	cd toolchains && ./mkchain.sh h8300-elf
	cd toolchains && ./mkchain.sh sh-elf
	cd toolchains && ./mkchain.sh rx-elf
