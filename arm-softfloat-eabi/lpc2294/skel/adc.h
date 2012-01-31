/*
 * adc.h
 *
 *  Created on: Jan 31, 2012
 *      Author: daniel
 */

#ifndef ADC_H_
#define ADC_H_

#define ADC_ADCR_SEL 0xF
#define ADC_ADCR_CLKDIV_SHIFT 7
#define ADC_ADCR_CLKDIV (0xF << ADC_ADCR_CLKDIV_SHIFT)
#define ADC_ADCR_BURST (1 << 16)
#define ADC_ADCR_CLKS_SHIFT 17
#define ADC_ADCR_CLKS_ELEVEN	(000 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_TEN		(001 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_NINE		(010 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_EIGHT		(011 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_SEVEN		(100 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_SIX		(101 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_FIVE		(110 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_FOUR		(111 << ADC_ADCR_CLKS_SHIFT)
#define ADC_ADCR_CLKS_PDN		(1 << 21)
#define ADC_ADCR_START_SHIFT	24
#define ADC_ADCR_START_NOSTART	(000 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_START_STARTNOW	(001 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_START_P016		(010 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_START_P022		(011 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_START_MAT01	(100 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_START_MAT03	(101 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_START_MAT10	(110 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_START_MAT11	(111 << ADC_ADCR_START_SHIFT)
#define ADC_ADCR_EDGE			(1 << 27)
#endif /* ADC_H_ */
