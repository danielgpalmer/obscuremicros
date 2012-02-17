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
#include <time.h>
#include <string.h>

#define SIZEOFARRAY(array) (sizeof(array)/sizeof(array[0]))

static bool synced = true;
static int marks = 0;
static uint16_t bits = 0;
static bool timeready = false;

static int minutes, hours, doy, year, dow;

static bool jjy_isleapyear(int year) {
	return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

static int daysinmonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static int jjy_doytomonth(int day, bool leap, int* dayofmonth) {

	int days = 0;
	for (int month = 0; month < SIZEOFARRAY(daysinmonth); month++) {
		if (day < (days + daysinmonth[month])) {
			*dayofmonth = day - days + 1;
			return month;
		}
		days += daysinmonth[month];
		if (month == 2 && leap) { // Handle leap years
			days += 1;
		}
	}

	return -1;
}

static void jjy_mark() {

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
			year = ((bits & 0xF) + (((bits >> 4) & 0xF) * 10)) + 100;
			break;
		case 6:
			dow = (bits >> 6) % 0x7;
			timeready = true;
			break;
		default:
			break;
	}

	bits = 0;

}

void jjy_addsample(unsigned sample) {
	timeready = false;
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
				bits = (bits << 1);
			}
			else if (highs == ONE_ACTIVE && lows == ONE_LOW) {
				bits = (bits << 1) | 1;
			}
			else if (highs == MARK_ACTIVE && lows == MARK_LOW) {
				jjy_mark();
				marks++;
			}
			highs = 0;
			lows = 0;
			samples = 0;
		}
	}
}

struct tm* jjy_gettime() {
	int dom;
	static struct tm now;
	memset(&now, 0, sizeof(now));
	now.tm_sec = 0;
	now.tm_min = minutes;
	now.tm_hour = hours;
	now.tm_mon = jjy_doytomonth(doy, jjy_isleapyear(year + 1900), &dom);
	now.tm_mday = dom;
	now.tm_wday = dow;
	now.tm_yday = doy;
	now.tm_year = year;
	return &now;
}
