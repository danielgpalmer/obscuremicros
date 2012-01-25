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
	} > ram	/* the startup code goes into FLASH */
	
	.text :								/* collect all sections that should go into FLASH after startup  */ 
	{
		*(.text)						/* all .text sections (code)  */
		*(.rodata)						/* all .rodata sections (constants, strings, etc.)  */
		*(.rodata*)						/* all .rodata* sections (constants, strings, etc.)  */
		*(.glue_7)						/* all .glue_7 sections  (no idea what these are) */
		*(.glue_7t)						/* all .glue_7t sections (no idea what these are) */
		_etext = .;						/* define a global symbol _etext just after the last code byte */
	} > ram
	

	

	.data :								/* collect all initialized .data sections that go into RAM  */ 
	{
		_data = .;						/* create a global symbol marking the start of the .data section  */
		*(.data)						/* all .data sections  */
		_edata = .;						/* define a global symbol marking the end of the .data section  */
	} > ram 							/* put all the above into RAM (but load the LMA copy into FLASH) */

	.bss :								/* collect all uninitialized .bss sections that go into RAM  */
	{
		_bss_start = .;					/* define a global symbol marking the start of the .bss section */
		*(.bss)							/* all .bss sections  */
	} > ram								/* put all the above in RAM (it will be cleared in the startup code */

	. = ALIGN(4);						/* advance location counter to the next 32-bit boundary */
	_bss_end = . ;						/* define a global symbol marking the end of the .bss section */
}
	_end = .;							/* define a global symbol marking the end of application RAM */
	end = .;	
