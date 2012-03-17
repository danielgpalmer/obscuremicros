/*
 * testharness.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#include "cfi.h"
#include "jedec.h"
#include "flashstubs.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

bool inquerymode = false;
bool inidmode = false;

uint8_t* array;

void flash_write(uint8_t address, uint8_t data) {

	printf("cfi_write(0x%02x, 0x%02x)\n", address, data);

	if (!inquerymode && address == QUERYADDRESS && data == QUERYDATA) {
		printf("Entered query mode\n");
		inquerymode = true;
	}
	else if (!inidmode && data == JEDECIDMODE) {
		printf("Entered id mode\n");
		inidmode = true;
	}
	else if (data == READARRAY) {
		printf("Exited query/idmode\n");
		inquerymode = false;
		inidmode = false;
	}

}

uint8_t flash_read(uint8_t address) {

	printf("cfi_read(0x%02x)\n", address);

	if (inquerymode) {
		return 'A';
	}
	else if (inidmode) {
		switch (address) {
			case 0:
				return 0x89; // Intel...
			case 1:
				return 0x75; // the part I'm working with
			default:
				return 0xFF;
		}
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

	printf("Testing JEDEC ID..\n");
	jedec_getid();

	printf("Testing CFI interface..\n");

	cfiid_t* id = cfi_getid();

}
