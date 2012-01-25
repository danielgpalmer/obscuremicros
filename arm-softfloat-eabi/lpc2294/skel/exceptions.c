#include <stdio.h>
#include <stdint.h>

#include "uart.h"
#include "interrupts.h"

void DATAABORT_Routine(void) __attribute__ ((interrupt("ABORT")));
void IRQ_Routine(void) __attribute__ ((interrupt("IRQ")));
void FIQ_Routine(void) __attribute__ ((interrupt("FIQ")));
void SWI_Routine(void) __attribute__ ((interrupt("SWI")));
void UNDEF_Routine(void) __attribute__ ((interrupt("UNDEF")));

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
	while (1)
		;
}

void FIQ_Routine(void) {
	while (1)
		;
}

void SWI_Routine(void) {
	while (1)
		;
}

void UNDEF_Routine(void) {
	while (1)
		;
}
