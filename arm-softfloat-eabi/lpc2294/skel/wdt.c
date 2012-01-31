/*
 * wdt.c
 *
 *  Created on: Jan 31, 2012
 *      Author: daniel
 */

#include "wdt.h"
#include "lpc21xx.h"
#include <stdbool.h>

#define FEEDFIRST 0xAA
#define FEEDSECOND 0x55

void wdt_enable(uint32_t constant, bool reset) {

	WDTC = constant;

	if (reset) {
		WDMOD = WDT_WDMOD_WDEN | WDT_WDMOD_WDRESET;
	}
	else {
		WDMOD = WDT_WDMOD_WDEN;
	}
}

void wdt_feed() {
	WDFEED = FEEDFIRST;
	WDFEED = FEEDSECOND;
}
