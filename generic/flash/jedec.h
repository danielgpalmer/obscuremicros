/*
 * jedec.h
 *
 *  Created on: Mar 16, 2012
 *      Author: daniel
 */

#ifndef JEDEC_H_
#define JEDEC_H_

#include <stdint.h>

#define JEDECIDMODE 0x90
#define READARRAY 0xFF

typedef struct {
	uint16_t mfrid;
	uint16_t deviceid;
} jedecid_t;

jedecid_t* jedec_getid();

#endif /* JEDEC_H_ */
