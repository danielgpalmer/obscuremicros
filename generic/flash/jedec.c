/*
 * jedec.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#ifndef FLASH_DONTWANT_JEDEC

#include "jedec.h"
#include "flashstubs.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "common.h"

jedecid_t* jedec_getid(bool eightbitbus) {

	jedecid_t* id = malloc(sizeof(jedecid_t));
	if (id != NULL) {
		flash_write_byte(0, JEDECIDMODE);
		if (eightbitbus) {
			id->mfrid = cfi_readbyte(0);
			id->deviceid = cfi_readbyte(1);
		}
		else {
			id->mfrid = cfi_readword(0);
			id->deviceid = cfi_readword(1);
		}
		flash_write_byte(0, READARRAY);
	}
	else {
		printf("\n\n*** couldn't allocate memory for struct ***\n\n");
	}
	return id;
}

#endif
