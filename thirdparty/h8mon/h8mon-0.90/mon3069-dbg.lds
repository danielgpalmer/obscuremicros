/*OUTPUT_FORMAT("coff-h8300")*/
OUTPUT_FORMAT("elf32-h8300")
OUTPUT_ARCH(h8300h)
ENTRY("_start")
MEMORY
{
	rom     : o = 0x480000, l = 0xbe00
/*	ram     : o = 0xffc020, l = 0x03f00*/
}

SECTIONS
{
.vect : {
	VECT_start = .;
	*(.vect)
        }  > vectors
	VECT_end = .;
.text :	{
	*(.text)
	*(.strings)
	*(.rodata)
	*(.rodata.str1.1)
	_dataROM_start = . ; 
	} > rom
.data : AT (_dataROM_start) {
	_dataRAM_start = . ; 
	*(.data)
	_dataRAM_end = . ; 
	} > rom
.bss : {
	VVECT_start = .;
	*(.vvect)
	VVECT_end = .;
	_bss_start = .;
	*(.bss)
	*(COMMON)
	_bss_end = . ;  
	_end = .;
	*(.stack)
	}  >rom
	_stack = . ; 
	_APR_stack = ADDR(.bss) + SIZEOF(.bss);
}
