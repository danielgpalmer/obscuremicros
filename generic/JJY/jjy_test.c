/*
 * jjy_test.c
 *
 *  Created on: Feb 17, 2012
 *      Author: daniel
 */

#include "jjy.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static int bitssent = 0;
static uint16_t bitchain = 0;

static void jjy_zero() {

	for (int sample = 0; sample < ZERO_ACTIVE; sample++) {
		jjy_addsample(1);
	}
	for (int sample = 0; sample < ZERO_LOW; sample++) {
		jjy_addsample(0);
	}

	bitchain = (bitchain << 1);
	bitssent++;

}

static void jjy_one() {

	for (int sample = 0; sample < ONE_ACTIVE; sample++) {
		jjy_addsample(1);
	}
	for (int sample = 0; sample < ONE_LOW; sample++) {
		jjy_addsample(0);
	}

	bitchain = (bitchain << 1) | 1;
	bitssent++;
}

static void jjy_mark() {

	for (int sample = 0; sample < MARK_ACTIVE; sample++) {
		jjy_addsample(1);
	}
	for (int sample = 0; sample < MARK_LOW; sample++) {
		jjy_addsample(0);
	}

	bitchain = 0;
	bitssent++;

}

static void jjy_callsignzero() {
	for (int sample = 0; sample < SAMPLESPERSECOND; sample++) {
		jjy_addsample(0);
	}
}

static void jjy_callsignone() {
	for (int sample = 0; sample < SAMPLESPERSECOND; sample++) {
		jjy_addsample(1);
	}
}

#define BITFORVALUE(TOTAL,VALUE)	if (TOTAL >= VALUE) { \
										TOTAL -= VALUE;\
										jjy_one(); \
									} \
									else { \
										jjy_zero(); \
									}

static bool evenparity(uint16_t data) {
	int evenbits = 0;
	for (int i = 0; i < (sizeof(data) * 8); i++) {
		evenbits += (data >> i) & 1;
	}
	return (evenbits & 1);
}

static void sendtime(struct tm *ltime) {

	bool hourpar, minutepar;

	bitssent = 0;
	ltime->tm_sec = 0;
	printf("Telling the JJY code that that the time is %s", asctime(ltime));

	// minutes
	int minutes = ltime->tm_min;
	jjy_mark();
	BITFORVALUE(minutes, 40);
	BITFORVALUE(minutes, 20);
	BITFORVALUE(minutes, 10);
	jjy_zero();
	BITFORVALUE(minutes, 8);
	BITFORVALUE(minutes, 4);
	BITFORVALUE(minutes, 2);
	BITFORVALUE(minutes, 1);
	minutepar = evenparity(bitchain);

	// hours
	int hours = ltime->tm_hour;
	jjy_mark();
	jjy_zero();
	jjy_zero();
	BITFORVALUE(hours, 20);
	BITFORVALUE(hours, 10);
	jjy_zero();
	BITFORVALUE(hours, 8);
	BITFORVALUE(hours, 4);
	BITFORVALUE(hours, 2);
	BITFORVALUE(hours, 1);
	hourpar = evenparity(bitchain);
	// day of year
	int doy = ltime->tm_yday;
	jjy_mark();
	jjy_zero();
	jjy_zero();
	BITFORVALUE(doy, 200);
	BITFORVALUE(doy, 100);
	jjy_zero();
	BITFORVALUE(doy, 80);
	BITFORVALUE(doy, 40);
	BITFORVALUE(doy, 20);
	BITFORVALUE(doy, 10);
	jjy_mark();
	BITFORVALUE(doy, 8);
	BITFORVALUE(doy, 4);
	BITFORVALUE(doy, 2);
	BITFORVALUE(doy, 1);
	jjy_zero();
	jjy_zero();

	// parity
	if (hourpar) {
		jjy_one();
	}
	else {
		jjy_zero();
	}
	if (minutepar) {
		jjy_one();
	}
	else {
		jjy_zero();
	}

	// change to summer time
	jjy_zero();
	jjy_mark();

	// year
	if (ltime->tm_min == 15 || ltime->tm_min == 45) {
		// Have no idea what this looks like yet..
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
	}
	else {
		// summer time in effect
		jjy_zero();
		int year = ltime->tm_year % 100;
		BITFORVALUE(year, 80);
		BITFORVALUE(year, 40);
		BITFORVALUE(year, 20);
		BITFORVALUE(year, 10);
		BITFORVALUE(year, 8);
		BITFORVALUE(year, 4);
		BITFORVALUE(year, 2);
		BITFORVALUE(year, 1);
	}

	jjy_mark();
	// day of week
	if (ltime->tm_min == 15 || ltime->tm_min == 45) {
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
	}
	else {
		int dow = ltime->tm_wday;
		BITFORVALUE(dow, 4);
		BITFORVALUE(dow, 2);
		BITFORVALUE(dow, 1);

		// leap second
		jjy_zero();
		jjy_zero();

		//end
		jjy_zero();
		jjy_zero();
		jjy_zero();
		jjy_zero();
	}
	jjy_mark();

	printf("Done, %d bits sent\n", bitssent);

	printf("According to JJY code the time is %s", asctime(jjy_gettime()));
}

int main(int argc, char *argv[]) {

	time_t now = time(NULL);
	struct tm *ltime = localtime(&now);

	printf("-- Normal tests --\n");
	sendtime(ltime);

	printf("-- Quarter past alt test --\n");
	ltime->tm_min = 15;
	sendtime(ltime);

	printf("-- Quarter to alt test -- \n");
	ltime->tm_min = 45;
	sendtime(ltime);

	//printf("-- Stress tests --\n");
	//
	//for (int i = 0; i < 160; i++) {
	//	int random = (rand() % 3);
	//	switch (random) {
	//		case 0:
	//			jjy_one();
	//			break;
	//		case 1:
	//			jjy_zero();
	//			break;
	//		case 2:
	//			jjy_mark();
	//			break;
	//	}
	//}

	//if (jjy_gettime() != NULL) {
	//	printf("FAILED: time should be null");
	//}

}

