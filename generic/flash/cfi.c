/*
 * cfi.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#include "cfi.h"
#include "cfistubs.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

void cfi_init() {

}

cfiid_t* cfi_getid() {

	cfiid_t* id = malloc(sizeof(id));

	if (id != NULL) {
		cfi_write(QUERYADDRESS, QUERYDATA);

		for (int i = ADDRESS_IDSTRING; i < ADDRESS_SYSTEMINTERFACE; i++) {
			cfi_read(i);
		}

		cfi_write(0, READARRAY);

	}
	return id;

}

cfigeometry_t* cfi_getgeometry() {
	cfigeometry_t* geo = malloc(sizeof(geo));

	if (geo != NULL) {

	}

	return geo;
}

cfisysint_t* cfi_getsysteminterface() {
	cfisysint_t* sysint = malloc(sizeof(sysint));

	if (sysint != NULL) {

	}

	return sysint;
}
