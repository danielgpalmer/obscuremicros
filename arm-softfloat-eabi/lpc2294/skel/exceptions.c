/**
 * These are default handlers for all of the exceptions that can happen.
 * The ones you wish to handle should replace these by defining them somewhere else.
 * The versions here are all marked as weak so that you can replace them :)
 */

#include <stdio.h>
#include <stdint.h>

#include "exceptions.h"
#include "uart.h"
#include "interrupts.h"

void DATAABORT_Routine(void) {

	register uint32_t *lnk_ptr;

	__asm__ __volatile__ (
			"sub lr, lr, #8\n"
			"mov %0, lr" : "=r" (lnk_ptr)
	);

	printf("Disabling interrupts\n");
	disableInterrupts();

	/* On data abort exception the LR points to PC+8 */
	printf("Data Abort at %p 0x%08X\n", lnk_ptr, *(lnk_ptr));

	while (1)
		; // stop.
}

void PREFETCHABORT_Routine(void) {
	printf("PREFETCHABORT_Routine()\n");
}

void IRQ_Routine(void) {
	printf("IRQ\n");
	while (1)
		;
}

void FIQ_Routine(void) {
	printf("FIQ\n");
	while (1)
		;
}

void SWI_Routine(void) {
	while (1)
		;
}

void UNDEF_Routine(void) {
	printf("UNDEF\n");
	while (1)
		;
}

void VICDefault_Routine(void) {
	printf("VIC Default\n");
	while (1)
		;
}
