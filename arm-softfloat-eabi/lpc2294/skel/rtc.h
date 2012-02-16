/*
 * rtc.h
 *
 *  Created on: Jan 31, 2012
 *      Author: daniel
 */

#ifndef RTC_H_
#define RTC_H_

#include <time.h>
#include <sys/time.h>

#define RTC_ILR_RTCCIF 1
#define RTC_ILR_RTCALF (1 << 1)

#define RTC_CCR_CLKEN 1
#define RTC_CCR_CTCRST (1 << 1)
#define RTC_CCR_CTTEST (3 << 2)

#define RTC_CIIR_IMSEC	1
#define RTC_CIIR_IMMIN	(1 << 1)
#define RTC_CIIR_IMHOUR	(1 << 2)
#define RTC_CIIR_IMDOM	(1 << 3)
#define RTC_CIIR_IMDOW	(1 << 4)
#define RTC_CIIR_IMDOY	(1 << 5)
#define RTC_CIIR_IMMON	(1 << 6)
#define RTC_CIIR_IMYEAR (1 << 7)

#define RTC_AMR_AMRSEC	1
#define RTC_AMT_AMRMIN	(1 << 1)
#define RTC_AMT_AMRHOUR	(1 << 2)
#define RTC_AMT_AMRDOM	(1 << 3)
#define RTC_AMT_AMRDOW	(1 << 4)
#define RTC_AMT_AMRDOY	(1 << 5)
#define RTC_AMT_AMRMON	(1 << 6)
#define RTC_AMT_AMRYEAR	(1 << 7)

void rtc_start();
void rtc_gettime(struct tm *t);
void rtc_gettimeofday(struct timeval *tv);
void rtc_dumpregisters();

#endif /* RTC_H_ */
