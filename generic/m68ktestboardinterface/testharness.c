/*
 * testharness.c
 *
 *  Created on: Mar 7, 2012
 *      Author: daniel
 */

#include "m68kinterface.h"
#include "stubs.h"

#include <stdio.h>
#include <stdint.h>

static uint32_t address;
static uint16_t dataout, datain;

void getpins(pinsin_t* pinsin) {

}

void setpins(pinsout_t* pinsout) {

}

int main(int argc, char* argv[]) {

	printf("Testing a read cycle\n");

	printf("Testing a write cycle\n");

}
