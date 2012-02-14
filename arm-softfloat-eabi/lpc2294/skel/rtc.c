/*
 * rtc.c
 *
 *  Created on: Jan 31, 2012
 *      Author: daniel
 */

#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

#include "rtc.h"
#include "rtcpreload.h"
#include "lpc21xx.h"

#include "config.h"

void rtc_start() {

	CCR = RTC_CCR_CTCRST;
	PREINT = ((FOSC * PLL_M / VPBDIV_VAL) / 32768) - 1;
	PREFRAC = 0;
	SEC = RTC_PRELOAD_SEC;
	MIN = RTC_PRELOAD_MIN;
	HOUR = RTC_PRELOAD_HOUR;
	DOM = RTC_PRELOAD_DOM;
	DOW = RTC_PRELOAD_DOW;
	DOY = RTC_PRELOAD_DOY;
	MONTH = RTC_PRELOAD_MONTH;
	YEAR = RTC_PRELOAD_YEAR;

	CCR = RTC_CCR_CLKEN & ~RTC_CCR_CTCRST;

}

void rtc_gettime(struct tm* tm) {

	tm->tm_sec = SEC;
	tm->tm_min = MIN;
	tm->tm_hour = HOUR;
	tm->tm_wday = DOW;
	tm->tm_mday = DOM;
	tm->tm_yday = DOY;
	tm->tm_year = (YEAR - 1900);
	tm->tm_mon = (MONTH - 1);
	tm->tm_isdst = -1;

}

void rtc_gettimeofday(struct timeval *tv) {
	struct tm tm;
	rtc_gettime(&tm);
	tv->tv_sec = mktime(&tm);
	tv->tv_usec = CTC * (1000000 / (2 ^ 15));
}

void rtc_dumpregisters() {

	printf("ILR [0x%08x]\n", ILR);
	printf("CTC [0x%08x]\n", CTC);
	printf("CCR [0x%08x]\n", CCR);
	printf("CIIR [0x%08x]\n", CIIR);
	printf("AMR [0x%08x]\n", AMR);
	printf("CTIME0 [0x%08x]\n", CTIME0);
	printf("CTIME1 [0x%08x]\n", CTIME1);
	printf("CTIME2 [0x%08x]\n", CTIME2);
	printf("YEAR [%d]\n", YEAR);
	printf("MONTH [%d]\n", MONTH);
	printf("DOM [%d]\n", DOM);
	printf("HOUR [%d]\n", HOUR);
	printf("MIN [%d]\n", MIN);
	printf("SEC [%d]\n", SEC);
}
