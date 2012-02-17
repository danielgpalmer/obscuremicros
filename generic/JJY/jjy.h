/*
 * jjy.h
 *
 *  Created on: Feb 17, 2012
 *      Author: daniel
 */

#ifndef JJY_H_
#define JJY_H_

#define SAMPLESPERSECOND 10

#define ZERO_ACTIVE (SAMPLESPERSECOND * 0.8)
#define ZERO_LOW (SAMPLESPERSECOND * 0.2)

#define ONE_ACTIVE (SAMPLESPERSECOND * 0.5)
#define ONE_LOW (SAMPLESPERSECOND * 0.5)

#define MARK_ACTIVE (SAMPLESPERSECOND * 0.2)
#define MARK_LOW (SAMPLESPERSECOND * 0.8)

#define NUMMARKS 7

void jjy_addsample(unsigned sample);

#endif /* JJY_H_ */
