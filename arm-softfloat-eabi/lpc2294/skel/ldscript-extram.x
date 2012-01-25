/*	
	This linker script will link everything so that you can load it via JTAG
	into the external SRAM and execute it from there. The stack is in the 
	internal SRAM for performance. The advantages of using the external SRAM
	is that it's quicker to write than the flash(internal and external), it's
	bigger than the internal SRAM (so you can actually load something that uses
	libc stuff) and because SRAM is writable you can have as many software
	breakpoints as you want. 
	
	You must use the crt-ram.s startup code with this linker script for this to
	work correctly!
*/

ENTRY(_startup)

MEMORY 
{
	flash     			: ORIGIN = 0,          LENGTH = 256K	/* FLASH ROM                            	*/	
	ram_isp_low(A)		: ORIGIN = 0x40000120, LENGTH = 223		/* variables used by Philips ISP bootloader	*/		 
	ram					: ORIGIN = 0x40000200, LENGTH = 15840	/* free RAM area							*/
	ram_isp_high(A)		: ORIGIN = 0x40001FE0, LENGTH = 32		/* variables used by Philips ISP bootloader	*/
	extram				: ORIGIN = 0x81000000, LENGTH = 1024K	/* External SRAM */
}

_stack_end = 0x40001EDC;

SECTIONS 
{
	startup : {
		*(.startup)
	} > extram							
	
	.text :								/* collect all sections that should go into FLASH after startup  */ 
	{
		*(.text)						/* all .text sections (code)  */
		*(.rodata)						/* all .rodata sections (constants, strings, etc.)  */
		*(.rodata*)						/* all .rodata* sections (constants, strings, etc.)  */
		*(.glue_7)						/* all .glue_7 sections  (no idea what these are) */
		*(.glue_7t)						/* all .glue_7t sections (no idea what these are) */
		_etext = .;						/* define a global symbol _etext just after the last code byte */
	} > extram
	
	.data :								/* collect all initialized .data sections that go into RAM  */ 
	{
		_data = .;						/* create a global symbol marking the start of the .data section  */
		*(.data)						/* all .data sections  */
		_edata = .;						/* define a global symbol marking the end of the .data section  */
	} > extram 							/* put all the above into RAM (but load the LMA copy into FLASH) */

	.bss :								/* collect all uninitialized .bss sections that go into RAM  */
	{
		_bss_start = .;					/* define a global symbol marking the start of the .bss section */
		*(.bss)							/* all .bss sections  */
	} > extram								/* put all the above in RAM (it will be cleared in the startup code */

	. = ALIGN(4);						/* advance location counter to the next 32-bit boundary */
	_bss_end = . ;						/* define a global symbol marking the end of the .bss section */
}
	_end = .;							/* define a global symbol marking the end of application RAM */
	end = .;	
