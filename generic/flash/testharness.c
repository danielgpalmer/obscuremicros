/*
 * testharness.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#include "cfi.h"
#include "cfistubs.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool inquerymode = false;

uint8_t* array;

void cfi_write(uint8_t address, uint8_t data) {

	printf("cfi_write(0x%02x, 0x%02x)\n", address, data);

	if (!inquerymode && address == QUERYADDRESS && data == QUERYDATA) {
		printf("Entered query mode\n");
		inquerymode = true;
	}
	else if (inquerymode && data == READARRAY) {
		printf("Exited query mode\n");
		inquerymode = false;
	}

}

uint8_t cfi_read(uint8_t address) {

	printf("cfi_read(0x%02x)\n", address);

	if (inquerymode) {
		return 'A';
	}
	else {
		return *(array + address);
	}

}

int main() {

	printf("Allocating shitty pretend array\n");
	array = malloc(262144); //2mbit baby
	if (array == NULL) {
		printf("Couldn't allocate array!!\n");
		return 1;
	}

	printf("Testing CFI interface..\n");

	cfiid_t* id = cfi_getid();

}
