/*
 * timer.c
 *
 *  Created on: Jan 30, 2012
 *      Author: daniel
 */

#include "timer.h"
#include "lpc22xx.h"

void timer_reset(unsigned int which) {

	switch (which) {
		case 0:
			T0TCR = TIMER_CONTROL_ENABLE | TIMER_CONTROL_RESET;
			T0IR = 0xFF;
			T0MCR = 0;
			T0CCR = 0;
			T0EMR = 0;
			break;
		case 1:
			T1TCR = TIMER_CONTROL_ENABLE | TIMER_CONTROL_RESET;
			T1IR = 0xFF;
			T1MCR = 0;
			T1CCR = 0;
			T1EMR = 0;
			break;
	}

}

void timer_start(unsigned int which) {

	switch (which) {
		case 0:
			T0TCR &= ~TIMER_CONTROL_RESET;
			break;
		case 1:
			T1TCR &= ~TIMER_CONTROL_RESET;
			break;
	}

}

void timer_dumpregisters() {

	printf("--\n");
	printf("T0IR 0x%08x\n", T0IR);
	printf("T0TC 0x%08x\n", T0TC);
	printf("T0PR 0x%08x\n", T0PR);
	printf("T0PC 0x%08x\n", T0PC);
	printf("T0MR0 0x%08x\n", T0MR0);
	printf("T0MR1 0x%08x\n", T0MR1);
	printf("T0MR2 0x%08x\n", T0MR2);
	printf("T0MR3 0x%08x\n", T0MR3);
	printf("T0MCR 0x%08x\n", T0MCR);
	printf("T0CR0 0x%08x\n", T0CR0);
	printf("T0CR1 0x%08x\n", T0CR1);
	printf("T0CR2 0x%08x\n", T0CR2);
	printf("T0CR3 0x%08x\n", T0CR3);
	printf("T0CCR 0x%08x\n", T0CCR);
}
