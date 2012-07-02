/** @file
 * @brief	H8/300H逆アセンブラ
 *
 * @date	2002.03.14
 * @author	Takashi SHUDO
 */

#include "disasm.h"
#include "tprintf.h"

#ifdef TEST
#include <stdio.h>
#define tprintf printf
#endif

typedef struct {
	unsigned char pos;	//!< オペランドのバイト位置
	unsigned char type;	//!< オペランドの表示種類
} st_operand;	//!< オペランドの情報

typedef struct {
	unsigned char kind;	//!< 種類
	unsigned char size:2;	//!< サイズ[.B/W/L]
	unsigned char sd:1;	//!< .[BWL]を表示？
	unsigned char len:5;	//!< 命令長
} st_order;	//!< 命令の情報

enum {
	NOOPR,
	IMM3,	// "#0x%01x:3",
	IMM8,	// "#0x%02x:8",
	IMM16,	// "#0x%02x%0x02x:16",
	IMM24,	// "#0x%02x%0x02x%0x02x:24",
	IMM32,	// "#0x%02x%0x02x%0x02x:32",
	RDH,	// "%s",	// Rn[H/L](H)
	RDL,	// "%s",	// Rn[H/L](L)
	EDH,	// "%s",	// [E|R]n(H)
	EDL,	// "%s",	// [E|R]n(L)
	ERDH,	// "%s",	// ERn(H)
	ERDL,	// "%s",	// ERn(L)
	AERD,	// "@%s",	// ERn(H)
	A16D,	// "@(%d:16,%s)",// ERn(H)
	A24D,	// "@(%d:24,%s)",// ERn(H)
	ARI,	// "@%s+",	// ERn(H)
	ARD,	// "@-%s",	// RRn(H)
	ABS8,	// "@%d:8",
	ABS16,	// "@%d:16",
	ABS24,	// "@%d:24",
	APD8,	// "@(%d:8,pc)",
	APD16,	// "@(%d:16,pc)",
	AA,	// "@@%d:8",
	BIMM8,	// ".%d",
	BIMM16,	// ".%d",
	CCR,	// "ccr"
	NBR1,	// "#1"
	NBR2,	// "#2"
	NBR4	// "#4"
};

/**
 * ニーモニック文字列データ
 */
static const char mnemonic[][10] = {
	"add", "adds",
	"addx", "and", "andc", "band",
	"bra", "brn", "bhi", "bls",
	"bcc", "bcs", "bne", "beq", "bvc",
	"bvs", "bpl", "bmi", "bge",
	"blt", "bgt", "ble", "bclr",
	"biand", "bild", "bior", "bist",
	"bixor", "bld", "bnot", "bor",
	"bset", "bsr", "bst", "btst", "bxor",
	"cmp", "daa", "das", "dec",
	"divxs", "divxu", "eepmov", "exts",
	"extu", "inc", "jmp", "jsr",
	"ldc", "mov", "movfpe", "movtpe",
	"mulxs", "mulxu", "neg", "nop",
	"not", "or", "orc", "pop",
	"push", "rotl", "rotr", "rotxl",
	"rotxr", "rte", "rts", "shal",
	"shar", "shll", "shlr", "sleep",
	"stc", "sub", "subs\t#1", "subs\t#2", "subs\t#4", "subx",
	"trapa", "xor", "xorc", ""
};

enum {
	ADD, ADDS,
	ADDX, AND, ANDC, BAND,
	BRA, BRN, BHI, BLS,
	BCC, BCS, BNE, BEQ, BVC,
	BVS, BPL, BMI, BGE,
	BLT, BGT, BLE, BCLR,
	BIAND, BILD, BIOR, BIST,
	BIXOR, BLD, BNOT, BOR,
	BSET, BSR, BST, BTST, BXOR,
	CMP, DAA, DAS, DEC,
	DIVXS, DIVXU, EEPMOV, EXTS,
	EXTU, INC, JMP, JSR,
	LDC, MOV, MOVFPE, MOVTPE,
	MULXS, MULXU, NEG, NOP,
	NOT, OR, ORC, POP,
	PUSH, ROTL, ROTR, ROTXL,
	ROTXR, RTE, RTS, SHAL,
	SHAR, SHLL, SHLR, SLEEP,
	STC, SUB, SUBS1, SUBS2, SUBS4, SUBX,
	TRAPA, XOR, XORC
};

static const char msize[][3] = {
	"", ".b", ".w", ".l"
};	//!< movサイズ

#define NSZ	0
#define SZB	1
#define SZW	2
#define SZL	3

/**
 * 8ビット長レジスタ名
 */
static const char regname8[][4] = {
	"r0h", "r1h", "r2h", "r3h", "r4h", "r5h", "r6h", "r7h",
	"r0l", "r1l", "r2l", "r3l", "r4l", "r5l", "r6l", "r7l"
};

/**
 * 16ビット長レジスタ名
 */
static const char regname16[][4] = {
	"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
	"e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7"
};

/**
 * 32ビット長レジスタ名
 */
static const char regname32[][4] = {
	"er0", "er1", "er2", "er3", "er4", "er5", "er6", "er7"
};

#define MAXPATWD	3
/**
 * 命令バイナリパターンデータ構造
 */
typedef struct {
	st_order 	order;		//!< 命令
	st_operand	operand[2];	//!< オペランド
	unsigned short	pat[MAXPATWD];	//!< ビットパターン
	unsigned short	mask[MAXPATWD];	//!< マスク
} st_obp;

/**
 * 命令バイナリパターンデータ
 */
