/**/
OUTPUT_FORMAT("elf32-h8300")
OUTPUT_ARCH(h8300h)
ENTRY("_start")
MEMORY
{
	iram(rwx)		: o = 0xFFC610, l = 0x3000 /* internal sram */
	dram(rwx)		: o = 0x400000, l = 0x1FFFFF /* 2Mbyte DRAM between 0x400000 and 0x5fffff */
}

/*PROVIDE (__stack = 0x5FFFFF - 3);*/
PROVIDE (__stack = 0xFFFF00);


SECTIONS
{ 
.text :	{
	*(.text)
	*(.strings)
	*(.rodata)
	*(.rodata.str1.1)
	} > dram

.data : 
	{
        . = ALIGN(0x04);
        *(.data)
        } > dram

.bss : {
        . = ALIGN(0x04);
        _bss_start = . ;
        *(.bss)
        *(COMMON)
	. = ALIGN(0x04);
        _bss_end = . ;
	. = ALIGN(0x04);
	_iram_data = .; 
        } > dram

/* internal ram sections */
.itext :
        AT ( ADDR(.bss) + SIZEOF ( .bss ) )
        {
        . = ALIGN(0x04);
	_iram_image_start = .;
        *(.itext)
	. = ALIGN(0x04);
        } > iram
.idata :
        AT ( ADDR(.bss) + SIZEOF ( .bss ) + SIZEOF (.itext) )
        {
        . = ALIGN(0x04);
        *(.idata)
	. = ALIGN(0x4);
	_iram_image_end = .; /* not sure what to call this, basically the end of static data */
        } > iram
.ibss   :
        AT ( ADDR(.bss) + SIZEOF ( .bss ) + SIZEOF (.itext) + SIZEOF (.idata) )
        {
        . = ALIGN(0x04);
	_iram_bss_start = .;
        *(.ibss)
	. = ALIGN(0x04);
	_iram_bss_end = .;
        } > iram
}
