/** @file
 * @brief	モニタI/Oデバイス
 *
 * @date	2007.03.11
 * @author	Takashi SHUDO
 */

#include "monio.h"

static const struct st_monio *monio;	//!< モニタIOデバイス
static const struct st_monio *stubio;	//!< stub IOデバイス

/**
 * @brief	モニタI/Oを初期化する
 * @param[in] io	モニタIOポインタ
 */
void init_monio(const struct st_monio *io)
{
	monio = io;

	monio->init();
}

/**
 * @brief	モニタIOからの受信データを取得する
 * @param[out]	rd	受信データポインタ
 * @return	!=0:データあり 0:データ無し
 */
unsigned char mgetc(char *rd)
{
	return monio->getc(rd);
}

/**
 * @brief	モニタIOからデータを送信する
 * @param[out]	td	送信データ
 */
void mputc(char td)
{
	monio->putc(td);
}

/*
  stub I/O
*/

/**
 * @brief	stub I/Oを初期化する
 * @param[in] io	stub IOポインタ
 */
void init_stubio(const struct st_monio *io)
{
	stubio = io;

	stubio->init();
}

/**
 * @brief	stub IOからの受信データを取得する
 * @param[out]	rd	受信データポインタ
 * @return	!=0:データあり 0:データ無し
 */
unsigned char stubgetc(char *rd)
{
	return stubio->getc(rd);
}

/**
 * @brief	stub IOからデータを送信する
 * @param[out]	td	送信データ
 */
void stubputc(char td)
{
	stubio->putc(td);
}
