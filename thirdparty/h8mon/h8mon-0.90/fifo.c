/** @file
 * @brief	汎用リングバッファ
 *
 * @date	2002.03.24
 * @author	Takashi SHUDO
 */

#include "fifo.h"

/**
 * @brief	fifoを初期化する
 * @param[in] fp	初期化するfifo
 * @param[in] buf	fifoとなるバッファポインタ
 * @param[in] size	バッファのバイト数
 */
void init_fifo(struct fifo *fp, unsigned char *buf, long size)
{
	fp->buf = buf;
	fp->size = size;
	fp->wp = buf;
	fp->rp = buf;
}

/**
 * @brief fifoにデータを書き込む
 * @param[in]	fp	データが書き込まれるfifo
 * @param[in]	data	書き込むデータ
 * @return	0:成功 0以外:バッファフル
 */
int write_fifo(struct fifo *fp, unsigned char data)
{
	unsigned char	*np;

	np = fp->wp + 1;
	if(np >= &fp->buf[fp->size]) {
		np = fp->buf;
	}

	if(np != fp->rp) {
		*fp->wp = data;
		fp->wp = np;

		return 0;
	} else {
		return -1;	// バッファフル
	}
}

/**
 * @brief fifoからデータを読み出す
 * @param[in]	fp	データが読み出されるfifo
 * @param[out]	data	読み出されるデータのポインタ
 * @return	1:成功 1以外:データ無し
 */
int read_fifo(struct fifo *fp, unsigned char *data)
{
	unsigned char	*np;

	if(fp->rp != fp->wp) {
		*data = *fp->rp;

		np = fp->rp + 1;
		if(np >= &fp->buf[fp->size]) {
			np = fp->buf;
		}
		fp->rp = np;

		return 1;
	} else {
		return 0;	// バッファエンプティ
	}
}

/**
 * @brief fifoに書き込まれているデータのサイズを返す
 * @param[in]	fp	データのサイズを読み出すfifo
 * @return	fifoに書き込まれているデータサイズ
 */
unsigned long fifo_size(struct fifo *fp)
{
	if(fp->wp >= fp->rp) {
		return fp->wp - fp->rp;
	} else {
		return fp->size - (fp->rp - fp->wp);
	}
}
