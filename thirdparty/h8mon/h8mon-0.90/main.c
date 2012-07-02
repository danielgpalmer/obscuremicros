/** @file
 * @brief	h8monメイン
 *
 * @date	2008.04.28
 * @author	 Takashi SHUDO
 */

/** \mainpage
 *
 * h8monはH8/3069用のモニタソフトです。
 */

#include "vect.h"
#include "str.h"
#include "shell.h"
#include "tprintf.h"
#include "lineedit.h"
#include "stub.h"
#include "task.h"
#include "mon.h"
#include "monio.h"

const extern struct st_monio sci0_monio;
const extern struct st_monio sci1_monio;
extern const struct shell_command mon_com[];
extern int check_application(void);

/**
 * @brief	モニタ本体
 */
int main(int argc, char *argv[])
{
	init_vectortable();
	set_monmode(MMODE_MON);
	
	init_debuger();

	init_monio(&sci1_monio);
#ifdef STUB_IS_SCI0
	init_stubio(&sci0_monio);
#endif

	print_version();

	init_usertask();
	init_shell(mon_com);

	stub_init();

	if(check_application()) {
		task_reg.name.ccr = 0x00;
		task_reg.name.pc = (INTPOINTER)APP_ADDRESS;
		exec_usertask();
	} else {
		print_prompt();
		mon();
	}
	
	return 0;
}
