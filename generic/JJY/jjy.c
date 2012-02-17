/*
 * jjy.c
 *
 *  Created on: Feb 17, 2012
 *      Author: daniel
 */

#include "jjy.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

static bool synced = true;
static int marks = 0;
static uint16_t bits = 0;

static int minutes, hours, doy, year, dow;

static void jjy_mark() {

	printf("mark 0x%04x\n", bits);

	switch (marks) {
		case 1:
			minutes = (bits & 0xF) + ((bits >> 5) * 10);
			break;
		case 2:
			hours = (bits & 0xF) + ((bits >> 5) * 10);
			break;
		case 3:
			doy = ((bits & 0xF) * 10) + ((bits >> 5) * 100);
			break;
		case 4:
			doy += ((bits >> 5) & 0xF);
			break;
		case 5:
			year = (bits & 0xF) + (((bits >> 4) & 0xF) * 10);
			break;
		case 6:
			dow = (bits >> 6) % 0x7;
			printf("Time is %02d:%02d %02d %02d %02d\n", hours, minutes, doy, year, dow);
			break;
		default:
			break;
	}

	bits = 0;

}

void jjy_addsample(unsigned sample) {
	static unsigned samples = 0;
	static unsigned lows = 0;
	static unsigned highs = 0;

	switch (sample) {
		case 0:
			lows++;
			break;
		case 1:
			highs++;
			break;
		default:
			break;
	}

	samples++;

	if (synced) {
		if (samples == SAMPLESPERSECOND) {
			if (highs == ZERO_ACTIVE && lows == ZERO_LOW) {
				printf("got 0\n");
				bits = (bits << 1);
			}
			else if (highs == ONE_ACTIVE && lows == ONE_LOW) {
				printf("got 1\n");
				bits = (bits << 1) | 1;
			}
			else if (highs == MARK_ACTIVE && lows == MARK_LOW) {
				printf("got mark\n");
				jjy_mark();
				marks++;
			}
			highs = 0;
			lows = 0;
			samples = 0;
		}
	}
}
