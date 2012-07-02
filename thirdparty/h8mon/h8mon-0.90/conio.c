/** @file
 * @brief	コンソールI/O
 *
 * @date	2008.05.02
 * @author	Takashi SHUDO
 */
#include "conio.h"
#include "monio.h"

/**
 * @brief	1文字受信する
 * @param[out]	rd	受信データ
 * @return	1:成功 1以外:データ無し
 */
unsigned char cgetc(char *rd)
{
	return mgetc(rd);
}

/**
 * @brief	1文字送信する
 * @param[in]	td	送信データ
 */
void cputc(char td)
{
	if(td == '\n') {
		mputc('\r');
	}
	mputc(td);
}

