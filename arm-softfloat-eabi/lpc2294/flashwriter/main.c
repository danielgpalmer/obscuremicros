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

void flash_write_byte(uint32_t address, uint8_t data) {
	//printf("flash_write(0x%04x, 0x%04x)\n", address, data);
	*(((uint8_t*) (EXTFLASH)) + address) = data;
}
void flash_write_word(uint32_t address, uint16_t data) {
	//printf("flash_write(0x%04x, 0x%04x)\n", address, data);

	uint32_t offset = address;
	if (offset) {
		offset &= 0xFFFFFFFFFE;
		if (offset) {
			offset /= 2;
		}
	}
	*(EXTFLASH + offset) = data;
}
uint8_t flash_read_byte(uint32_t address) {
	//printf("flash_read_byte(0x%04x) - 0x%02x\n", address, *(EXTFLASH + address));
	return *(((uint8_t*) EXTFLASH) + address);
}
uint16_t flash_read_word(uint32_t address) {
	//printf("flash_read_word(0x%04x)\n", address);
	uint32_t offset = address;
	if (offset) {
		offset &= 0xFFFFFFFFFE;
		if (offset) {
			offset /= 2;
		}
	}
	return *(EXTFLASH + offset);
}

static void getlockstatus() {
	printf("Getting block lock status for first block...");
	switch (intel_getlockstatus(0)) {
		case unlocked:
			printf("unlocked\n");
			break;
		case locked:
			printf("locked\n");
			break;
		case lockeddown:
			printf("lockeddown\n");
			break;
	}
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

	printf("Getting protection register...");
	intelprotectionregister_t* pr = intel_getprotectionregister();
	printf("done");
	printf("Protection register factory portion: 0x%016llx, user portion: 0x%016llx\n", pr->factory, pr->user);

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
	printf("Size: %d bytes, Interface code: %d, Max bytes in multi byte program %d,Erase block regions %d\n",
			geo->bytes, geo->flashdeviceinterfacecode, geo->multibytemax, geo->eraseblockregions);

	for (int eraseblock = 0; eraseblock < geo->eraseblockregions; eraseblock++) {
		cfieraseblockinfo_t* blockinfo = &(geo->eraseblockinfo[eraseblock]);
		printf("Erase block region %d has %d blocks of %d bytes\n", eraseblock, blockinfo->numblocks,
				blockinfo->blocksize);
	}
/*
	printf("Using intel driver to unlock first block...");
	intel_unlockblock(0);
	printf("done\n");

	getlockstatus();

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
	else {
		printf("done\n");
	}*/
	/*
	 printf("Writing some data to the the first block...");

	 //for (int i = 0; i < 0xff; i++) {
	 intel_writeword(0, 0xAA55);
	 intel_writeword(2, 0x55AA);
	 //}
	 printf("done\n");*/

	for (int i = 0; i < 0xff; i++) {
		if (i % 0xf == 0) {
			printf("\n");
		}
		printf("0x%02x ", flash_read_byte(i));
	}
	printf("\n");

	while (1) {
	}
}
