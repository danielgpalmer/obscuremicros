/** @file
 * @brief	コマンドシェル
 *
 * @date	2007.03.16
 *		2002.03.24
 * @author	Takashi SHUDO
*/

#include "shell.h"
#include "lineedit.h"
#include "str.h"
#include "tprintf.h"
#include "history.h"

static struct lineedit	comLine;	//!< コマンドライン
static char lastCom[MAX_LINE_COLUMS+1];	//!< 最後に実行したコマンド

static const struct shell_command *shell_coms;	//!< モニタコマンドデータベース

static struct history his;	//!< コマンドヒストリ

/**
 * @brief	シェルを初期化する
 * @param[in]	coms	初期化するshell_commandのポインタ
 */
void init_shell(const struct shell_command *coms)
{
	const struct shell_command *cp = coms;
	shell_coms = coms;
	lastCom[0] = 0;

	/*
	 * 各コマンドの初期化
	 */
	while(cp->name[0] != 0) {
		if(cp->init != 0) {
			cp->init();
		}
		cp ++;
	}

	init_history(&his);
	init_lineedit(&comLine);
}

static const char prompt[] = ">";	//!< プロンプト文字列

/**
 *	@brief	プロンプトを表示する
 */
void print_prompt(void)
{
	tprintf((char *)prompt);
}

/**
 * @brief	編集中のコマンドラインを破棄する
 */
void dispose_shell_line(void)
{
	new_lineedit(&comLine);
}


#define	MAXARGV	10	//!< コマンド引数の最大数

static char *argv[MAXARGV];	//!< コマンド引数
static const char nstr[] = "";	//!< NULL文字

/**
 * @brief	str文字列のコマンドを実行する
 * @param	str	コマンド文字列ポインタ
 * @return	コマンド実行結果
 */
static int exec_shell_command(char *str)
{
	const struct shell_command *cp = shell_coms;
	int argc = 0;
	int ex = 0;
	int rt = 0;
	int i;
	int flgc = 0;
	char *tk;

	if(*str == 0) {
		flgc = 1;
		if(lastCom[0] != 0) {
			str = lastCom;
		} else {
			return 0;
		}
	} else {
		strcopy(lastCom, str);
	}

	tk = str;
	
//	str2small(str);	// 小文字へ変換

	/*
	 * トークンを区切る
	 */
	for(i=0; i<MAXARGV; i++) {
		argv[i] = (char *)nstr;
	}

	for(i=0; i<MAXARGV; i++) {
		/*
		 * ' '以外の文字が先頭
		 */
		while(tk < &str[MAX_LINE_COLUMS]) {
			if((*tk == 0) || (*tk == CR)) {
				*tk = 0;
				goto next;
			} else if(*tk != ' ') {
				argv[i] = tk;
				argc ++;
				tk ++;
				break;
			}
			
			tk ++;
		}
		
		/*
		 *	' 'まで読み捨て
		 */
		while(tk < &str[MAX_LINE_COLUMS]) {
			if((*tk == 0) || (*tk == CR)) {
				*tk = 0;
				goto next;
			} else if(*tk == ' ') {
				*tk = 0;
				tk ++;
				break;
			}
			
			tk ++;
		}
	}
	
 next:

	while(cp->name[0] != 0) {
	    if((strcomp((char *)cp->name, argv[0]) == 0)
	       || (strcomp((char *)cp->sname, argv[0]) == 0)) {
		    if(cp->command != 0) {
			    if(flgc) {
				    if(cp->attr & CMDATTR_CONT) {
					    if(cp->attr & CMDATTR_ARGLESS) {
						    argc = 1;
					    }
					    rt = cp->command(argc, argv);
					    ex = 1;
				    } else {
					    goto end;
				    }
			    } else {
				    rt = cp->command(argc, argv);
				    ex = 1;
			    }
			    break;
		    }
	    }
	    cp ++;
	}
	
	if(ex == 0) {
		lastCom[0] = 0;
		tprintf("Invalid Command.\n");
	}

 end:
	
	return rt;
}


/**
 * @brief	文字列編集タスク
 * @param	ch	編集文字データ
 * @return	コマンド実行結果
 */
int task_shell(char ch)
{
	int rt;

	if(ch == CTRL_C) {	// CTRL-C ?
		init_lineedit(&comLine);
		tprintf("\n");
		print_prompt();
	} else {
		switch(do_lineedit(&comLine, ch)) {
		case LER_NOP:
			break;
			
		case LER_RETURN: // '\r'受信でコマンド実行
			save_history(&his, comLine.buf);
			rt = exec_shell_command(comLine.buf);
			new_lineedit(&comLine);
			if(rt) {
				return rt;
			}
			print_prompt();
			return rt;
			break;

		case LER_BACKLINE:
			back_history(&his, &comLine);
			break;
			
		case LER_NEXTLINE:
			foward_history(&his, &comLine);
			break;
		}
	}

	return 0;
}

