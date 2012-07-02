/** @file
 * @brief	H8/300Hプログラム起動およびステップ実行、ブレーク機能
 *
 * @date	2002.03.22
 * @author	Takashi SHUDO
 */

#include "task.h"
#include "disasm.h"
#include "config.h"

extern void _exec(void);
extern void _exec_mon(void);

st_task	task_reg;	//!< ユーザタスクのレジスタ状態
st_task	break_reg;	//!< ユーザタスクのレジスタ状態(SCI割り込み時)

struct break_pt	break_buf[MAX_BREAKPOINT_NUM];	//!< ブレークポイント

static unsigned char task_stat;		//!< ユーザタスクの実行状態
static struct break_pt next_order;	//!< ステップ実行後のアドレス、命令
static struct break_pt bran_order;	//!< ブランチ実行後のアドレス、命令

static void (* break_func)(void);	//!< break時に実行されるユーザ関数

/**
 * @brief	ブレーク発生時に実行される関数を設定する
 */
void set_break_func(void (* func)(void))
{
	break_func = func;
}

/**
 * @brief	実行中のタスクをブレーク停止状態へ
 */
void break_task(void)
{
	if(break_func != 0) {
		break_func();
	} else {
		if(task_stat == EXST_EXEC) {
			task_reg = break_reg;
			task_stat = EXST_ABREAK;	// ブレーク停止状態へ
			
			_exec_mon();
		}
	}

	return;
}

/**
 * @brief	ステップ実行だった？
 * @return	実行状態
 */
unsigned char task_status(void)
{
	return task_stat;
}

/**
 * @brief	次のブランチも含む実行アドレスにトラップを書き込む
 */
static void prepare_trap_next_order(void)
{
	calc_next_pc((void *)((INTPOINTER)task_reg.name.pc), &task_reg,
		     (void *)&next_order.addr, (void *)&bran_order.addr);
	
	next_order.code	= *next_order.addr;	// 保存して
	*next_order.addr = BREAK_CODE;		// そこにブレーク
	
	if(next_order.addr != bran_order.addr) {
		bran_order.code	= *bran_order.addr;	// 保存して
		*bran_order.addr = BREAK_CODE;		// そこにブレーク
	}
}

/**
 * @brief	次の実行アドレスがブレークポイント？
 * @return	1:次はブレーク
 */
static unsigned char next_order_is_break(void)
{
	short i;

	for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
		if(task_reg.name.pc == (INTPOINTER)break_buf[i].addr) {
			return 1;
		}
	}

	return 0;
}

/**
 * @brief 全てのブレークポイントを設定したアドレスにトラップ命令を書き込む
 */
static void prepare_breakpoints(void)
{
	long i;

	for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
		if(break_buf[i].addr != 0) {
			*break_buf[i].addr = BREAK_CODE;
		}
	}
}

/**
 * @brief	ユーザプログラム実行
 */
void exec_usertask(void)
{
	if(next_order_is_break()) {
		// 実行しようとした命令がブレークポイントだった

		task_stat = EXST_NEXEC;	// トラップ後実行しなければならない
		prepare_trap_next_order();

	} else {
		task_stat = EXST_EXEC;	// 実行状態へ
		prepare_breakpoints();	// ブレークトラップを書き込んで
	}
	
	_exec();	// 実行
}

/**
 * @brief	ユーザタスクステップ実行
 */
void step_exec_usertask(void)
{
	prepare_trap_next_order();
	
	task_stat = EXST_ESTEP;		// ステップ実行のフラグ設定

	_exec();	// 実行
}

/*****************************************************************************
	ブレーク機能

	ブレーク機能はTRAPA命令割込みを使用
*****************************************************************************/

/**
 * @brief	全ブレークポイントを削除
 */
void delete_all_breakpoints(void)
{
	long i;

	for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
		break_buf[i].addr = 0;
	}
}

/**
 * @brief	全ブレークポイントを設定したアドレスのトラップ命令を戻す
 */
void finish_breakpoints(void)
{
	long i;

	for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
		if(break_buf[i].addr != 0) {
			*break_buf[i].addr = break_buf[i].code;	// 命令を戻す
		}
	}

	if(task_stat == EXST_ESTEP) {	// ステップ実行だった？

		*next_order.addr = next_order.code;	// 保存してた命令を戻す
		if(next_order.addr != bran_order.addr) {
			*bran_order.addr = bran_order.code;	// 保存してた命令を戻す
		}
		
		task_stat = EXST_ASTEP;		// ステップ実行後状態へ

	} else if(task_stat == EXST_NEXEC) {	// 次が実行のステップ実行だった？

		*next_order.addr = next_order.code;	// 保存してた命令を戻す
		if(next_order.addr != bran_order.addr) {
			*bran_order.addr = bran_order.code;	// 保存してた命令を戻す
		}

		task_stat = EXST_EXEC;	// 実行状態へ
		prepare_breakpoints();	// ブレークトラップを書き込んで
		
		_exec();	// もう一度実行
	} else {
		task_stat = EXST_ABREAK;	// ブレーク停止状態へ
	}
}

/**
 * @brief	ユーザプログラムの状態初期化
 */
void init_usertask(void)
{
	int i;
	
	extern const INTPOINTER init_pc_addr;	// ユーザプログラムのPC初期値
	extern const INTPOINTER init_sp_addr;	// ユーザプログラムのSP初期値

	task_reg.name.pc = break_reg.name.pc = init_pc_addr;	// PCの初期化
	task_reg.name.sp = break_reg.name.sp = init_sp_addr;	// SPの初期化
	task_reg.name.ccr= break_reg.name.ccr= 0x80;
	
	for(i=0; i<7; i++) {
		task_reg.data[i] = break_reg.data[i] = 0x0000000;
	}
	
	task_stat = EXST_RESET;
	break_func = 0;
	
	delete_all_breakpoints();
}

/**
 * @brief	デバッガ初期化
 */
void init_debuger(void)
{
	EnableSystemIRQ;	// 割り込み許可
}
