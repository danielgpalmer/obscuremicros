/*
 * cfi.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#include "cfi.h"
#include "flashstubs.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

void cfi_init() {

}

cfiid_t* cfi_getid() {

	cfiid_t* id = malloc(sizeof(cfiid_t));

	if (id != NULL) {
		flash_write(QUERYADDRESS, QUERYDATA);

		for (int i = ADDRESS_IDSTRING; i < ADDRESS_SYSTEMINTERFACE; i++) {
			flash_read(i);
		}

		flash_write(0, READARRAY);

	}
	return id;

}

cfigeometry_t* cfi_getgeometry() {
	cfigeometry_t* geo = malloc(sizeof(cfigeometry_t));

	if (geo != NULL) {

	}

	return geo;
}

cfisysint_t* cfi_getsysteminterface() {
	cfisysint_t* sysint = malloc(sizeof(cfisysint_t));

	if (sysint != NULL) {

	}

	return sysint;
}
