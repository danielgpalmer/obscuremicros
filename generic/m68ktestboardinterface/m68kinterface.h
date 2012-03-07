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
	IDLE, READ, WRITE, ENDTRANSCTION
} interfacestate_t;

typedef struct {
	bool addressstrobe;
	uint8_t function;
	bool readnotwrite;
	bool upperdatastrobe;
	bool lowerdatastrobe;
} pinsin_t;

typedef struct {
	bool dtack;
	bool reset;
	bool halt;
	bool busreq;
	uint8_t ipl;
} pinsout_t;

#endif /* M68KINTERFACE_H_ */
