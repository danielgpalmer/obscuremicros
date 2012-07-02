/** @file
 * @brief	コマンドヒストリ
 *
 * @date	2007.03.18
 * @author	Takashi SHUDO
 */

#include "history.h"
#include "str.h"

/**
 * @brief	ヒストリーをクリアする
 * @param[in]	his	クリアするhistory
 */
void init_history(struct history *his)
{
	short i, j;

	for(j=0; j<(MAX_COM_HIS+1); j++) {
		for(i=0; i<(MAX_LINE_COLUMS+1); i++) {
			his->his_buf[j][i] = 0;
		}
	}
	
	his->pos = 0;
	his->num = 0;
}

/**
 * @brief	ヒストリーにコマンドを記録する
 * @param[in]	his	記録されるhistory
 * @param[in]	com	記録するコマンド文字列
 */
void save_history(struct history *his, char *com)
{
	short i;

	if(strleng(com) != 0) {
		strncopy(his->his_buf[0], com, MAX_LINE_COLUMS);
	
		for(i=MAX_COM_HIS; i>0; i--) {
			strncopy(his->his_buf[i], his->his_buf[i-1],
				 MAX_LINE_COLUMS);
		}
		if(his->num <= MAX_COM_HIS) {
			his->num ++;
		}
		his->pos = 0;
	}
}

/**
 * @brief	次のヒストリーを表示する
 * @param[in]	his	次のヒストリを表示するhistory
 * @param[out]	le	ヒストリを表示するlineedit
 */
void foward_history(struct history *his, struct lineedit *le)
{
	if(his->pos > 0) {
		his->pos --;
		set_str_lineedit(le, his->his_buf[his->pos]);
	}
}

/**
 * @brief	前のヒストリーを表示する
 * @param[in]	his	前のヒストリを表示するhistory
 * @param[out]	le	ヒストリを表示するlineedit
 */
void back_history(struct history *his, struct lineedit *le)
{
	if((his->pos < MAX_COM_HIS) &&
	   (his->pos < his->num)) {
		if(his->pos == 0) {
			strncopy(his->his_buf[0], le->buf, MAX_LINE_COLUMS);
		}
		
		his->pos ++;

		set_str_lineedit(le, his->his_buf[his->pos]);
	}
}

