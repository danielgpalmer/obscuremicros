/** @file
 * @brief	ネットワークタイマ
 *		H8/3069F TMR使用
 *
 * @date	2011.04.29
 * @author	Takashi SHUDO
 */

#include "nettimer.h"

#define REG_8TCR2	(*(volatile unsigned char *)0xffff90)
#define REG_8TCSR2	(*(volatile unsigned char *)0xffff92)

#define REG_8TCR3	(*(volatile unsigned char *)0xffff91)
#define REG_8TCSR3	(*(volatile unsigned char *)0xffff93)

#define REG_8TCNT23	(*(volatile unsigned short *)0xffff98)

#define SYSTIMER_CLOCK_HZ	25000000

void start_nettimer(void)
{
	REG_8TCR3	= 0x00;
	REG_8TCR2	= 0x04;	// TCNT3 のオーバーフローでカウント

	REG_8TCNT23	= 0;
	REG_8TCR3	= 0x03;	/* φ/8192 = 25MHz/8192
				 * ≒ 3052(Hz) = 0.32768(ms)
				 */
	// 約21秒まで計測可能
}

unsigned short get_nettimer(void)
{
	/*
	 * 戻り値をおおよそ ms にする
	 * 多少の誤差は無視
	 */
	
	return (((unsigned long)REG_8TCNT23) * 1000)/(SYSTIMER_CLOCK_HZ/8192);
}
