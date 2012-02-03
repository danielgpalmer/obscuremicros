/*
 * exceptions.h
 *
 *  Created on: Jan 30, 2012
 *      Author: daniel
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

void DATAABORT_Routine(void) __attribute__ ((interrupt("ABORT"), weak, noreturn));
void IRQ_Routine(void) __attribute__ ((interrupt("IRQ"), weak, noreturn));
void FIQ_Routine(void) __attribute__ ((interrupt("FIQ"), weak, noreturn));
void SWI_Routine(void) __attribute__ ((interrupt("SWI"), weak, noreturn));
void UNDEF_Routine(void) __attribute__ ((interrupt("UNDEF"), weak, noreturn));
void VICDefault_Routine(void) __attribute__ ((interrupt("IRQ"), weak, noreturn));

#endif /* EXCEPTIONS_H_ */
