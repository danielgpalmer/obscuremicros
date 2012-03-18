/*
 * intel.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#include "intel.h"
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#define MODE_READARRAY 				0xFF
#define MODE_READSTATUSREGISTER 	0x70
#define MODE_CLEARSTATUS			0x50
#define MODE_BLOCKERASE				0x20
#define MODE_BLOCKERASE_CONFIRM		0xD0

uint8_t intel_readstatusregister() {

	flash_write(0, MODE_READSTATUSREGISTER);
	uint8_t sr = flash_read_byte(0);
	flash_write(0, MODE_READARRAY);
	return sr;
}

bool intel_eraseblock(uint32_t blockaddress) {

	flash_write(blockaddress, MODE_BLOCKERASE);
	flash_write(blockaddress, MODE_BLOCKERASE_CONFIRM);

	uint8_t sr;
	while (!(sr = intel_readstatusregister()) & STATUS_WRITESTATEMACHINESTATUS) {
		printf(".\n");
	}

	printf("SR - 0x%02x\n", sr);

	flash_write(blockaddress, MODE_CLEARSTATUS);

	if (sr & STATUS_ERASESTATUS) {

		if (sr & STATUS_BLOCKLOCKSTATUS) {
			errno = ERROR_ERASEBLOCKLOCKED;
		}
		else {
			errno = 0;
		}

		return false;
	}

	return true;

}
