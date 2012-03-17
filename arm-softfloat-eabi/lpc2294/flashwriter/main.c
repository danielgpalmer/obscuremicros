#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "core.h"
#include "interrupts.h"

#include "olimexh2294.h"

// Flash headers
#include "flash/flashstubs.h"
#include "flash/jedec.h"
#include "flash/cfi.h"
#include "flash/flashmfr.h"
//

#define SIZEOFARRAY(array) (sizeof(array)/sizeof(array[0]))

void flash_write(uint8_t address, uint8_t data) {
	printf("flash_write(0x%04x, 0x%04x)\n", address, data);
	*(EXTFLASH + address) = data;

}
uint8_t flash_read_byte(uint8_t address) {
	printf("flash_read_byte(0x%04x)\n", address);
	return *(EXTFLASH + address);
}
uint16_t flash_read_word(uint8_t address) {
	printf("flash_read_word(0x%04x)\n", address);
	return *(EXTFLASH + address);
}

void main() {

	initialize();
	interrupts_reset();

	printf("-- Flash Writer --\n");

	printf("Getting JEDEC ID...");
	jedecid_t* id = jedec_getid(false);
	printf("done\n");
	printf("MFR - 0x%04x, DEVICE - 0x%04x\n", id->mfrid, id->deviceid);
	for (int i = 0; i < SIZEOFARRAY(mfrs); i++) {
		if (mfrs[i].id == id->mfrid) {
			printf("MFR is %s\n", mfrs[i].name);
			break;
		}
	}

	printf("Querying identity via CFI..");
	cfiid_t* cfiid = cfi_getid();
	printf("done\n");
	printf("CFI ID -- %s Primary Alg. ID 0x%04x @ 0x%04x, Alt Alg. ID 0x%04x @ 0x%04x\n", cfiid->querystring,
			cfiid->prialgid, cfiid->prialgaddress, cfiid->altalgid, cfiid->altalgaddress);

	printf("Querying system interface info via CFI..");
	cfi_getsysteminterface();
	printf("done\n");

	printf("Querying  device geometry via CFI..");
	cfi_getgeometry();
	printf("done\n");

	while (1) {
	}
}
