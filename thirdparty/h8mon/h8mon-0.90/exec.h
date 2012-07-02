/** @file
 * @brief  H8/300Hプログラム起動およびステップ実行、ブレーク機能
 *
 * @date	2002.03.22
 * @author	Takashi Shudo
 */

#ifndef _H_task
#define _H_task

#define TRAPA0	0x5700
#define TRAPA1	0x5710
#define TRAPA2	0x5720
#define TRAPA3	0x5730

#define	BREAK_CODE	TRAPA3	//!< ブレーク用トラップ命令

#define MAXBREAK	8	//!< 設定可能な最大ブレークポイント数

// ユーザプログラムの実行状態(execStat)
#define EXST_RESET	0	//!< リセット直後(未実行)
#define EXST_ASTEP	1	//!< ステップ実行後
#define EXST_ABREAK	2	//!< ブレーク停止後
#define EXST_EXEC	3	//!< 実行中(実行中なのでモニタでは見られない)
#define EXST_ESTEP	4	//!< ステップ実行中(実行中なのでモニタで
				// は見られない)
#define EXST_NEXEC	5	//!< 現在のPCがブレークだったのでステッ
				// プ実行後の実行待ち(実行中なのでモニ
				// タでは見られない)

/**
 * 実行プログラムののレジスタ
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
		unsigned long r7;
		unsigned long ccr;
		unsigned long pc;
	} name;
} st_task;

/**
 * ブレークポイント
 */
typedef	struct {
	volatile unsigned short	*addr;	//!< アドレス
	volatile unsigned short	code;	//!< 元のコード
} breakPt;

unsigned char ExecStatus(void);
void ExecUserProgram(void);
void StepExecUserProgram(void);

void DeleteAllBreakpoints(void);
void PrepareBreakpoints(void);
void FinishBreakpoints(void);
void InitUserProgram(void);

#endif // _H_task