static const st_obp h8300obp[] = {
  {{ADD,SZB,1,2}, {{1,IMM8},{0,RDL}},	{ 0x8000, 0, 0}, { 0xf000, 0, 0}},
  {{ADD,SZB,1,2}, {{1,RDH}, {1,RDL}},	{ 0x0800, 0, 0}, { 0xff00, 0, 0}},
  {{ADD,SZW,1,4}, {{2,IMM16},{1,EDL}},	{ 0x7910, 0, 0}, { 0xfff0, 0, 0}},
  {{ADD,SZW,1,2}, {{1,RDH}, {1,EDL}}, 	{ 0x0900, 0, 0}, { 0xff00, 0, 0}},
  {{ADD,SZL,1,6}, {{2,IMM32},{1,ERDL}},	{ 0x7a10, 0, 0}, { 0xfff8, 0, 0}},
  {{ADD,SZL,1,2}, {{1,ERDH},{1,ERDL}}, 	{ 0x0a80, 0, 0}, { 0xff88, 0, 0}},
  
  {{ADDS,SZL,0,2},{{0,NBR1},{1,ERDL}},	{ 0x0b00, 0, 0}, { 0xfff8, 0, 0}},
  {{ADDS,SZL,0,2},{{0,NBR2},{1,ERDL}},	{ 0x0b80, 0, 0}, { 0xfff8, 0, 0}},
  {{ADDS,SZL,0,2},{{0,NBR4},{1,ERDL}},	{ 0x0b90, 0, 0}, { 0xfff8, 0, 0}},
  
  {{ADDX,SZB,0,2}, {{0,IMM8},{0,RDL}},	{ 0x9000, 0, 0}, { 0xf000, 0, 0}},
  {{ADDX,SZB,0,2}, {{1,RDH},{1,RDL}},	{ 0x0e00, 0, 0}, { 0xff00, 0, 0}},
  
  {{AND,SZB,1,2}, {{1,IMM8},{0,RDL}},	{ 0xe000, 0, 0}, { 0xf000, 0, 0}},
  {{AND,SZB,1,2}, {{1,RDH},{1,RDL}},	{ 0x1600, 0, 0}, { 0xff00, 0, 0}},
  {{AND,SZW,1,4}, {{2,IMM16},{1,EDL}},	{ 0x7960, 0, 0}, { 0xfff0, 0, 0}},
  {{AND,SZW,1,2}, {{1,EDH},{1,EDL}},	{ 0x6600, 0, 0}, { 0xff00, 0, 0}},
  {{AND,SZL,1,6}, {{2,IMM32},{1,ERDL}},	{ 0x7a60, 0, 0}, { 0xfff0, 0, 0}},
  {{AND,SZL,1,4}, {{3,ERDH},{3,ERDL}},	{ 0x01f0, 0x6600, 0}, { 0xffff, 0xff88, 0}},
  
  {{ANDC,SZB,0,2}, {{1,IMM8},{0,NOOPR}},{ 0x0600, 0, 0}, { 0xff00, 0, 0}},
  
  {{BAND,SZB,0,2}, {{1,IMM3},{1,RDL}},	{ 0x7600, 0, 0}, { 0xff80, 0, 0}},
  {{BAND,SZB,0,4}, {{3,IMM3},{1,AERD}},	{ 0x7c00, 0x7600, 0}, { 0xff8f, 0xff8f, 0}},
  {{BAND,SZB,0,4}, {{3,IMM3},{1,ABS8},},{ 0x7e00, 0x7600, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BRA,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4000, 0, 0}, { 0xff00, 0, 0}},
  {{BRA,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5800, 0, 0}, { 0xffff, 0, 0}},
  {{BRN,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4100, 0, 0}, { 0xff00, 0, 0}},
  {{BRN,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5810, 0, 0}, { 0xffff, 0, 0}},
  {{BHI,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4200, 0, 0}, { 0xff00, 0, 0}},
  {{BHI,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5820, 0, 0}, { 0xffff, 0, 0}},
  {{BLS,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4300, 0, 0}, { 0xff00, 0, 0}},
  {{BLS,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5830, 0, 0}, { 0xffff, 0, 0}},
  {{BCC,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4400, 0, 0}, { 0xff00, 0, 0}},
  {{BCC,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5840, 0, 0}, { 0xffff, 0, 0}},
  {{BCS,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4500, 0, 0}, { 0xff00, 0, 0}},
  {{BCS,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5850, 0, 0}, { 0xffff, 0, 0}},
  {{BNE,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4600, 0, 0}, { 0xff00, 0, 0}},
  {{BNE,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5860, 0, 0}, { 0xffff, 0, 0}},
  {{BEQ,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4700, 0, 0}, { 0xff00, 0, 0}},
  {{BEQ,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5870, 0, 0}, { 0xffff, 0, 0}},
  {{BVC,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4800, 0, 0}, { 0xff00, 0, 0}},
  {{BVC,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5880, 0, 0}, { 0xffff, 0, 0}},
  {{BVS,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4900, 0, 0}, { 0xff00, 0, 0}},
  {{BVS,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x5890, 0, 0}, { 0xffff, 0, 0}},
  {{BPL,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4a00, 0, 0}, { 0xff00, 0, 0}},
  {{BPL,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x58a0, 0, 0}, { 0xffff, 0, 0}},
  {{BMI,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4b00, 0, 0}, { 0xff00, 0, 0}},
  {{BMI,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x58b0, 0, 0}, { 0xffff, 0, 0}},
  {{BGE,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4c00, 0, 0}, { 0xff00, 0, 0}},
  {{BGE,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x58c0, 0, 0}, { 0xffff, 0, 0}},
  {{BLT,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4d00, 0, 0}, { 0xff00, 0, 0}},
  {{BLT,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x58d0, 0, 0}, { 0xffff, 0, 0}},
  {{BGT,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4e00, 0, 0}, { 0xff00, 0, 0}},
  {{BGT,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x58e0, 0, 0}, { 0xffff, 0, 0}},
  {{BLE,NSZ,0,2}, {{1,BIMM8},{0,NOOPR}},{ 0x4f00, 0, 0}, { 0xff00, 0, 0}},
  {{BLE,NSZ,0,4}, {{2,BIMM16},{0,NOOPR}},{ 0x58f0, 0, 0}, { 0xffff, 0, 0}},
  
  {{BCLR,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7200, 0, 0}, { 0xff80, 0, 0}},
  {{BCLR,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7d00, 0x7200, 0}, { 0xff8f, 0xff8f, 0}},
  {{BCLR,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7f00, 0x7200, 0}, { 0xff8f, 0xff8f, 0}},
  {{BCLR,SZB,0,2}, {{1,RDH},{1,RDL}}, { 0x6200, 0, 0}, { 0xff00, 0, 0}},
  {{BCLR,SZB,0,4}, {{3,RDH},{1,AERD}}, { 0x7d00, 0x6200, 0}, { 0xff8f, 0xff8f, 0}},
  {{BCLR,SZB,0,4}, {{3,RDH},{1,ABS8}}, { 0x7f00, 0x6200, 0}, { 0xff00, 0xfff0, 0}},
  
  {{BIAND,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7680, 0, 0}, { 0xff80, 0, 0}},
  {{BIAND,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7c80, 0x7680, 0}, { 0xff8f, 0xff8f, 0}},
  {{BIAND,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7e00, 0x7680, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BILD,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7780, 0, 0}, { 0xff80, 0, 0}},
  {{BILD,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7c00, 0x7780, 0}, { 0xff8f, 0xff8f, 0}},
  {{BILD,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7e00, 0x7780, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BIOR,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7480, 0, 0}, { 0xff80, 0, 0}},
  {{BIOR,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7c00, 0x7480, 0}, { 0xff8f, 0xff8f, 0}},
  {{BIOR,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7e00, 0x7480, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BIST,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x6780, 0, 0}, { 0xff80, 0, 0}},
  {{BIST,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7d00, 0x6780, 0}, { 0xff8f, 0xff8f, 0}},
  {{BIST,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7f00, 0x6780, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BIXOR,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7580, 0, 0}, { 0xff80, 0, 0}},
  {{BIXOR,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7c00, 0x7580, 0}, { 0xff8f, 0xff8f, 0}},
  {{BIXOR,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7e00, 0x7580, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BLD,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7700, 0, 0}, { 0xff80, 0, 0}},
  {{BLD,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7c00, 0x7700, 0}, { 0xff8f, 0xff8f, 0}},
  {{BLD,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7e00, 0x7700, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BNOT,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7100, 0, 0}, { 0xff80, 0, 0}},
  {{BNOT,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7d00, 0x7100, 0}, { 0xff8f, 0xff8f, 0}},
  {{BNOT,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7f00, 0x7100, 0}, { 0xff00, 0xff8f, 0}},
  {{BNOT,SZB,0,2}, {{1,RDH},{1,RDL}}, { 0x6100, 0, 0}, { 0xff00, 0, 0}},
  {{BNOT,SZB,0,4}, {{3,RDH},{1,AERD}}, { 0x7d00, 0x6100, 0}, { 0xff8f, 0xff0f, 0}},
  {{BNOT,SZB,0,4}, {{3,RDH},{1,ABS8}}, { 0x7f00, 0x6100, 0}, { 0xff00, 0xff0f, 0}},
  
  {{BOR,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7400, 0, 0}, { 0xff80, 0, 0}},
  {{BOR,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7c00, 0x7400, 0}, { 0xff8f, 0xff8f, 0}},
  {{BOR,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7e00, 0x7400, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BSET,SZB,0,2}, {{1,IMM3},{1,RDL}}, { 0x7000, 0, 0}, { 0xff80, 0, 0}},
  {{BSET,SZB,0,4}, {{3,IMM3},{1,AERD}}, { 0x7d00, 0x7000, 0}, { 0xff8f, 0xff8f, 0}},
  {{BSET,SZB,0,4}, {{3,IMM3},{1,ABS8}}, { 0x7f00, 0x7000, 0}, { 0xff00, 0xff8f, 0}},
  {{BSET,SZB,0,2}, {{1,RDH},{1,RDL}}, { 0x6000, 0, 0}, { 0xff00, 0, 0}},
  {{BSET,SZB,0,4}, {{3,RDH},{1,AERD}}, { 0x7d00, 0x6000, 0}, { 0xff8f, 0xff0f, 0}},
  {{BSET,SZB,0,4}, {{3,RDH},{1,ABS8}}, { 0x7f00, 0x6000, 0}, { 0xff00, 0xff0f, 0}},
  
  {{BSR,NSZ,0,2}, {{1,IMM8},{0,NOOPR}},	{ 0x5500, 0, 0}, { 0xff00, 0, 0}},
  {{BSR,NSZ,0,4}, {{2,IMM16},{0,NOOPR}},{ 0x5c00, 0, 0}, { 0xffff, 0, 0}},
  
  {{BST,SZB,0,2}, {{1,IMM3},{1,RDL}},	{ 0x6700, 0, 0}, { 0xff80, 0, 0}},
  {{BST,SZB,0,4}, {{3,IMM3},{1,AERD}},	{ 0x7d00, 0x6700, 0}, { 0xff8f, 0xff8f, 0}},
  {{BST,SZB,0,4}, {{3,IMM3},{1,ABS8}},	{ 0x7f00, 0x6700, 0}, { 0xff00, 0xff8f, 0}},
  
  {{BTST,SZB,0,2}, {{1,IMM3},{1,RDL}},	{ 0x7300, 0, 0}, { 0xff80, 0, 0}},
  {{BTST,SZB,0,4}, {{3,IMM3},{1,AERD}},	{ 0x7c00, 0x7300, 0}, { 0xff8f, 0xff8f, 0}},
  {{BTST,SZB,0,4}, {{3,IMM3},{1,ABS8}},	{ 0x7e00, 0x7300, 0}, { 0xff00, 0xff8f, 0}},
  {{BTST,SZB,0,2}, {{1,RDH},{1,RDL}},	{ 0x6300, 0, 0}, { 0xff00, 0, 0}},
  {{BTST,SZB,0,4}, {{3,RDH},{1,AERD}},	{ 0x7c00, 0x6300, 0}, { 0xff8f, 0xff0f, 0}},
  {{BTST,SZB,0,4}, {{3,RDH},{1,ABS8}},	{ 0x7e00, 0x6300, 0}, { 0xff00, 0xff0f, 0}},
  
  {{BXOR,SZB,0,2}, {{1,IMM3},{1,RDL}},	{ 0x7500, 0, 0}, { 0xff80, 0, 0}},
  {{BXOR,SZB,0,4}, {{3,IMM3},{1,AERD}},	{ 0x7c00, 0x7500, 0}, { 0xff8f, 0xff8f, 0}},
  {{BXOR,SZB,0,4}, {{3,IMM3},{1,ABS8}},	{ 0x7e00, 0x7500, 0}, { 0xff00, 0xff8f, 0}},
  
  {{CMP,SZB,1,2}, {{1,IMM8},{0,RDL}},	{ 0xa000, 0, 0}, { 0xf000, 0, 0}},
  {{CMP,SZB,1,2}, {{1,RDH},{1,RDL}},	{ 0x1c00, 0, 0}, { 0xff00, 0, 0}},
  {{CMP,SZW,1,4}, {{2,IMM16},{1,EDL}},	{ 0x7920, 0, 0}, { 0xfff0, 0, 0}},
  {{CMP,SZW,1,2}, {{1,EDH},{1,EDL}},	{ 0x1d00, 0, 0}, { 0xff00, 0, 0}},
  {{CMP,SZL,1,6}, {{2,IMM32},{1,ERDL}},	{ 0x7a20, 0, 0}, { 0xfff8, 0, 0}},
  {{CMP,SZL,1,2}, {{1,ERDH},{1,ERDL}},	{ 0x1f80, 0, 0}, { 0xff88, 0, 0}},
  
  {{DAA,SZB,0,2}, {{1,RDL},{0,NOOPR}},	{ 0x0f00, 0, 0}, { 0xfff0, 0, 0}},
  
  {{DAS,SZB,0,2}, {{1,RDL},{0,NOOPR}},	{ 0x1f00, 0, 0}, { 0xfff0, 0, 0}},
  
  {{DEC,SZB,1,2}, {{1,RDL},{0,NOOPR}},	{ 0x1a00, 0, 0}, { 0xfff0, 0, 0}},
  {{DEC,SZW,1,2}, {{0,NBR1},{1,EDL}},	{ 0x1b50, 0, 0}, { 0xfff0, 0, 0}},
  {{DEC,SZW,1,2}, {{0,NBR2},{1,EDL}},	{ 0x1bd0, 0, 0}, { 0xfff0, 0, 0}},
  {{DEC,SZL,1,2}, {{0,NBR1},{1,ERDL}},	{ 0x1b70, 0, 0}, { 0xfff8, 0, 0}},
  {{DEC,SZL,1,2}, {{0,NBR2},{1,ERDL}},	{ 0x1bf0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{DIVXS,SZB,1,4}, {{3,RDH},{3,RDL}},	{ 0x01d0, 0x5100, 0}, { 0xffff, 0xff00, 0}},
  {{DIVXS,SZW,1,4}, {{3,EDH},{3,ERDL}},	{ 0x01d0, 0x5300, 0}, { 0xffff, 0xff08, 0}},
  
  {{DIVXU,SZB,1,2}, {{1,RDH},{1,RDL}},	{ 0x5100, 0, 0}, { 0xff00, 0, 0}},
  {{DIVXU,SZW,1,2}, {{1,EDH},{1,ERDL}},	{ 0x5300, 0, 0}, { 0xff08, 0, 0}},
  
  {{EEPMOV,SZB,1,4}, {{0,NOOPR},{0,NOOPR}},{ 0x7b5c, 0x598f, 0}, { 0xffff, 0xffff, 0}},
  {{EEPMOV,SZW,1,4}, {{0,NOOPR},{0,NOOPR}},{ 0x7bd4, 0x598f, 0}, { 0xffff, 0xffff, 0}},
  
  {{EXTS,SZW,1,2}, {{1,EDL},{0,NOOPR}},	{ 0x17d0, 0, 0}, { 0xfff0, 0, 0}},
  {{EXTS,SZL,1,2}, {{1,ERDL},{0,NOOPR}},{ 0x17f0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{EXTU,SZW,1,2}, {{1,EDL},{0,NOOPR}},	{ 0x1750, 0, 0}, { 0xfff0, 0, 0}},
  {{EXTU,SZL,1,2}, {{1,ERDL},{0,NOOPR}},{ 0x1770, 0, 0}, { 0xfff8, 0, 0}},
  
  {{INC,SZB,1,2}, {{1,RDL},{0,NOOPR}},	{ 0x0a00, 0, 0}, { 0xfff0, 0, 0}},
  {{INC,SZW,1,2}, {{0,NBR1},{1,EDL}},	{ 0x0b50, 0, 0}, { 0xfff0, 0, 0}},
  {{INC,SZW,1,2}, {{0,NBR2},{1,EDL}},	{ 0x0bd0, 0, 0}, { 0xfff0, 0, 0}},
  {{INC,SZL,1,2}, {{0,NBR1},{1,ERDL}},	{ 0x0b70, 0, 0}, { 0xfff8, 0, 0}},
  {{INC,SZL,1,2}, {{0,NBR2},{1,ERDL}},	{ 0x0bf0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{JMP,NSZ,0,2}, {{1,AERD},{0,NOOPR}},	{ 0x5900, 0, 0}, { 0xff8f, 0, 0}},
  {{JMP,NSZ,0,4}, {{1,ABS24},{0,NOOPR}},{ 0x5a00, 0, 0}, { 0xff00, 0, 0}},
  {{JMP,NSZ,0,2}, {{1,AA},{0,NOOPR}},	{ 0x5b00, 0, 0}, { 0xff00, 0, 0}},
  
  {{JSR,NSZ,0,2}, {{1,AERD},{0,NOOPR}},	{ 0x5d00, 0, 0}, { 0xff8f, 0, 0}},
  {{JSR,NSZ,0,4}, {{1,ABS24},{0,NOOPR}},{ 0x5e00, 0, 0}, { 0xff00, 0, 0}},
  {{JSR,NSZ,0,2}, {{1,AA},{0,NOOPR}},	{ 0x5f00, 0, 0}, { 0xff00, 0, 0}},
  
  {{LDC,SZB,0,2}, {{1,IMM8},{0,CCR}},	{ 0x0700, 0, 0}, { 0xff00, 0, 0}},
  {{LDC,SZB,0,2}, {{1,RDL},{0,CCR}},	{ 0x0300, 0, 0}, { 0xfff0, 0, 0}},
  {{LDC,SZW,0,4}, {{1,AERD},{0,CCR}},	{ 0x0140, 0x6900, 0}, { 0xffff, 0xff8f, 0}},
  {{LDC,SZW,0,6}, {{3,A16D},{0,CCR}},	{ 0x0140, 0x6f00, 0}, { 0xffff, 0xff8f, 0}},
  {{LDC,SZW,0,10},{{3,A24D},{0,CCR}},	{ 0x0140, 0x7800, 0x6b20}, { 0xffff, 0xff8f, 0xffff}},
  {{LDC,SZW,0,4}, {{3,ARI},{0,CCR}},	{ 0x0140, 0x6d00, 0}, { 0xffff, 0xff8f, 0}},
  {{LDC,SZW,0,6}, {{4,ABS16},{0,CCR}},	{ 0x0140, 0x6b00, 0}, { 0xffff, 0xffff, 0}},
  {{LDC,SZW,0,8}, {{5,ABS24},{0,CCR}},	{ 0x0140, 0x6b20, 0}, { 0xffff, 0xffff, 0}},
  
  {{MOV,SZB,1,2}, {{1,IMM8},{0,RDL}},	{ 0xf000, 0, 0}, { 0xf000, 0, 0}},
  {{MOV,SZB,1,2}, {{1,RDH},{1,RDL}},	{ 0x0c00, 0, 0}, { 0xff00, 0, 0}},
  {{MOV,SZB,1,2}, {{1,AERD},{1,RDL}},	{ 0x6800, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZB,1,4}, {{1,A16D},{1,RDL}},	{ 0x6e00, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZB,1,8}, {{1,A24D},{3,RDL}},	{ 0x7800, 0x6a20, 0}, { 0xff8f, 0xfff0, 0}},
  {{MOV,SZB,1,2}, {{1,ARD},{1,RDL}},	{ 0x6c00, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZB,1,2}, {{1,ABS8},{0,RDL}},	{ 0x2000, 0, 0}, { 0xf000, 0, 0}},
  {{MOV,SZB,1,4}, {{2,ABS16},{1,RDL}},	{ 0x6a00, 0, 0}, { 0xfff0, 0, 0}},
  {{MOV,SZB,1,6}, {{3,ABS24},{1,RDL}},	{ 0x6a20, 0, 0}, { 0xfff0, 0, 0}},
  {{MOV,SZB,1,2}, {{1,RDL},{1,AERD}},	{ 0x6880, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZB,1,4}, {{1,RDL},{1,A16D}},	{ 0x6e80, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZB,1,8}, {{3,RDL},{1,A24D}},	{ 0x7800, 0x6aa0, 0}, { 0xff8f, 0xfff0, 0}},
  {{MOV,SZB,1,2}, {{1,RDL},{1,ARD}},	{ 0x6c80, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZB,1,2}, {{0,RDL},{1,ABS8}},	{ 0x3000, 0, 0}, { 0xf000, 0, 0}},
  {{MOV,SZB,1,4}, {{1,RDL},{2,ABS16}},	{ 0x6a80, 0, 0}, { 0xfff0, 0, 0}},
  {{MOV,SZB,1,6}, {{1,RDL},{3,ABS24}},	{ 0x6aa0, 0, 0}, { 0xfff0, 0, 0}},
  
  {{MOV,SZW,1,4}, {{2,IMM16},{1,EDL}},	{ 0x7900, 0, 0}, { 0xfff0, 0, 0}},
  {{MOV,SZW,1,2}, {{1,EDH},{1,EDL}},	{ 0x0d00, 0, 0}, { 0xff00, 0, 0}},
  {{MOV,SZW,1,2}, {{1,AERD},{1,EDL}},	{ 0x6900, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZW,1,4}, {{1,A16D},{1,EDL}},	{ 0x6f00, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZW,1,8}, {{1,A24D},{3,EDL}},	{ 0x7800, 0x6b20, 0}, { 0xff8f, 0xfff0, 0}},
  {{MOV,SZW,1,2}, {{1,ARI},{1,EDL}},	{ 0x6d00, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZW,1,4}, {{2,ABS16},{1,EDL}},	{ 0x6b00, 0, 0}, { 0xfff0, 0, 0}},
  {{MOV,SZW,1,6}, {{3,ABS24},{1,EDL}},	{ 0x6b20, 0, 0}, { 0xfff0, 0, 0}},
  {{MOV,SZW,1,2}, {{1,EDL},{1,AERD}},	{ 0x6980, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZW,1,4}, {{1,EDL},{1,A16D}},	{ 0x6f80, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZW,1,8}, {{3,EDL},{1,A24D}},	{ 0x7800, 0x6ba0, 0}, { 0xff8f, 0xfff0, 0}},
  {{MOV,SZW,1,2}, {{1,EDL},{1,ARD}},	{ 0x6d80, 0, 0}, { 0xff80, 0, 0}},
  {{MOV,SZW,1,4}, {{1,EDL},{1,ABS16}},	{ 0x6b80, 0, 0}, { 0xfff0, 0, 0}},
  {{MOV,SZW,1,6}, {{3,EDL},{3,ABS24}},	{ 0x6ba0, 0, 0}, { 0xfff0, 0, 0}},

  {{MOV,SZL,1,6}, {{2,IMM32},{1,ERDL}},	{ 0x7a00, 0, 0}, { 0xfff8, 0, 0}},
  {{MOV,SZL,1,2}, {{1,ERDH},{1,ERDL}},	{ 0x0f80, 0, 0}, { 0xff88, 0, 0}},
  {{MOV,SZL,1,4}, {{3,ERDH},{3,ERDL}},	{ 0x0100, 0x6900, 0}, { 0xffff, 0xff88, 0}},
  {{MOV,SZL,1,6}, {{3,A16D},{3,ERDL}},	{ 0x0100, 0x6f00, 0}, { 0xffff, 0xff88, 0}},
  {{MOV,SZL,1,10},{{3,A24D},{5,ERDL}},	{ 0x0100, 0x7800, 0x6b20}, { 0xffff, 0xff8f, 0xfff8}},
  {{MOV,SZL,1,4},{{3,ARI},{3,ERDL}},	{ 0x0100, 0x6d00, 0}, { 0xffff, 0xff88, 0}},
  {{MOV,SZL,1,6},{{4,ABS16},{3,ERDL}},	{ 0x0100, 0x6b00, 0}, { 0xffff, 0xfff8, 0}},
  {{MOV,SZL,1,8},{{5,ABS24},{3,ERDL}},	{ 0x0100, 0x6b20, 0}, { 0xffff, 0xfff8, 0}},
  {{MOV,SZL,1,4}, {{3,ERDL},{3,AERD}},	{ 0x0100, 0x6980, 0}, { 0xffff, 0xff88, 0}},
  {{MOV,SZL,1,6}, {{3,ERDL},{3,A16D}},	{ 0x0100, 0x6f80, 0}, { 0xffff, 0xff88, 0}},
  {{MOV,SZL,1,10},{{5,ERDL},{3,A24D}},	{ 0x0100, 0x7880, 0x6ba0}, { 0xffff, 0xff8f, 0xfff8}},
  {{MOV,SZL,1,4},{{3,ERDL},{3,ARD}},	{ 0x0100, 0x6d80, 0}, { 0xffff, 0xff88, 0}},
  {{MOV,SZL,1,6},{{3,ERDL},{4,ABS16}},	{ 0x0100, 0x6b80, 0}, { 0xffff, 0xfff8, 0}},
  {{MOV,SZL,1,8},{{3,ERDL},{5,ABS24}},	{ 0x0100, 0x6ba0, 0}, { 0xffff, 0xfff8, 0}},
  
  {{MOVFPE,SZB,0,4},{{2,A16D},{1,RDL}},	{ 0x6a40, 0, 0}, { 0xfff0, 0, 0}},
  
  {{MOVTPE,SZB,0,4},{{1,RDL},{2,A16D}},	{ 0x6ac0, 0, 0}, { 0xfff0, 0, 0}},

  {{MULXS,SZB,1,4},{{3,RDH},{3,RDL}},	{ 0x01c0, 0x5000, 0}, { 0xffff, 0xff00, 0}},
  {{MULXS,SZW,1,4},{{3,EDH},{3,EDL}},	{ 0x01c0, 0x5200, 0}, { 0xffff, 0xff08, 0}},
  
  {{MULXU,SZB,1,2},{{1,RDH},{2,RDL}},	{ 0x5000, 0, 0}, { 0xff00, 0, 0}},
  {{MULXU,SZW,1,2},{{1,EDH},{2,EDL}},	{ 0x5200, 0, 0}, { 0xff08, 0, 0}},
  
  {{NEG,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1780, 0, 0}, { 0xfff0, 0, 0}},
  {{NEG,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1790, 0, 0}, { 0xfff0, 0, 0}},
  {{NEG,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x17b0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{NOP,NSZ,0,2},{{0,NOOPR},{0,NOOPR}},	{ 0x0000, 0, 0}, { 0xffff, 0, 0}},
  
  {{NOT,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1700, 0, 0}, { 0xfff0, 0, 0}},
  {{NOT,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1710, 0, 0}, { 0xfff0, 0, 0}},
  {{NOT,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x1730, 0, 0}, { 0xfff8, 0, 0}},
  
  {{OR,SZB,1,2},{{1,IMM8},{0,RDL}},	{ 0xc000, 0, 0}, { 0xf000, 0, 0}},
  {{OR,SZB,1,2},{{1,RDH},{1,RDL}},	{ 0x1400, 0, 0}, { 0xff00, 0, 0}},
  {{OR,SZW,1,4},{{2,IMM16},{1,EDL}},	{ 0x7940, 0, 0}, { 0xfff0, 0, 0}},
  {{OR,SZW,1,2},{{1,EDH},{1,EDL}},	{ 0x6400, 0, 0}, { 0xff00, 0, 0}},
  {{OR,SZL,1,6},{{2,IMM32},{1,ERDL}},	{ 0x7a40, 0, 0}, { 0xfff8, 0, 0}},
  {{OR,SZL,1,4},{{3,ERDH},{3,ERDL}},	{ 0x01f0, 0x6400, 0}, { 0xffff, 0xff88, 0}},
  
  {{ORC,SZB,0,2},{{1,IMM8},{0,NOOPR}},	{ 0x0400, 0, 0}, { 0xff00, 0, 0}},
  
  {{POP,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x6d70, 0, 0}, { 0xfff0, 0, 0}},
  {{POP,SZL,1,4},{{3,ERDL},{0,NOOPR}},	{ 0x0100, 0x6d70, 0}, { 0xffff, 0xfff8, 0}},
  
  {{PUSH,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x6df0, 0, 0}, { 0xfff0, 0, 0}},
  {{PUSH,SZL,1,4},{{3,ERDL},{0,NOOPR}},	{ 0x0100, 0x6df0, 0}, { 0xffff, 0xfff8, 0}},
  
  {{ROTL,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1280, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTL,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1290, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTL,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x12b0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{ROTR,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1380, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTR,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1390, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTR,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x13b0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{ROTXL,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1200, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTXL,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1210, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTXL,SZL,1,2},{{1,ERDL},{0,NOOPR}},{ 0x1230, 0, 0}, { 0xfff8, 0, 0}},
  
  {{ROTXR,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1300, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTXR,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1310, 0, 0}, { 0xfff0, 0, 0}},
  {{ROTXR,SZL,1,2},{{1,ERDL},{0,NOOPR}},{ 0x1330, 0, 0}, { 0xfff8, 0, 0}},
  
  {{RTE,NSZ,0,2},{{0,NOOPR},{0,NOOPR}},	{ 0x5670, 0, 0}, { 0xffff, 0, 0}},
  
  {{RTS,NSZ,0,2},{{0,NOOPR},{0,NOOPR}},	{ 0x5470, 0, 0}, { 0xffff, 0, 0}},
  
  {{SHAL,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1080, 0, 0}, { 0xfff0, 0, 0}},
  {{SHAL,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1090, 0, 0}, { 0xfff0, 0, 0}},
  {{SHAL,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x10b0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{SHAR,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1180, 0, 0}, { 0xfff0, 0, 0}},
  {{SHAR,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1190, 0, 0}, { 0xfff0, 0, 0}},
  {{SHAR,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x11b0, 0, 0}, { 0xfff8, 0, 0}},
  
  {{SHLL,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1000, 0, 0}, { 0xfff0, 0, 0}},
  {{SHLL,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1010, 0, 0}, { 0xfff0, 0, 0}},
  {{SHLL,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x1030, 0, 0}, { 0xfff8, 0, 0}},
  
  {{SHLR,SZB,1,2},{{1,RDL},{0,NOOPR}},	{ 0x1100, 0, 0}, { 0xfff0, 0, 0}},
  {{SHLR,SZW,1,2},{{1,EDL},{0,NOOPR}},	{ 0x1110, 0, 0}, { 0xfff0, 0, 0}},
  {{SHLR,SZL,1,2},{{1,ERDL},{0,NOOPR}},	{ 0x1130, 0, 0}, { 0xfff8, 0, 0}},
  
  {{SLEEP,NSZ,0,2},{{0,NOOPR},{0,NOOPR}},{ 0x0180, 0, 0}, { 0xffff, 0, 0}},
  
  {{STC,SZB,0,2}, {{1,RDL},{0,CCR}},	{ 0x0300, 0, 0}, { 0xfff0, 0, 0}},
  {{STC,SZW,0,4}, {{1,AERD},{0,CCR}},	{ 0x0140, 0x6980, 0}, { 0xffff, 0xff8f, 0}},
  {{STC,SZW,0,6}, {{3,A16D},{0,CCR}},	{ 0x0140, 0x6f80, 0}, { 0xffff, 0xff8f, 0}},
  {{STC,SZW,0,10},{{3,A24D},{0,CCR}},	{ 0x0140, 0x7800, 0x6ba0}, { 0xffff, 0xff8f, 0xffff}},
  {{STC,SZW,0,4}, {{3,ARI},{0,CCR}},	{ 0x0140, 0x6d80, 0}, { 0xffff, 0xff8f, 0}},
  {{STC,SZW,0,6}, {{4,ABS16},{0,CCR}},	{ 0x0140, 0x6b80, 0}, { 0xffff, 0xffff, 0}},
  {{STC,SZW,0,8}, {{5,ABS24},{0,CCR}},	{ 0x0140, 0x6ba0, 0}, { 0xffff, 0xffff, 0}},
  
  {{SUB,SZB,1,2}, {{1,RDH}, {1,RDL}},	{ 0x1800, 0, 0}, { 0xff00, 0, 0}},
  {{SUB,SZW,1,4}, {{2,IMM16},{1,EDL}},	{ 0x7930, 0, 0}, { 0xfff0, 0, 0}},
  {{SUB,SZW,1,2}, {{1,RDH}, {1,EDL}}, 	{ 0x1900, 0, 0}, { 0xff00, 0, 0}},
  {{SUB,SZL,1,6}, {{2,IMM32},{1,ERDL}},	{ 0x7a30, 0, 0}, { 0xfff8, 0, 0}},
  {{SUB,SZL,1,2}, {{1,ERDH},{1,ERDL}}, 	{ 0x1a80, 0, 0}, { 0xff88, 0, 0}},
  
  {{SUBS1,SZL,0,2},{{1,ERDL},{0,NOOPR}},{ 0x1b00, 0, 0}, { 0xfff8, 0, 0}},
  {{SUBS2,SZL,0,2},{{1,ERDL},{0,NOOPR}},{ 0x1b80, 0, 0}, { 0xfff8, 0, 0}},
  {{SUBS4,SZL,0,2},{{1,ERDL},{0,NOOPR}},{ 0x1b90, 0, 0}, { 0xfff8, 0, 0}},
  
  {{SUBX,SZB,0,2}, {{0,IMM8},{0,RDL}},	{ 0xb000, 0, 0}, { 0xf000, 0, 0}},
  {{SUBX,SZB,0,2}, {{1,RDH},{1,RDL}},	{ 0x1e00, 0, 0}, { 0xff00, 0, 0}},
  
  {{TRAPA,SZB,0,2}, {{1,IMM3},{0,NOOPR}},{ 0x5700, 0, 0}, { 0xffcf, 0, 0}},

  {{XOR,SZB,1,2}, {{1,IMM8},{0,RDL}},	{ 0xd000, 0, 0}, { 0xf000, 0, 0}},
  {{XOR,SZB,1,2}, {{1,RDH},{1,RDL}},	{ 0x1500, 0, 0}, { 0xff00, 0, 0}},
  {{XOR,SZW,1,4}, {{2,IMM16},{1,EDL}},	{ 0x7950, 0, 0}, { 0xfff0, 0, 0}},
  {{XOR,SZW,1,2}, {{1,EDH},{1,EDL}},	{ 0x6500, 0, 0}, { 0xff00, 0, 0}},
  {{XOR,SZL,1,6}, {{2,IMM32},{1,ERDL}},	{ 0x7a50, 0, 0}, { 0xfff8, 0, 0}},
  {{XOR,SZL,1,4}, {{3,ERDH},{3,ERDL}},	{ 0x01f0, 0x6500, 0}, { 0xffff, 0xff88, 0}},
  
  {{XORC,SZB,0,2}, {{1,IMM8},{0,NOOPR}}, { 0x0500, 0, 0}, { 0xff00, 0, 0}},
  
  {{0,0,0,0}, {{0,0},{0,0}}, { 0, 0, 0}, { 0, 0, 0}}
};

/**
 * @brief	逆アセンブル表示する
 * @param[in]	addr	逆アセンブルするアドレス
 * @param[in]	pos	addrオフセット値
 * @param[in]	type	タイプ
 */
static void disp_operand(unsigned char *addr,
			unsigned char pos, unsigned char type)
{
	switch(type) {
	case IMM3:
		tprintf("#0x%01x:3", (int)(((*(addr+pos) & 0x70)) >> 4));
		break;
		
	case IMM8:
		tprintf("#0x%02x:8", (int)*(addr+pos));
		break;
			
	case IMM16:
		tprintf("#0x%02x%02x:16", (int)*(addr+pos),
			(int)*(addr+pos+1));
		break;
			
	case IMM24:
		tprintf("#0x%02x%x02x%02x:24",
			(int)*(addr+pos), (int)*(addr+pos+1),
			(int)*(addr+pos+2));
		break;
			
	case IMM32:
		tprintf("#0x%02x%02x%02x%02x:32",
			(int)*(addr+pos), (int)*(addr+pos+1),
			(int)*(addr+pos+2), (int)*(addr+pos+3));
		break;
		
	case RDH:
		tprintf("%s", regname8[((*(addr+pos)) >> 4)]);
		break;
				
	case RDL:
		tprintf("%s", regname8[((*(addr+pos)) & 0x0f)]);
		break;
						
	case EDH:
		tprintf("%s", regname16[((*(addr+pos)) >> 4)]);
		break;
		
	case EDL:
		tprintf("%s", regname16[((*(addr+pos)) & 0x0f)]);
		break;
		
	case ERDH:
		tprintf("%s", regname32[((*(addr+pos)) >> 4) & 0x07]);
		break;
		
	case ERDL:
		tprintf("%s", regname32[((*(addr+pos)) & 0x07)]);
		break;
					
	case AERD:
		tprintf("@%s", regname32[((*(addr+pos)) >> 4) & 0x07]);
		break;
					
	case A16D:
		tprintf("@(#0x%02x%02x:16,%s)",
			(int)*(addr+pos+1), (int)*(addr+pos+2),
			regname32[((*(addr+pos)) >> 4) & 0x07]);// ERn(H));
		break;
		
	case A24D:
		tprintf("@(#0x%02x%02x%02x:24,%s)",
			(int)*(addr+pos+4), (int)*(addr+pos+5),
			(int)*(addr+pos+6),
			regname32[((*(addr+pos)) >> 4) & 0x07]);// ERn(H));
		break;
		
	case ARI:
		tprintf("@%s+", regname32[((*(addr+pos)) >> 4) & 0x07]);
		// ERn(H));
		break;
		
	case ARD:
		tprintf("@-%s",	regname32[((*(addr+pos)) >> 4) & 0x07]);
		// RRn(H));
		break;
		
	case ABS8:
		tprintf("@#0x%02x:8", (int)*(addr+pos));
		break;
			
	case ABS16:
		tprintf("@#0x%02x%02x:16", (int)*(addr+pos),
			(int)*(addr+pos+1));
		break;
		
	case ABS24:
		tprintf("@#0x%02x%02x%02x:24",
			(int)*(addr+pos), (int)*(addr+pos+1),
			(int)*(addr+pos+2));
		break;
		
	case APD8:
		tprintf("@(#0x%02x:8,pc)", (int)*(addr+pos));
		break;
			
	case APD16:
		tprintf("@(#0x%02x%02x:16,pc)",
			(int)*(addr+pos), (int)*(addr+pos+1));
		break;
			
	case AA:
		tprintf("@@#0x%02x:8", (int)*(addr+pos));
		break;
		
	case BIMM8:
		tprintf(".%d", (int)(*((signed char *)addr+pos)));
		break;
		
	case BIMM16:
		tprintf(".%d", (int)((short)(*((signed char *)addr+pos))<<8)
			+ *((unsigned char *)addr+pos+1)+2);
		break;
		
	case CCR:
		tprintf("ccr");
		break;
		
	case NBR1:
		tprintf("#1");
		break;
		
	case NBR2:
		tprintf("#2");
		break;
		
	case NBR4:
		tprintf("#4");
		break;
	}
}

/**
 * @brief	ビットパターンのコンペア
 */
static unsigned char compare_bin(unsigned short *src,
				const unsigned short *mask,
				const unsigned short *pat,
				unsigned char wl)
{
	int i;

	for(i=0; i<wl; i++) {
//		tprintf("## %d/%d : %04x %04x %04x\n", i, wl, *src,
//		*mask, *pat);
		
		if((*src & *mask) != *pat) {
			return 0;
		}

		src ++;
		mask ++;
		pat ++;
	}

//	tprintf("#wl=%d\n", wl);
	
	return 1;
}

/**
 * @brief	addrの命令実行後のPCを計算
 * @param[in]	addr	現在のPC
 * @param[in]	task	タスクのレジスタ状態
 * @param[out]	naddr	実行後のPCのポインタ
 * @param[out]	baddr	ブランチ実行後のPC
 * @return	!=-1:PCの命令のサイズ(byte)
 *		-1:PCは未定義命令
 */
unsigned char calc_next_pc(void *addr, st_task *task,
			 void **naddr, void **baddr)
{
	unsigned short wp[MAXPATWD];
	const st_obp *mp;
	unsigned char i, j;

	for(i=0,j=0; i<MAXPATWD; i++, j+=2) {
		wp[i] = ((((unsigned short)*((unsigned char *)addr+j))<<8)
			 + *((unsigned char *)addr+j+1));
	}

	/*
	 * 命令をサーチ
	 */
	for(mp = h8300obp; mp->mask[0]!=0; mp++) {
		unsigned char wn = MAXPATWD;

		if((mp->order.len/2) < wn) {
			(wn = mp->order.len/2);
		}
		
		if(compare_bin(wp, mp->mask, mp->pat, wn)) {

			long pos = mp->operand[0].pos;
			signed long offset = 0;
			long next = 0;

			switch(mp->order.kind) {
			case RTS:
			case RTE:
				next =
#ifdef H8NORMAL
					(((unsigned long)(*(unsigned char *)
					((INTPOINTER)task->name.sp)))<<8)+
					(((unsigned long)(*(unsigned char *)
					((INTPOINTER)(task->name.sp+1)))));
#else
					(((unsigned long)(*(unsigned char *)
					(task->name.sp+1)))<<16) +
					(((unsigned long)(*(unsigned char *)
					(task->name.sp+2)))<<8) +
					(((unsigned long)(*(unsigned char *)
					(task->name.sp+3))));
#endif

//				tprintf("##R7   %08x\n", task->name.r7);
//				tprintf("##Next %08x\n\n", next);

				*naddr = (void *)((INTPOINTER)next);
				*baddr = (void *)((INTPOINTER)next);
				break;
				
			case JMP:
			case JSR:
				{
					// 必ずジャンプする命令

					switch(mp->operand[0].type) {
					case AERD:
						next = task->data[
							((*((unsigned char *)
							    addr+pos)) >> 4) &
							0x07];
						break;
						
					case ABS24:
						next = ((((unsigned long)
							  (*((unsigned char *)
							     addr+pos)))<<16)+
							(((unsigned long)
							  (*((unsigned char *)
							     addr+pos+1)))<<8)+
							(*((unsigned char *)
							     addr+pos+2)));
//						tprintf("##next %06lx\n", next);
						break;
						
					case AA:
					    {
						  unsigned long *p =
							*((unsigned long **)
							((unsigned char *)addr+
							 pos));
							next = *p;
					    }
					    break;
						
					default:
						// 命令データベース
						// (h8300obp)にエラー
						// あり
						break;
					}

					*naddr = (void *)((INTPOINTER)next);
					*baddr = (void *)((INTPOINTER)next);
				}
				break;
				
			case BRA:				// BRA命令？
				{
					// 必ずジャンプする

					if(mp->operand[0].type == BIMM8) {
						
						offset = (*((signed char *)
							    addr+pos));
						
					} else if(mp->operand[0].type
						  == BIMM16) {
						
						offset = (((short)
							   (*((signed char *)
							      addr+pos))<<8)
							  + *((unsigned char *)
							      addr+pos+1));
					}
					
					*naddr = (char *)addr + mp->order.len +
							offset;
					*baddr = (char *)addr + mp->order.len +
							offset;
				}
				break;

			default:
			    {
				  if((BRN <= mp->order.kind) &&
				     (mp->order.kind <=BLE)) {
					  // 分岐命令？
					  
					  if(mp->operand[0].type == BIMM8) {
						  
						  offset = (*((signed char *)
							      addr+pos));
						  
//				tprintf("##IMM8:%d\n", (int)offset);
//				tprintf("##Next:%06lx\n", (long)*baddr);
						  
					  } else if(mp->operand[0].type
						    == BIMM16) {
						  
						  offset = (((short)
							(*((signed char *)
							addr+pos))<<8)
							+ *((unsigned char *)
							addr+pos+1));
					  }
					  
					  *naddr = (char *)addr +
							  mp->order.len;
					  *baddr = (char *)addr +
							  mp->order.len +
							  offset;
					  
				  } else {
					  // ブランチやジャンプしない命令
					  
					  *naddr = (char *)addr +
							  mp->order.len;
					  *baddr = (char *)addr +
							  mp->order.len;
				  }
			    }
			    break;
			}
			
			return mp->order.len;
		}
	}

	return -1;
}

/**
 * @brief	addrのメモリを１命令文逆アセンブルして表示
 * @param[in]	addr	逆アセンブルするメモリ
 * @return	１命令のバイト数
 */
unsigned char disasm(void *addr)
{
	unsigned short wp[MAXPATWD];
	const st_obp *mp;
	unsigned char i, j;

	for(i=0,j=0; i<MAXPATWD; i++, j+=2) {
		wp[i] = ((((unsigned short)*((unsigned char *)addr+j))<<8)
				 + *((unsigned char *)addr+j+1));
	}

	for(mp = h8300obp; mp->mask[0]!=0; mp++) {
		unsigned char wn = MAXPATWD;

		if((mp->order.len/2) < wn) {
			wn = (mp->order.len/2);
		}

		if(compare_bin(wp, mp->mask, mp->pat, wn)) {
			unsigned char i;

			for(i=0; i<10; i++) {
				if(i<mp->order.len) {
					unsigned char b =
						*((unsigned char *)addr + i);
					tprintf("%02X ", (int)b);
				} else {
					tprintf("   ");
				}
			}

			// ニモニック表示
			tprintf("%s", mnemonic[mp->order.kind]);
			if(mp->order.sd) {
				tprintf("%s", msize[mp->order.size]);
			}

			if(mp->operand[0].type != NOOPR) {
				tprintf("\t");
				disp_operand((unsigned char *)addr,
					    mp->operand[0].pos,
					    mp->operand[0].type);
			}

			if(mp->operand[1].type != NOOPR) {
				tprintf(",");
				disp_operand((unsigned char *)addr,
					    mp->operand[1].pos,
					    mp->operand[1].type);
			}
			
			break;
		}
	}

	if(mp->mask[0] == 0) {
		tprintf("%02X %02X                         ",
			(int)*(unsigned char *)addr,
			(int)*((unsigned char *)addr+1));
		tprintf(".data.w\t0x%02x%02x",
			(int)*(unsigned char *)addr,
			(int)*((unsigned char *)addr+1));
	}
	
	if(mp->pat[0] == 0) {
		return 2;
	} else {
		return mp->order.len;
	}
}

/***********************************************************************
  テストプログラム
***********************************************************************/
#ifdef TEST

#include "sformat.h"

#define MAXBUFSIZE 80
unsigned char sbuf[MAXBUFSIZE];
unsigned char bbuf[MAXBUFSIZE/2];

#define MAXCBUF 0x800000
unsigned char cbuf[MAXCBUF];
unsigned long coff = 0;
unsigned long cadd, ladd = 0;

st_task taskReg = {0,0,0,0,0,0,0,0,0,0};

int main(int argc, char *argv[])
{
	if(argc <= 2) {
		printf("Usage: %s S-Format_file offset\n", argv[0]);
	} else {
		FILE *fp;

		if(argc >= 3) {
			coff = hstol(argv[2]);
		}
		
		fp = fopen(argv[1], "r");

		if(fp == NULL) {
			printf("Cannot open file %s.\n", argv[1]);
		}

		{
			int bp;

			for(;;) {
			
				unsigned long add, len;
				unsigned char i;
				
				if(fgets(sbuf, MAXBUFSIZE, fp) == NULL) {
					printf("End:%06lX\n", ladd);
					break;
				}
					
				switch(DecodeSFormat(sbuf,
						     (unsigned char **)&add,
						     &len, bbuf)) {
				case 0:
					*(bbuf+len) = 0;
					printf("File:%s\n", bbuf);
					break;
					
				case 1:
				case 2:
				case 3:
					{
						unsigned char *sp, *dp =
								cbuf+add-coff;

						if(add > ladd) {
							ladd = add;
						}
						
//						printf("%06lX\n", add);
						
						for(sp=bbuf; sp<&bbuf[len];
							sp++) {
							*dp = *sp;
							dp ++;
						}
					}
					break;
					
				case 7:
				case 8:
				case 9:
					{
						add = 0;
						for(i=0; i<len; i++) {
							add = ((add << 8) +
							       bbuf[i]);
						}
						printf("Entry:%08\n", add);
					}
				break;
				
				case -1:
					return;
					break;
				}
			}

			for(cadd = coff; cadd<ladd;) {
				void *nadd, *badd;
				long ol;
				
				printf("%06lX : ", cadd);
				
				cadd += disasm((unsigned char *)cbuf+
					       (cadd-coff));
				printf("\n");
//			calc_next_pc(bbuf+bp, &taskReg, &nadd, &badd);
				
//			printf("Now: %06lx, Next: %06lx, Branche: %06lx\n",
//					   bbuf+bp, (long)nadd, (long)badd);
			}
		}
	}
}

#endif // TEST
