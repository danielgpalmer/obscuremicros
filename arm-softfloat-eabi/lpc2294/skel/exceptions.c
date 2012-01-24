#include <stdint.h>

#include "uart.h"
#include "interrupts.h"

void DATAABORT_Routine (void) __attribute__ ((interrupt("ABORT")));

void DATAABORT_Routine (void) {

	register uint32_t *lnk_ptr;

	__asm__ __volatile__ (
	"sub lr, lr, #8\n"
	"mov %0, lr" : "=r" (lnk_ptr)
	);

	printf("Disabling interrupts\n");
	disableInterrupts();

	/* On data abort exception the LR points to PC+8 */
	printf("Data Abort at %p 0x%08X\n", lnk_ptr, *(lnk_ptr));

	while(1); // stop.
}

void PREFETCHABORT_Routine (void) {
        uart0Puts("PREFETCHABORT_Routine()");
}

