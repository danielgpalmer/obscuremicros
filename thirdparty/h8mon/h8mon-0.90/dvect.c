/** @file
 * @brief	H8/3069ベクタテーブル
 *
 * @date	2002.02.27
 * @author	Takashi SHUDO
 */

#include "config.h"
#include "vect.h"
#include "task.h"

#define VVECT_ADDR	0x00ffbf20

typedef void (*va)(void);

long mon_stack[SIZEOF_MONSTACK/sizeof(long)]	//!< モニタのスタックエリア
__attribute__ ((section (".stack")));

extern void _sci1_eri(void);
extern void _sci1_rxi(void);
extern void _sci1_break(void);
extern void _sci1_txi(void);
extern void _sci1_tei(void);

/**
 * @brief	仮想ベクタテーブルの初期化
 */
void init_vectortable(void)
{
	DisableSystemIRQ;
	
	*((va *)(VVECT_ADDR + 0xe0)) = _sci1_eri;	// 0xe0	56	ERI1
	*((va *)(VVECT_ADDR + 0xe4)) = _sci1_break;	// 0xe4	57	RXI1
	*((va *)(VVECT_ADDR + 0xe8)) = _sci1_txi;	// 0xe8	58	TXI1
	*((va *)(VVECT_ADDR + 0xec)) = _sci1_tei;	// 0xec	59	TEI1
}
