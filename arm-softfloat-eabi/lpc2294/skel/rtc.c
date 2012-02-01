/*
 * rtc.c
 *
 *  Created on: Jan 31, 2012
 *      Author: daniel
 */

#include <time.h>

#include "rtc.h"
#include "lpc21xx.h"

void rtc_gettime(struct tm* tm) {

	tm->tm_sec = SEC;
	tm->tm_min = MIN;
	tm->tm_hour = HOUR;
	tm->tm_wday = DOW;
	tm->tm_mday = DOM;
	tm->tm_yday = DOY;
	tm->tm_year = YEAR;

}
