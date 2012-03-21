/*
 * flashmfr.h
 *
 *  Created on: Mar 18, 2012
 *      Author: daniel
 */

#ifndef FLASHMFR_H_
#define FLASHMFR_H_

typedef struct {
	uint16_t id;
	char* name;
} flashmfr_t;

static flashmfr_t mfrs[] = { { 0x0089, "Intel" } };

#endif /* FLASHMFR_H_ */
