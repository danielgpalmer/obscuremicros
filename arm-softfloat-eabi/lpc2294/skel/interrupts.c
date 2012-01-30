/*
 * interrupts.c
 *
 *  Created on: 2 May 2011
 *      Author: daniel
 */

#include <stdio.h>
#include "lpc22xx.h"
#include "exceptions.h"

#define IRQ_MASK 0x00000080
#define FIQ_MASK 0x00000040

static inline unsigned asm_get_cpsr(void) {
	unsigned long retval;
	asm volatile (" mrs  %0, cpsr" : "=r" (retval) : /* no inputs */);
	return retval;
}

static inline void asm_set_cpsr(unsigned val) {
	asm volatile (" msr  cpsr, %0" : /* no outputs */: "r" (val) );
}

unsigned enableFIQ(void) {
	unsigned _cpsr;

	_cpsr = asm_get_cpsr();
	asm_set_cpsr(_cpsr & ~FIQ_MASK);
	return _cpsr;
}

unsigned enableIRQ(void) {
	unsigned _cpsr;

	_cpsr = asm_get_cpsr();
	asm_set_cpsr(_cpsr & ~IRQ_MASK);
	return _cpsr;
}

void disableInterrupts(void) {
	unsigned _cpsr;
	_cpsr = asm_get_cpsr();
	asm_set_cpsr(_cpsr | (FIQ_MASK | IRQ_MASK));
}

void interrupts_reset() {
	VICDefVectAddr = (uint32_t) VICDefault_Routine;
	VICIntSelect = 0;
	VICIntEnClr = 0xFFFF;
}

void dumpInterruptRegisters(void) {
	printf("--\n");
	printf("CPSR 0x%08x\n", asm_get_cpsr());
	printf("VICIRQStatus	0x%04x\n", VICIRQStatus);
	printf("VICFIQStatus	0x%04x\n", VICFIQStatus);
	printf("VICRawIntr		0x%04x\n", VICRawIntr);
	printf("VICIntSelect	0x%04x\n", VICIntSelect);
	printf("VICIntEnable	0x%04x\n", VICIntEnable);
	printf("VICVectAddr		0x%08x\n", VICVectAddr);

	printf("VICVectCntrl0 0x%08x\n", VICVectCntl0);
	printf("VICVectAddr0 0x%08x\n", VICVectAddr0);
	printf("VICVectCntrl1 0x%08x\n", VICVectCntl1);
	printf("VICVectAddr1 0x%08x\n", VICVectAddr1);
}
