	;;
	
	.h8300h
	.section .text
	
	.global	VECT_start
	.global	VECT_end
	.global	_bss_start
	.global	_end
	.global	_stack

	.global	_VECT_START
	.global	_VECT_END
	.global	_VVECT_START
	.global	_VVECT_END
	
	.global	_DATAROM_START
	.global	_DATARAM_START
	.global	_DATARAM_END

	.global	_BSS_START
	.global	_BSS_END
	
	.global	_INIT_STACK

_VECT_START:
	.data.l	VECT_start
	
_VECT_END:	
	.data.l	VECT_end

_VVECT_START:
	.data.l	VVECT_start
	
_VVECT_END:	
	.data.l	VVECT_end

_DATAROM_START:
	.data.l	_dataROM_start
	
_DATARAM_START:
	.data.l	_dataRAM_start
	
_DATARAM_END:
	.data.l	_dataRAM_end
	
_BSS_START:
	.data.l	_bss_start
	
_BSS_END:	
	.data.l	_end

_INIT_STACK:
	.data.l	_stack

	.end
	
