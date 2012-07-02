/** @file
 * @brief	コマンドヒストリ
 *
 * @date	2007.03.18
 * @author	Takashi SHUDO
 */

#ifndef _H_history
#define _H_history

#include "config.h"
#include "lineedit.h"

struct history {
	char	his_buf[MAX_COM_HIS+1][MAX_LINE_COLUMS+1];// ヒストリバッファ
	short	pos;		// 現在表示編集中のヒストリ
	short	num;		// 記録されているヒストリ数
}; //!< ヒストリデータ構造

void init_history(struct history *his);
void save_history(struct history *his, char *com);
void foward_history(struct history *his, struct lineedit *le);
void back_history(struct history *his, struct lineedit *le);

#endif // _H_history
