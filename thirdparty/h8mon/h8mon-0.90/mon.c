/** @file
 * @brief	H8/300Hモニタ
 * 
 * @author Takashi SHUDO
 */

#include "mon.h"
#include "task.h"
#include "section.h"
#include "tprintf.h"
#include "commands.h"
#include "shell.h"
#include "stub.h"
#include "conio.h"
#include "monio.h"

static unsigned char mon_mode;	//!< モニタ動作モード

/**
 * @brief	モニタの動作モードを設定する
 * @param	mode	設定する動作モード
 */
void set_monmode(unsigned char mode)
{
	mon_mode = mode;
}

/**
 * @brief	バージョンを表示する
 */
void print_version(void)
{
	extern const char version[];
	extern const char cpu_name[];
	extern const char date[];
	extern const char time[];

	tprintf("\nH8/300(H) %s Monitor Ver. %s", cpu_name, version);
#ifdef DEBUG
	tprintf("(DEBUG Version)");
#endif
	tprintf("\n");

	tprintf("(c)2011 Takashi SHUDO\n");
	tprintf("  Virtual vector table area : "
		ADDR_PRINT_FORMAT " - " ADDR_PRINT_FORMAT "\n",
		(long)VVECT_START, (long)VVECT_END-1);
	tprintf("  Monitor used RAM area     : "
		ADDR_PRINT_FORMAT " - " ADDR_PRINT_FORMAT "\n",
		(long) BSS_START, (long)BSS_END-1);
	tprintf("  Monitor stack end address : "
		ADDR_PRINT_FORMAT "\n", (long)INIT_STACK);
	tprintf("  Build date                : %s %s\n", time, date);
}

#ifdef USE_NET
#include "net/net.h"
extern unsigned char net_act;
#endif
/**
 * @brief	モニタメインループ
 */
void mon(void)
{
	char rd;
	
	switch(mon_mode) {
	case MMODE_MON:
		switch(task_status()) {
		case EXST_ABREAK:
			tprintf("Break.\n");
			
		case EXST_ASTEP:
			// ステップ実行だった
			disp_next_order();
			print_prompt();
			break;
		}
		break;
		
	case MMODE_GDB:
		switch(task_status()) {
		case EXST_ASTEP:
			// ステップ実行だった
			stub_send_signal();
			break;
			
		case EXST_ABREAK:
			// ブレークだった
			stub_send_expected();
			break;
		}
		break;
	}
								  
	for(;;) {
		switch(mon_mode) {
		case MMODE_MON:
#ifdef USE_NET
			if(net_act) {
				proc_network();
			}
#endif
			if(cgetc(&rd)) {
				task_shell(rd);
			}

#ifdef STUB_IS_SCI0
			if(stubgetc(&rd)) {
				if(rd == '$') {
					mon_mode = MMODE_GDB;
					stub_init();
					stub_task(rd);
					tprintf("\n=== Stub mode ===\n");
				}
			}
#else // STUB_IS_SCI0
			if(rd == '$') {
				mon_mode = MMODE_GDB;
				stub_init();
				stub_task(rd);
			}
#endif // STUB_IS_SCI0
			break;

		case MMODE_GDB:
#ifdef STUB_IS_SCI0
			if(stubgetc(&rd)) {
				if(stub_task(rd)) {
					tprintf("\n== Monitor mode ==\n");
					mon_mode = MMODE_MON;
					print_prompt();
				}
			}
#else // STUB_IS_SCI0
			if(cgetc(&rd)) {
				if(stub_task(rd)) {
					mon_mode = MMODE_MON;
					print_prompt();
				}
			}
#endif // STUB_IS_SCI0
			break;
		}
	}
}
