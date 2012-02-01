#!/bin/bash

echo -n "#define RTC_PRELOAD_SEC "; date -u +%-S 
echo -n "#define RTC_PRELOAD_MIN "; date -u +%-M
echo -n "#define RTC_PRELOAD_HOUR "; date -u +%-H
echo -n "#define RTC_PRELOAD_DOM "; date -u  +%-d
echo -n "#define RTC_PRELOAD_DOW "; date -u +%-w
echo -n "#define RTC_PRELOAD_DOY "; date -u +%-j
echo -n "#define RTC_PRELOAD_MONTH "; date -u +%-m
echo -n "#define RTC_PRELOAD_YEAR "; date -u +%-Y
