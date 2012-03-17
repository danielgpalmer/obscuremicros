/*
 * intel.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#include "intel.h"
#include <stdint.h>

#define MODE_READARRAY 0xFF
#define MODE_READSTATUSREGISTER 0x70

uint8_t intel_readstatusregister() {

	flash_write(0, MODE_READSTATUSREGISTER);
	uint8_t sr = flash_read_byte(0);
	flash_write(0, MODE_READARRAY);
	return sr;
}
