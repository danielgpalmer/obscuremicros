/*
 * cfi.c
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#ifndef FLASH_DONTWANT_CFI

#include "cfi.h"
#include "flashstubs.h"
#include "common.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

cfiid_t* cfi_getid() {

	cfiid_t* id = malloc(sizeof(cfiid_t));

	if (id != NULL) {
		memset(id, 0, sizeof(cfiid_t));
		cfi_writebyte(QUERYADDRESS, QUERYDATA);
		for (int i = ADDRESS_IDSTRING; i < ADDRESS_SYSTEMINTERFACE; i++) {
			uint8_t byte = cfi_readbyte(i);

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
		cfi_writebyte(0, READARRAY);
	}
	return id;

}

cfisysint_t* cfi_getsysteminterface() {
	cfisysint_t* sysint = malloc(sizeof(cfisysint_t));
	if (sysint != NULL) {
		memset(sysint, 0, sizeof(cfisysint_t));
		cfi_writebyte(QUERYADDRESS, QUERYDATA);
		for (int i = ADDRESS_SYSTEMINTERFACE; i < ADDRESS_GEOMETRY; i++) {
			uint8_t byte = cfi_readbyte(i);
		}
		cfi_writebyte(0, READARRAY);

	}

	return sysint;
}
cfigeometry_t* cfi_getgeometry() {
	cfigeometry_t* geo = malloc(sizeof(cfigeometry_t));

	if (geo != NULL) {
		memset(geo, 0, sizeof(cfigeometry_t));
		cfi_writebyte(QUERYADDRESS, QUERYDATA);
		for (int i = ADDRESS_GEOMETRY; i < ADDRESS_GEOMETRY_ERASEBINFO; i++) {
			uint8_t byte = cfi_readbyte(i);
			switch (i) {
				case GEOMETRY_SIZE:
					geo->bytes = pow(2, byte);
					break;
				case GEOMETRY_FDICD_LOW:
					geo->flashdeviceinterfacecode = byte;
					break;
				case GEOMETRY_FDICD_HIGH:
					geo->flashdeviceinterfacecode |= (byte << 8);
					break;
				case GEOMETRY_MULTIBYTE_LOW:
					geo->multibytemax = byte;
					break;
				case GEOMETRY_MUTLIBYTE_HIGH:
					geo->multibytemax |= (byte << 8);
					geo->multibytemax = pow(2, geo->multibytemax);
					break;
				case GEOMERTY_ERASEREGIONS:
					geo->eraseblockregions = byte;
					break;
			}

		}

		geo->eraseblockinfo = malloc(sizeof(cfieraseblockinfo_t) * geo->eraseblockregions);
		memset(geo->eraseblockinfo, 0, sizeof(cfieraseblockinfo_t) * geo->eraseblockregions);
		for (int eraseblock = 0; eraseblock < geo->eraseblockregions; eraseblock++) {
			for (int i = 0; i < 4; i++) {
				cfieraseblockinfo_t* blockinfo = &(geo->eraseblockinfo[eraseblock]);
				uint8_t byte = cfi_readbyte(ADDRESS_GEOMETRY_ERASEBINFO + (eraseblock * 4) + i);
				switch (i) {
					case 0:
						blockinfo->numblocks = byte;
						break;
					case 1:
						blockinfo->numblocks |= (byte << 8);
						blockinfo->numblocks += 1;
						break;
					case 2:
						blockinfo->blocksize = byte;
						break;
					case 3:
						blockinfo->blocksize |= (byte << 8);
						if (blockinfo->blocksize == 0) {
							blockinfo->blocksize = 128;
						}
						else {
							blockinfo->blocksize *= 256;
						}
						break;
				}
			}
		}

		cfi_writebyte(0, READARRAY);
	}

	return geo;
}

void cfi_freegeometry(cfigeometry_t* geo) {
	free(geo->eraseblockinfo);
	free(geo);
}

#endif
