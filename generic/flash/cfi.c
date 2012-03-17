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
#include <string.h>

cfiid_t* cfi_getid() {

	cfiid_t* id = malloc(sizeof(cfiid_t));
	memset(id, 0, sizeof(cfiid_t));

	if (id != NULL) {
		flash_write(QUERYADDRESS, QUERYDATA);

		for (int i = ADDRESS_IDSTRING; i < ADDRESS_SYSTEMINTERFACE; i++) {
			uint8_t byte = flash_read_byte(i);
			printf("0x%02x[%c]\n", byte, byte);

			if (i < 0x13) {
				id->querystring[i - ADDRESS_IDSTRING] = byte;
			}
			else if (i == 0x13) {
				id->prialgid = byte;
			}
			else if (i == 0x14) {
				id->prialgid |= (byte << 8);
			}
			else if (i == 0x15) {
				id->prialgaddress = byte;
			}
			else if (i == 0x16) {
				id->prialgaddress |= (byte << 8);
			}
			else if (i == 0x17) {
				id->altalgid = byte;
			}
			else if (i == 0x18) {
				id->altalgid |= (byte << 8);
			}
			else if (i == 0x19) {
				id->altalgaddress = byte;
			}
			else if (i == 0x1A) {
				id->altalgaddress |= (byte << 8);
			}

		}

		flash_write(0, READARRAY);

	}
	return id;

}

cfisysint_t* cfi_getsysteminterface() {
	cfisysint_t* sysint = malloc(sizeof(cfisysint_t));

	if (sysint != NULL) {
		flash_write(QUERYADDRESS, QUERYDATA);
		for (int i = ADDRESS_SYSTEMINTERFACE; i < ADDRESS_GEOMETRY; i++) {
			uint8_t byte = flash_read_byte(i);
			printf("0x%02x[%c]\n", byte, byte);

		}
		flash_write(0, READARRAY);

	}

	return sysint;
}
cfigeometry_t* cfi_getgeometry() {
	cfigeometry_t* geo = malloc(sizeof(cfigeometry_t));

	if (geo != NULL) {
	}

	return geo;
}

