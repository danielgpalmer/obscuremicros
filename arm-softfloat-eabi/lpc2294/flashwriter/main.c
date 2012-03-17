#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "interrupts.h"

// Flash headers
#include "flash/flashstubs.h"
#include "flash/jedec.h"
#include "flash/cfi.h"
//

void flash_write(uint8_t address, uint8_t data) {
	printf("flash_write(0x%04x, 0x%04x)\n", address, data);

}
uint8_t flash_read(uint8_t address) {
	printf("flash_read(0x%04x)\n", address);
	return 0x00;
}

void main() {

	initialize();
	interrupts_reset();

	printf("-- Flash Writer --\n");

	printf("Getting JEDEC ID...");
	jedecid_t* id = jedec_getid();
	printf("done\n");
	printf("MFR - 0x%04x, DEVICE - 0x%04x\n", id->mfrid, id->deviceid);

	while (1) {
	}
}
