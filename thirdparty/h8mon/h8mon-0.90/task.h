/** @file
 * @brief	H8/300Hプログラム起動およびステップ実行、ブレーク機能
 *
 * @date	2002.03.22
 * @author	Takashi SHUDO
 */

#ifndef _H_task
#define _H_task

#ifdef H8NORMAL
#define DisableSystemIRQ	asm("orc.b #0x80,ccr");	//!< 割り込み禁止
#define EnableSystemIRQ		asm("andc.b #0x7f,ccr");//!< 割り込み許可
#define ADDR_PRINT_FORMAT	"%04lX"
typedef	unsigned short	INTPOINTER;		//!< ノーマルモードのポインタ
#else
// UIビットは割り込みマスクとして使用
#define DisableSystemIRQ	asm("orc.b #0x40,ccr");	//!< 割り込み禁止
#define EnableSystemIRQ		asm("andc.b #0xbf,ccr");//!< 割り込み許可
#define ADDR_PRINT_FORMAT	"%06lX"
typedef	unsigned long	INTPOINTER;		//!< 拡張モードのポインタ
#endif

#define TRAPA0	0x5700
#define TRAPA1	0x5710
#define TRAPA2	0x5720
#define TRAPA3	0x5730

#define	BREAK_CODE	TRAPA3	//!< ブレーク用トラップ命令

// ユーザプログラムの実行状態(execStat)
#define EXST_RESET	0	//!< リセット直後(未実行)
#define EXST_ASTEP	1	//!< ステップ実行後
#define EXST_ABREAK	2	//!< ブレーク停止後
#define EXST_EXEC	3	//!< 実行中(実行中なのでモニタでは見られない)
#define EXST_ESTEP	4	//!< ステップ実行中(実行中なのでモニタで
				// は見られない)
#define EXST_NEXEC	5	//!< 現在のPCがブレークだったのでステッ
				// プ実行後の実行待ち
				// (実行中なのでモニタでは見られない)

/**
 * 実行タスクののレジスタ
 */
typedef union {
	unsigned long data[10];
	struct {
		unsigned long r0;
		unsigned long r1;
		unsigned long r2;
		unsigned long r3;
		unsigned long r4;
		unsigned long r5;
		unsigned long r6;
		unsigned long sp;
		unsigned long ccr;
		unsigned long pc;
	} name;
} st_task;

/**
 * ブレークポイント
 */
struct break_pt {
	volatile unsigned short	*addr;	//!< アドレス
	volatile unsigned short	code;	//!< 元のコード
};

extern st_task task_reg;	//!< ユーザタスクのレジスタ状態
extern st_task break_reg;	//!< ユーザタスクのレジスタ状態(SCI割り込み時)
extern struct break_pt break_buf[];	//!< ブレークポイント

void set_break_task(void (* func)(void));
void break_task(void);
unsigned char task_status(void);
void exec_usertask(void);
void step_exec_usertask(void);

void delete_all_breakpoints(void);
void finish_breakpoints(void);
void init_usertask(void);
void init_debuger(void);

#endif // _H_task
