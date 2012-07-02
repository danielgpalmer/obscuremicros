/** @file
 * @brief	ラインエディタ
 *
 * @date	2002.03.30
 * @author	Takashi SHUDO
 */

#ifndef	_H_lineedit
#define	_H_lineedit

#include "config.h"

// ヒストリを使用する場合の使用するメモリ=((MAX_COM_HIS+2) * MAX_LINE_COLUMS)

#define	CTRL_A	0x01
#define	CTRL_B	0x02
#define	CTRL_C	0x03
#define	CTRL_D	0x04
#define	CTRL_E	0x05
#define	CTRL_H	0x08
#define	CTRL_K	0x0b

#define	ESC	0x1b

#define	BS	0x08
#define LF	0x0a
#define	CR	0x0d
#define	XOFF	0x11
#define	XON	0x13

struct lineedit {
	char	buf[MAX_LINE_COLUMS+1];	//!< 編集文字列バッファ
	short	cur_pos;	//!< カーソル位置
	char	stat;		//!< エスケープシーケンス状態
}; //!< ラインエディタデータ構造

// do_lineedit()の戻り値
#define LER_NOP		0	//!< イベント無し
#define LER_RETURN	1	//!< RETUEN受信
#define LER_BACKLINE	2	//!< 前のヒストリ受信
#define LER_NEXTLINE	3	//!< 次のヒストリ受信

void init_lineedit(struct lineedit *le);
int do_lineedit(struct lineedit *le, char ch);
void new_lineedit(struct lineedit *le);
void set_str_lineedit(struct lineedit *le, char *str);

#endif	// _H_lineedit
