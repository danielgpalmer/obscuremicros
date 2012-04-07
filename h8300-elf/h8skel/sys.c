/*

 */

#include <stdint.h>
#include <stdbool.h>
#include "io3069.h"
#include "sys.h"
#include "monitor.h"

void sysEnableChipSelect(uint8_t csn) {
	switch (csn) {
		case 1:
		case 2:
		case 3:
			P8DDR |= (0x08 >> csn);
			break;
			// CS4 - CS7 are controlled by the CSCR register
		case 4:
		case 5:
		case 6:
		case 7:
			CSCR |= (0x01 << csn);
			break;

		default:
			break;
	}
}

void sysDisableChipSelect(uint8_t csn) {
	uint8_t mask = 0;
	switch (csn) {
		case 1:
		case 2:
		case 3:
			mask = 0xF7 >> csn;
			P8DDR &= mask;
			break;

		case 4:
		case 5:
		case 6:
		case 7:
			mask = 0xFE << csn;
			CSCR &= mask;
			break;

		default:
			break;
	}
}

void sysEnableDigitalToAnalogueConverter(uint8_t dan) {
	switch (dan) {
		case 0:
			DACR |= 0x60;
			break;
		case 1:
			DACR |= 0xA0;
			break;
		default:
			break;
	}
}

void sysDisableDigitalToAnalogueConverter(uint8_t dan) {
	switch (dan) {
		case 0:
			DACR &= 0xBF;
			break;
		case 1:
			DACR &= 0x7F;
			break;
		default:
			break;
	}
	if ((DACR & 0xC0) == 0) {
		DACR = 0x00;
	}
}

void sysSetupEightBitTimer(uint8_t tn) {

}

void sysSetEightBitTimerClockSource(uint8_t tn, uint8_t sourcemask) {
	switch (tn) {
		case 0:
			TCR0_8 &= EIGHTBITCLOCKCLEAR;
			TCR0_8 |= sourcemask;
			break;
		case 1:
			TCR1_8 &= EIGHTBITCLOCKCLEAR;
			TCR1_8 |= sourcemask;
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}
}

void sysSetEightBitTimerInterrupts(uint8_t tn, uint8_t intmask) {
	switch (tn) {
		case 0:
			TCR0_8 &= EIGHTBITINTCLEAR;
			TCR0_8 |= intmask;
			break;
		case 1:
			TCR1_8 &= EIGHTBITINTCLEAR;
			TCR1_8 |= intmask;
			break;
		case 2:
			break;
		case 3:
			break;
		default:
			break;
	}
}

void sysSetupSixteenBitTimer(uint8_t cn) {

}

void sysStartSixteenBitTimer(uint8_t cn) {
	switch (cn) {
		case 0:
			TSTR |= 0x01;
			break;
		case 1:
			TSTR |= 0x02;
			break;
		case 2:
			TSTR |= 0x04;
			break;
		default:
			break;
	}
}

void sysStopSixteenBitTimer(uint8_t cn) {
	switch (cn) {
		case 0:
			TSTR &= 0xFE;
			break;
		case 1:
			TSTR &= 0xFD;
			break;
		case 2:
			TSTR &= 0xFB;
			break;
		default:
			break;
	}
}

/** Enables the WAITE flag in the BCR (Bus Control Register) which causes the processor 
 to pause during memory access until the WAIT pin is logic 1.
 */

void sysEnableWaite() {
	// See Page 122 of the H8/3069RF Hardware Manual(Rev 5.0)
	BCR |= 0x01; // enable the WAITE flag in the bus control register
}

void sysDisableWaite() {
	BCR &= 0xFE; // disable the WAITE flag in the bus control register
}

void sysEnableClockOutput() {
	// See Page 699 of the H8/3069RF Hardware Manual(Rev 5.0)
	MSTCRH &= 0xEF;
}

void sysDisableClockOutput() {
	MSTCRH |= 0x80;
}

void sysDRAMRASUp() {
	DRCRA &= 0xFB;
}

/** Returns the status of the UI bit of the CCR register
 @return true if the UI bit is set, false if it is clear
 */bool sysIsUISet() {
	uint16_t ccrval = 0;
	asm( "stc.w ccr, %0\n\t"
			: : "m"(ccrval));

	if ((ccrval & 0x0040) == 0x0040) {
		return true;
	}
	else {
		return false;
	}
}
