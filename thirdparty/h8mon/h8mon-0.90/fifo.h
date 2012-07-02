/** @file
 * @brief	汎用リングバッファ
 *
 * @date	2002.03.24
 * @author	Takashi SHUDO
 */

#ifndef	_H_fifo
#define	_H_fifo

struct fifo {
	unsigned char	*buf;	// バッファのポインタ
	unsigned long	size;	// バッファのサイズ
	unsigned char	*wp;	// 書き込みデータのポインタ
	unsigned char	*rp;	// 読み込みデータのポインタ
};

void init_fifo(struct fifo *fp, unsigned char *buf, long size);
int write_fifo(struct fifo *fp, unsigned char data);
int read_fifo(struct fifo *fp, unsigned char *data);
unsigned long fifo_size(struct fifo *fp);

#endif	// _H_fifo
