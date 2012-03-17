/*
 * jedec.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#include "jedec.h"
#include "flashstubs.h"
#include <stddef.h>

jedecid_t* jedec_getid() {

	jedecid_t* id = malloc(sizeof(jedecid_t));
	if (id != NULL) {
		flash_write(0, JEDECIDMODE);
		id->mfrid = flash_read(0);
		id->deviceid = flash_read(1);
		flash_write(0, READARRAY);
	}
	return id;
}
