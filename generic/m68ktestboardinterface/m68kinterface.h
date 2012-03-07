/*
 * m68kinterface.h
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#ifndef M68KINTERFACE_H_
#define M68KINTERFACE_H_

#include <stdbool.h>
#include <stdint.h>

void m68kint_clock();

typedef enum {
	IDLE
} interfacestate_t;

typedef struct {
	uint8_t function;
	bool readnotwrite;
} pinsin_t;

typedef struct {
	bool reset;
	bool halt;
	bool busreq;
	uint8_t ipl;
} pinsout_t;

#endif /* M68KINTERFACE_H_ */
