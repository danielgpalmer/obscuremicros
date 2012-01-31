/*
 * wdt.h
 *
 *  Created on: Jan 31, 2012
 *      Author: daniel
 */

#ifndef WDT_H_
#define WDT_H_

#include <stdint.h>
#include <stdbool.h>

void wdt_enable(uint32_t constant, bool reset);
void wdt_feed();

#define WDT_WDMOD_WDEN		1
#define WDT_WDMOD_WDRESET	(1 << 1)
#define WDT_WDMOD_WDTOF		(1 << 2)
#define WDT_WDMOD_WDINT		(1 << 3)
#endif /* WDT_H_ */
