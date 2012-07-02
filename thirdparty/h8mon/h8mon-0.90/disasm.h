/** @file
 * @brief	H8/300H逆アセンブラ
 *
 * @date	2002.03.14
 * @author	Takashi SHUDO
 */

#ifndef _H_disasm
#define _H_disasm

#include "task.h"

unsigned char calc_next_pc(void *addr, st_task *task,
			   void **naddr, void **baddr);
unsigned char disasm(void *addr);

#endif // _H_disasm
