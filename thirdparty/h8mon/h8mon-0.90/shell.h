/** @file
 * @brief	コマンドシェル
 *
 * @date	2007.03.16
 *		2002.03.24
 * @author	Takashi SHUDO
*/

#ifndef	_H_shell
#define	_H_shell

#include "config.h"

// comList.attr 実行後のリターンのみ入力時の動作
#define CMDATTR_CONT	0x0001	// 同じコマンドを実行
#define CMDATTR_ARGLESS	0x0002	// 引数は省略

#define CMDATTR_CTAL	(CMDATTR_CONT|CMDATTR_ARGLESS)

struct shell_command {
	char name[8];
	char sname[4];
	void (*init)(void);
	int (*command)(int argc, char *argv[]);
	unsigned short attr;
	char *help_str;
};

void init_shell(const struct shell_command *coms);
void print_prompt(void);
void dispose_shell_line(void);
int task_shell(char ch);

#endif	/* _H_shell	*/
