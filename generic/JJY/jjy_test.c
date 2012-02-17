/*
 * jjy_test.c
 *
 *  Created on: Feb 17, 2012
 *      Author: daniel
 */

#include "jjy.h"

#include <time.h>
#include <stdio.h>

static int bitssent = 0;

static void jjy_zero() {

	for (int sample = 0; sample < ZERO_ACTIVE; sample++) {
		jjy_addsample(1);
	}
	for (int sample = 0; sample < ZERO_LOW; sample++) {
		jjy_addsample(0);
	}

	bitssent++;

}

static void jjy_one() {

	for (int sample = 0; sample < ONE_ACTIVE; sample++) {
		jjy_addsample(1);
	}
	for (int sample = 0; sample < ONE_LOW; sample++) {
		jjy_addsample(0);
	}

	bitssent++;
}

static void jjy_mark() {

	for (int sample = 0; sample < MARK_ACTIVE; sample++) {
		jjy_addsample(1);
	}
	for (int sample = 0; sample < MARK_LOW; sample++) {
		jjy_addsample(0);
	}

	bitssent++;

}

#define BITFORVALUE(TOTAL,VALUE)	if (TOTAL >= VALUE) { \
										TOTAL -= VALUE;\
										jjy_one(); \
									} \
									else { \
										jjy_zero(); \
									}

int main(int argc, char *argv[]) {

	time_t now = time(NULL);
	struct tm *ltime = localtime(&now);
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
	jjy_zero();
	jjy_zero();

	// change to summer time
	jjy_zero();
	jjy_mark();

	// summer time in effect
	jjy_zero();

	// year
	int year = ltime->tm_year % 100;
	BITFORVALUE(year, 80);
	BITFORVALUE(year, 40);
	BITFORVALUE(year, 20);
	BITFORVALUE(year, 10);
	BITFORVALUE(year, 8);
	BITFORVALUE(year, 4);
	BITFORVALUE(year, 2);
	BITFORVALUE(year, 1);

	// day of week
	int dow = ltime->tm_wday;
	jjy_mark();
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
	jjy_mark();

	printf("Done, %d bits sent\n", bitssent);

	printf("According to JJY code the time is %s", asctime(jjy_gettime()));

}

