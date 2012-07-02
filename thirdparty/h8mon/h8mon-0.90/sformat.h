/** @file
 * @brief motorola S-records デコーダ
 *
 * @date	2002.03.01
 * @author	Takashi SHUDO
 */

#ifndef _H_sformat
#define _H_sformat

int DecodeSFormat(char *str, unsigned char **add, unsigned long *len,
		   unsigned char *bbuf);

#endif // _H_sformat
