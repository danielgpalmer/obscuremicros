#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>

#include "core.h"
#include "interrupts.h"

#include "olimexh2294.h"

// Flash headers
#include "flash/flashstubs.h"
#include "flash/jedec.h"
#include "flash/cfi.h"
#include "flash/flashmfr.h"
#include "flash/intel.h"
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
	cfigeometry_t* geo = cfi_getgeometry();
	printf("done\n");
	printf("Size: %d bytes, Interface code: %d, Max bytes in multi byte program %d,Erase blocks %d\n", geo->bytes,
			geo->flashdeviceinterfacecode, geo->multibytemax, geo->eraseblockregions);

	for (int eraseblock = 0; eraseblock < geo->eraseblockregions; eraseblock++) {
		cfieraseblockinfo_t* blockinfo = &(geo->eraseblockinfo[eraseblock]);
		printf("Erase block region %d has %d blocks of %d bytes\n", eraseblock, blockinfo->numblocks,
				blockinfo->blocksize);
	}

	printf("Getting status register from CUI..");
	uint8_t sr = intel_readstatusregister();
	printf("done\n");

	printf("Status register is 0x%02x\n", sr);

	printf("Using intel driver to erase first block...");
	if (!intel_eraseblock(0)) {
		printf("Erase Error - ");
		switch (errno) {
			case ERROR_ERASEBLOCKLOCKED:
			printf("block is locked\n");
			break;
		default:
			printf("unknown error\n");
			break;
		}

	}
	printf("done\n");

	while (1) {
	}
}
