/*
 * exceptions.h
 *
 *  Created on: Jan 30, 2012
 *      Author: daniel
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

void DATAABORT_Routine(void) __attribute__ ((interrupt("ABORT")));
void IRQ_Routine(void) __attribute__ ((interrupt("IRQ")));
void FIQ_Routine(void) __attribute__ ((interrupt("FIQ")));
void SWI_Routine(void) __attribute__ ((interrupt("SWI")));
void UNDEF_Routine(void) __attribute__ ((interrupt("UNDEF")));
void VICDefault_Routine(void) __attribute__ ((interrupt("IRQ")));

#endif /* EXCEPTIONS_H_ */
