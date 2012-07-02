/*OUTPUT_FORMAT("coff-h8300")*/
OUTPUT_FORMAT("elf32-h8300")
OUTPUT_ARCH(h8300h)
ENTRY("_start")
MEMORY
{
	vectors : o = 0x000000, l = 0x100
	func    : o = 0x000100, l = 0x100
	rom     : o = 0x000200, l = 0xbe00
	ram     : o = 0xffbf20, l = 0x04000
}

SECTIONS
{
.vect : {
	VECT_start = .;
	*(.vect)
        }  > vectors
	VECT_end = .;
.monfunc : {
	LONG(ABSOLUTE(_sci1_putci))
	LONG(ABSOLUTE(_sci1_getc))
	LONG(ABSOLUTE(_sci1_putc))
	LONG(ABSOLUTE(_set_break_func))
	} > func
.text :	{
	*(.text.startup)
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
	} > ram
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
	}  >ram
	_stack = . ; 
	_APR_stack = ADDR(.bss) + SIZEOF(.bss);
}
