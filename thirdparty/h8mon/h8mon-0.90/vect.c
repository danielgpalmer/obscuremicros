/** @file
 * @brief	H8/3069ベクタテーブル
 *
 * @date	2002.02.27
 * @author	Takashi SHUDO
 */

#include "config.h"
#include "vect.h"

#define MAXVECT	64	//!< ベクタテーブル数

typedef void (*va)(void);

long mon_stack[SIZEOF_MONSTACK/sizeof(long)]	//!< モニタのスタックエリア
__attribute__ ((section (".stack")));

//!< 仮想ベクタテーブル
va vvect[MAXVECT]	__attribute__ ((section (".vvect")));

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
	long i;

	for(i=0; i<MAXVECT; i++) {
		vvect[i] = 0;
	}

#ifndef FIX_SCI1_INTERRUPT
	vvect[56] = _sci1_eri;		// 0x000000e0	56	ERI1
	vvect[57] = _sci1_break;	// 0x000000e4	57	RXI1
	vvect[58] = _sci1_txi;		// 0x000000e8	58	TXI1
	vvect[59] = _sci1_tei;		// 0x000000ec	59	TEI1
#endif
}

extern void start(void);
extern void _trapa_break(void);
extern void _trapa2(void);

extern void _sci0_eri(void);
extern void _sci0_break(void);
extern void _sci0_txi(void);
extern void _sci0_tei(void);

extern void _int01(void);
extern void _int02(void);
extern void _int03(void);
extern void _int04(void);
extern void _int05(void);
extern void _int06(void);
extern void _int07(void);
extern void _int08(void);
extern void _int09(void);
extern void _int10(void);
extern void _int11(void);
extern void _int12(void);
extern void _int13(void);
extern void _int14(void);
extern void _int15(void);
extern void _int16(void);
extern void _int17(void);
extern void _int18(void);
extern void _int19(void);
extern void _int20(void);
extern void _int21(void);
extern void _int22(void);
extern void _int23(void);
extern void _int24(void);
extern void _int25(void);
extern void _int26(void);
extern void _int27(void);
extern void _int28(void);
extern void _int29(void);
extern void _int30(void);
extern void _int31(void);
extern void _int32(void);
extern void _int33(void);
extern void _int34(void);
extern void _int35(void);
extern void _int36(void);
extern void _int37(void);
extern void _int38(void);
extern void _int39(void);
extern void _int40(void);
extern void _int41(void);
extern void _int42(void);
extern void _int43(void);
extern void _int44(void);
extern void _int45(void);
extern void _int46(void);
extern void _int47(void);
extern void _int48(void);
extern void _int49(void);
extern void _int50(void);
extern void _int51(void);
extern void _int52(void);
extern void _int53(void);
extern void _int54(void);
extern void _int55(void);
extern void _int56(void);
extern void _int57(void);
extern void _int58(void);
extern void _int59(void);
extern void _int60(void);
extern void _int61(void);
extern void _int62(void);
extern void _int63(void);

const va vect[MAXVECT]	__attribute__ ((section (".vect"))) =
{
			// オフセット値	No.	割り込み名称
	start,		// 0x00000000	0	POWER-ON Reset PC
	_int01,		// 0x00000004	1	システム予約
	_int02,		// 0x00000008	2	システム予約
	_int03,		// 0x0000000c	3	システム予約
	_int04,		// 0x00000010	4	システム予約
	_int05,		// 0x00000014	5	システム予約
	_int06,		// 0x00000018	6	システム予約
	_int07,		// 0x0000001c	7	外部割込み NMI
	_int08,		// 0x00000020	8	トラップ命令
	_int09,		// 0x00000024	9	トラップ命令
	_trapa2,	// 0x00000028	10	トラップ命令
	_trapa_break,	// 0x0000002c	11	トラップ命令
	_int12,		// 0x00000030	12	外部割込み IRQ0
	_int13,		// 0x00000034	13	外部割込み IRQ1
	_int14,		// 0x00000038	14	外部割込み IRQ2
	_int15,		// 0x0000003c	15	外部割込み IRQ3
	_int16,		// 0x00000040	16	外部割込み IRQ4
	_int17,		// 0x00000044	17	外部割込み IRQ5
	_int18,		// 0x00000048	18	システム予約
	_int19,		// 0x0000004c	19	システム予約
	_int20,		// 0x00000050	20	WOVI(インターバルタイマ
	_int21,		// 0x00000054	21	CMI(コンペアマッチ)
	_int22,		// 0x00000058	22	リザーブ
	_int23,		// 0x0000005c	23	リザーブ
	_int24,		// 0x00000060	24	IMIA0
	_int25,		// 0x00000064	25	IMIB0
	_int26,		// 0x00000068	26	OVI0
	_int27,		// 0x0000006c	27	リザーブ
	_int28,		// 0x00000060	24	IMIA1
	_int29,		// 0x00000074	29	IMIB1
	_int30,		// 0x00000078	30	OVI1
	_int31,		// 0x0000007c	31	リザーブ
	_int32,		// 0x00000080	32	IMIA2
	_int33,		// 0x00000084	33	IMIB2
	_int34,		// 0x00000088	34	OVI2
	_int35,		// 0x0000008c	35	リザーブ
	_int36,		// 0x00000090	36	CMIA0
	_int37,		// 0x00000094	37	CMIB0
	_int38,		// 0x00000098	38	CMIA1/CMIB1
	_int39,		// 0x0000009c	39	TOVI0/TOVI1
	_int40,		// 0x000000a0	40	CMIA2
	_int41,		// 0x000000a4	41	CMIB2
	_int42,		// 0x000000a8	42	CMIA3/CMIB3
	_int43,		// 0x000000ac	43	TOVI2/TOVI3
	_int44,		// 0x000000b0	44	DEND0A
	_int45,		// 0x000000b4	45	DEND0B
	_int46,		// 0x000000b8	46	DEND1A
	_int47,		// 0x000000cc	47	DEND1B
	_int48,		// 0x000000c0	48	リザーブ
	_int49,		// 0x000000c4	49	リザーブ
	_int50,		// 0x000000c8	50	リザーブ
	_int51,		// 0x000000cc	51	リザーブ
#ifdef STUB_IS_SCI0
	_sci0_eri,	// 0x000000d0	52	ERI0
	_sci0_break,	// 0x000000d4	53	RXI0
	_sci0_txi,	// 0x000000d8	54	TXI0
	_sci0_tei,	// 0x000000dc	55	TEI0
#else
	_int52,		// 0x000000d0	52	ERI0
	_int53,		// 0x000000d4	53	RXI0
	_int54,		// 0x000000d8	54	TXI0
	_int55,		// 0x000000dc	55	TEI0
#endif
#ifdef FIX_SCI1_INTERRUPT
	_sci1_eri,	// 0x000000e0	56	ERI1
	_sci1_break,	// 0x000000e4	57	RXI1
	_sci1_txi,	// 0x000000e8	58	TXI1
	_sci1_tei,	// 0x000000ec	59	TEI1
#else
	_int56,		// 0x000000e0	56	ERI1
	_int57,		// 0x000000e4	57	RXI1
	_int58,		// 0x000000e8	58	TXI1
	_int59,		// 0x000000ec	59	TEI1
#endif
	_int60,		// 0x000000f0	60	ERI2
	_int61,		// 0x000000f4	61	RXI2
	_int62,		// 0x000000f8	62	TXI2
	_int63		// 0x000000fc	63	TEI2
}; //!< ベクタテーブル
