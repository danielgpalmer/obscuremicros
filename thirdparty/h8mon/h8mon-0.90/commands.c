/** @file
 * @brief	モニタのコマンド
 *
 * @date	2002.03.02
 * @author	Takashi SHUDO
 */

#include "shell.h"
#include "lineedit.h"
#include "str.h"
#include "conio.h"
#include "sformat.h"
#include "tprintf.h"
#include "task.h"
#include "disasm.h"
#include "section.h"
#include "commands.h"

#define	MAXBUF	80	//!< デコード出来るS-recordsデータの１行の長さ
static char sbuf[MAXBUF];	//!< 文字列バッファ

/**
 * @brief	S-recordsデータのロード
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_load(int argc, char *argv[])
{
	unsigned char *add;
	unsigned char *tadd = (unsigned char *)0xffffffff;
	unsigned char *badd = 0;
	unsigned long len;
	unsigned char bbuf[MAXBUF/2];
	unsigned char flgXflow = 0;
	unsigned char flgVfy = 0;
	unsigned char flgAddDsp = 0;
	short i;

	for(i=1; i<argc; i++) {
		switch(argv[i][0]) {
		case 'x':
			tprintf("X-Flow On.\n");
			flgXflow = 1;
			break;
			
		case 'v':
			tprintf("Verify mode.\n");
			flgVfy = 1;
			break;
			
		case 'a':
			flgAddDsp = 1;
			break;
		}
	}

	tprintf("Please send S-records data.\n");

	/*
	 * １行読み込み
	 */
	for(;;) {

		char rd = 0;
		char *dp = sbuf;
		
		for(;;) {
			long rt = 0;
			while(rt == 0) {
				rt = cgetc(&rd);
			}

			if(rd == CTRL_C) {
				tprintf("Canceled.\n");
				return 0;
			}

			if(rd == '\n') {
				// 
			} else if(rd != '\r') {	/* 改行	*/
				*dp = rd;
				dp ++;
				if(dp >= &sbuf[MAXBUF]) {
					tprintf("Line too long.\n");
					return 0;
				}
			} else {
				*dp = 0;
				break;
			}
		}

		if(flgXflow) {
			cputc(XON);	// X Stop
		}
		
		switch(DecodeSFormat(sbuf, &add, &len, bbuf)) {
		case 0:
			// ファイル名
			if(flgAddDsp) {
				bbuf[len] = 0;
				tprintf("File : %s\n", bbuf);
			}
			break;
			
		case 1:
		case 2:
		case 3:
			// データ
			{
				unsigned char *sp, *dp = bbuf;
				if(tadd > add) {
					tadd = add;
				}
				if(badd < (add+len)) {
					badd = (add+len);
				}

				if(flgAddDsp) {
					tprintf(ADDR_PRINT_FORMAT "\r",
						(long)(INTPOINTER)add);
				}

				for(sp=add; sp<(add+len); sp++) {
					unsigned char *wp = sp;
					
					// ベクタテーブルへの書き込み
					// は仮想ベクタのアドレスへ
					if((VECT_START <= (INTPOINTER)sp) &&
					   ((INTPOINTER)sp < VECT_END)) {
						wp += VVECT_START;
					}
					if(flgVfy == 0) {
						*wp = *dp;
					}
					if(*wp != *dp) {
						tprintf("Verify Error at "
							ADDR_PRINT_FORMAT
							" W:%02X R:%02X\n",
							wp, (int)*dp, (int)*wp);
					}
					dp ++;
				}
			}
			break;

		case 7:
		case 8:
		case 9:
			// 終了、エントリーアドレス設定
			{
				volatile long i;
				
				add = 0;
				for(i=0; i<len; i++) {
					add = (unsigned char *)
						((((INTPOINTER)add) << 8)
						 + bbuf[i]);
				}
				task_reg.name.pc = (INTPOINTER)add;

				tprintf("Load address     : "
					ADDR_PRINT_FORMAT " - "
					ADDR_PRINT_FORMAT "\n",
					(long)((INTPOINTER)tadd),
					(long)((INTPOINTER)badd)-1);
				tprintf("Excution address : "
					ADDR_PRINT_FORMAT "\n",
					(long)((INTPOINTER)add));
				tprintf("Done.\n");
				goto end;
			}
			break;

		case -1:// フォーマットエラー
			tprintf("Format error:%s\n", sbuf);
			goto end;
			break;
		}
		
		if(flgXflow) {
			cputc(XOFF);	// X Start
		}
	}

 end:
	
	if(flgXflow) {
		cputc(XOFF);	// X Start
	}
	
	return 0;
}

/*----------------------------------------------------------------------------
  メモリダンプ
----------------------------------------------------------------------------*/
static unsigned char *mdAddr;	//!< ダンプアドレス
static unsigned char dwi;	//!< 1:byte 2:word 4:long

/**
 * @brief	メモリダンプの初期化
 */
static void init_memdump(void)
{
	mdAddr = 0;
	dwi = 1;
}

/**
 * @brief	メモリダンプ
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_memdump(int argc, char *argv[])
{
	unsigned char *ed = mdAddr + 256;
	unsigned char *dp;
	unsigned char an = 1;

	if(argc > 1) {
		if(argv[1][0] == '-') {
			switch(argv[1][1]) {
			case 'b':
				dwi = 1;
				break;
				
			case 'w':
				dwi = 2;
				break;
				
			case 'l':
				dwi = 4;
				break;
			}
			
			an ++;
		} else {
			dwi = 1;
		}
	}
	
	if(argc > an) {
		mdAddr = (void *)(INTPOINTER)hstol(argv[an]);
		an ++;
	}
	
	ed	= mdAddr + 256;
		
	if(argc > an) {
		ed	= (unsigned char *)(INTPOINTER)hstol(argv[an]) + 1;
	}
	
	for(dp=mdAddr; dp<ed; dp+=16) {
		unsigned char *p;
		char rd;
		
		tprintf(ADDR_PRINT_FORMAT " : ", (long)(INTPOINTER)dp);
		
		for(p=dp; p<dp+8; p+=dwi) {
			if(p < ed) {
				switch(dwi) {
				case 1:	tprintf("%02X ", (int)*p);		break;
				case 2:	tprintf("%04X ", (int)*(short *)p);	break;
				case 4:	tprintf("%08lX ", (long)*(long *)p);	break;
				}
			} else {
				tprintf("   ");
			}
		}
		tprintf(" ");
		for(p=dp+8; p<dp+16; p+=dwi) {
			if(p < ed) {
				switch(dwi) {
				case 1:	tprintf("%02X ", (int)*p);		break;
				case 2:	tprintf("%04X ", (int)*(short *)p);	break;
				case 4:	tprintf("%08lX ", (long)*(long *)p);	break;
				}
			} else {
				tprintf("   ");
			}
		}
		
		tprintf("  \"");
		
		for(p=dp; p<dp+16; p++) {
			if(p < ed) {
				if(((' ' <= *p) && (*p <= 'Z'))
				   || (('a' <= *p) && (*p <= 'z'))) {
					cputc(*p);
				} else {
					cputc('.');
				}
			} else {
				break;
			}
		}
		tprintf("\"\n");
		
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 0;
			}
		}
	}
	
	mdAddr = ed;

	return 0;
}

/*----------------------------------------------------------------------------
  メモリ編集
----------------------------------------------------------------------------*/
static void *meAddr;	//!< 編集アドレス
static unsigned char wi;//!< 0:byte 1:word 2:long

/**
 * @brief	メモリ編集の初期化
 */
static void init_memedit(void)
{
	meAddr = 0;
	wi = 0;
}

/**
 * @brief	メモリ編集
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_memedit(int argc, char *argv[])
{
	unsigned char an = 1;

	if(argc > 1) {
		if(argv[1][0] == '-') {
			switch(argv[1][1]) {
			case 'b':
				wi = 0;
				break;
				
			case 'w':
				wi = 1;
				break;
				
			case 'l':
				wi = 2;
				break;
			}
			
			an ++;
		} else {
			wi = 0;
		}
	}
	
	if(argc > an) {
		meAddr = (void *)(INTPOINTER)hstol(argv[an]);
		an ++;
	}
		
	if(argc > an) {
		// メモリ書き込み値あり

		short i;

		for(i=an; i<argc; i++) {

			switch(wi) {
			case 0:
				*(unsigned char *)meAddr =
						(unsigned char)hdstol(argv[i]);
				meAddr ++;
				break;
				
			case 1:
				*(unsigned short *)meAddr =
						(unsigned short)hdstol(argv[i]);
				meAddr += 2;
				break;
				
			case 2:
				*(unsigned long *)meAddr =
						(unsigned long)hdstol(argv[i]);
				meAddr += 4;
				break;
			}
		}
	} else {
		// メモリ書き込み値無し

		tprintf(ADDR_PRINT_FORMAT " : ", (long)(INTPOINTER)meAddr);

		switch(wi) {
		case 0:
			tprintf("%02X", (int)*(unsigned char *)meAddr);
			meAddr ++;
			break;

		case 1:
			tprintf("%04X", (int)*(unsigned short *)meAddr);
			meAddr += 2;
			break;

		case 2:
			tprintf("%08lX", (long)*(unsigned long *)meAddr);
			meAddr += 4;
			break;
		}
		
		tprintf("\n");
	}

	return 0;
}

/*****************************************************************************
	レジスタ設定
*****************************************************************************/
#define RN_R0	0
#define RN_R1	1
#define RN_R2	2
#define RN_R3	3
#define RN_R4	4
#define RN_R5	5
#define RN_R6	6
#define RN_R7	7
#define RN_SP	7
#define RN_CCR	8
#define RN_PC	9

static const char regName[][4] = {
	"R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7", 
	"CCR", "PC",
	""
}; //!< レジスタ名文字列

/**
 * @brief	プログラム実行
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_go(int argc, char *argv[])
{
	if(argc == 2) {
		task_reg.name.pc = hstol(argv[1]);
	}
	
	dispose_shell_line();	// 戻れないのでここで編集ラインは破棄
	exec_usertask();
	
	return 0;
}

/**
 * @brief	レジスタ表示
 */
static void disp_regs(void)
{
	unsigned char i, j;

	tprintf("%s = %06lX	 ", (char *)regName[9], task_reg.name.pc);
	tprintf("%s = %02X	 ", (char *)regName[8], task_reg.name.ccr);
	tprintf("\n");
	
	for(j=0; j<2; j++) {
		for(i=j*4; i<j*4+4; i++) {
			tprintf("%s = %08lX	 ",
				(char *)regName[i], task_reg.data[i]);
		}
		tprintf("\n");
	}
}

/**
 * @brief	レジスタ編集
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_reg(int argc, char *argv[])
{
	switch(argc) {
	case 1:
		disp_regs();
		break;

	case 2:
		/*
		 * "r regname"の処理
		 */
		{
			short i = 0;
			
			while(regName[i][0] != 0) {
				if(strcomp((char *)regName[i],
					   str2cap(argv[1])) == 0) {
					tprintf("%s = %08lX\n",
						(char *)regName[i],
						task_reg.data[i]);
					return 0;
				}
				i ++;
			}
			
			tprintf("Register:%s unknown.\n", argv[1]);
		}
		break;

	case 3:
		/*
		 * "r regname val"の処理
		 */ 
		{
			short i = 0;
			
			while(regName[i][0] != 0) {
				if(strcomp((char *)regName[i],
					   str2cap(argv[1])) == 0) {
					long val = hstol(argv[2]);
					task_reg.data[i] = val;
					tprintf("%s = %08lX\n",
						(char *)regName[i],
						task_reg.data[i]);
					return 0;
				}
				i ++;
			}
			
			tprintf("Register:%s unknown.\n", argv[1]);
		}
		break;
	}
	
	return 0;
}

/*----------------------------------------------------------------------------
  逆アセンブラ初期化
----------------------------------------------------------------------------*/
#define MAXDABYTES	48	//!< 1回の逆アセンブルバイト数

static void *daAddr;	//!< 逆アセンブルアドレス

/**
 * @brief	逆アセンブル初期化
 */
static void init_disa(void)
{
	daAddr = (unsigned char *)(INTPOINTER)task_reg.name.pc;
}

/**
 * @brief	逆アセンブル表示
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_disa(int argc, char *argv[])
{
	void *addr, *endAddr = daAddr + MAXDABYTES;

	switch(argc) {
	case 1:
		endAddr	= daAddr + MAXDABYTES;
		break;
		
	case 2:
		if(argv[1][0] == 'p') {
			daAddr	= (unsigned char *)
					((INTPOINTER)task_reg.name.pc - 16);
		} else {
			daAddr	= (unsigned char *)(INTPOINTER)hstol(argv[1]);
		}
		endAddr	= daAddr + MAXDABYTES;
		break;
		
	case 3:
		daAddr	= (unsigned char *)(INTPOINTER)hstol(argv[1]);
		endAddr	= (unsigned char *)(INTPOINTER)hstol(argv[2]) + 1;
	}
	
//	daAddr	= (unsigned char *)((INTPOINTER)daAddr & 0xfffffffe);
//	endAddr	= (unsigned char *)((INTPOINTER)endAddr & 0xfffffffe);
	
	for(addr = daAddr; addr<endAddr;) {
		char rd;
		
		tprintf(ADDR_PRINT_FORMAT " : ", (long)(INTPOINTER)addr);
			
		addr += disasm(addr);
			
		tprintf("\n");
			
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 0;
			}
		}
	}
		
	daAddr = addr;
		
	return 0;
}

/**
 * @brief	次に実行する命令を逆アセンブル表示
 */
void disp_next_order(void)
{
	disp_regs();
	
	tprintf(ADDR_PRINT_FORMAT " : ", task_reg.name.pc);
	
	disasm((void *)(INTPOINTER)task_reg.name.pc);

	tprintf("\n");
}

/**
 * @brief	ステップ実行
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_step(int argc, char *argv[])
{
	dispose_shell_line();	// 戻れないのでここで編集ラインは破棄
	step_exec_usertask();

	return 0;
}


/**
 * @brief	ブレークポイント設定
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_break(int argc, char *argv[])
{
	int i;

	switch(argc) {
	case 1:
		for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
			if(break_buf[i].addr != 0) {
				tprintf("%1d : " ADDR_PRINT_FORMAT "\n",
					i+1,
					(long)(INTPOINTER)break_buf[i].addr);
			}
		}
		break;
		
	case 2:
		{
			if(argv[1][0] == '-') {
				
				delete_all_breakpoints();
				
				tprintf("Deleted all breakpoints.\n");
				
			} else {
				volatile unsigned short *addr
					= (unsigned short *)(INTPOINTER)
						(hstol(argv[1]) & 0xfffffffe);

				// 設定済みのアドレスかを確認
				for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
					if(break_buf[i].addr ==  addr) {
						tprintf("Address "
							ADDR_PRINT_FORMAT
							" allready set to"
							" breakpoint.\n",
						(long)(INTPOINTER)addr);
						return 0;// 設定済みだった
					}
				}

				for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
				  if(break_buf[i].addr == 0) {// 未設定？

					  // 元の命令を保存
					  break_buf[i].code = *addr;
					  
					  // アドレスがROMかを確
					  // 認するため一度設定
					  *addr = BREAK_CODE;
					  
					  // 書けた？
					  if(*addr == BREAK_CODE) {
						  // 書けた
						  
						  break_buf[i].addr = addr;
						  // アドレス保存
						  *addr = break_buf[i].code;
						  // 元に戻す
						  
						  tprintf("Set breakpoint at "
							  ADDR_PRINT_FORMAT "\n",
							  (long)(INTPOINTER)addr);
					  } else {
						  // 書けない
						  
						  *addr = break_buf[i].code;
						  // 元に戻す
						  tprintf("Cannot Set breakpoint at "
							  ADDR_PRINT_FORMAT
							  ".\n",
							  (long)(INTPOINTER)addr);
					  }
					  break;
				  }
				}
				
				if(i == MAX_BREAKPOINT_NUM) {
					tprintf("Breakpoint Full .\n");
				}
			}
		}
		break;

	case 3:
		if(argv[1][0] == '-') {
			if(strleng(argv[2]) == 1) {
				// Break # で消去

				unsigned char num = dstol(argv[2]);

				if((1 <= num) && (num <= MAX_BREAKPOINT_NUM)) {
					break_buf[num-1].addr = 0;
					tprintf("Deleted breakpoint #%d.\n",
						(long)num);
				}
			} else {
				// アドレス指定で消去
				
				volatile unsigned short *addr
					= (unsigned short *)(INTPOINTER)
						(hstol(argv[2]) & 0xfffffffe);
			
				for(i=0; i<MAX_BREAKPOINT_NUM; i++) {
				  if(break_buf[i].addr == addr) {
					  break_buf[i].addr = 0;
					  tprintf("Deleted breakpoint address "
						  ADDR_PRINT_FORMAT ".\n",
						  (long)(INTPOINTER)addr);
				  }
				}
			}
		}
		break;
	}
	
	return 0;
}

/**
 * @brief	メモリフィル
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_fill(int argc, char *argv[])
{
	unsigned char *wa, *sa, *ea, dt;
    
	if(argc >=  4) {
		sa = (unsigned char *)(INTPOINTER)hstol(argv[1]);
		if(argv[2][0] == '@') {
			ea = sa + hdstol(&argv[2][1]);
		} else {
			ea = (unsigned char *)(INTPOINTER)hstol(argv[2]);
        }
		dt = (unsigned char)hstol(argv[3]);

		for(wa = sa; wa <= ea; wa ++) {
			*wa = dt;
		}
	}

    return 0;
}

static int memory_check(unsigned long *sp, unsigned long *ep,
			 unsigned long data)
{
	unsigned long *dp;
	char rd;
	int err = 0;

	tprintf("Writing %08lX...\n", data);
	
	for(dp=sp; dp<=ep; dp++) {
		*dp = data;
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 1;
			}
		}
	}

	tprintf("Checking...");
	for(dp=sp; dp<=ep; dp++) {
		if(*dp != data) {
			err = 1;
			tprintf("\nVerify Error at " ADDR_PRINT_FORMAT
				" W:%08lX R:%08lX", dp, data, *dp);
		}
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 1;
			}
		}
	}

	if(err) {
		tprintf("\nNG.\n");
	} else {
		tprintf("OK.\n");
	}

	return 0;
}

static int memory_check_a(unsigned long *sp, unsigned long *ep)
{
	unsigned long *dp;
	char rd;
	int err = 0;

	tprintf("Writing address data...\n");
	
	for(dp=sp; dp<=ep; dp++) {
		*dp = (unsigned long)dp;
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 1;
			}
		}
	}

	tprintf("Checking...");
	for(dp=sp; dp<=ep; dp++) {
		if(*dp != (unsigned long)dp) {
			err = 1;
			tprintf("\nVerify Error at " ADDR_PRINT_FORMAT
				" W:%08lX R:%08lX", dp, (unsigned long)dp,
				*dp);
		}
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 1;
			}
		}
	}

	if(err) {
		tprintf("\nNG.\n");
	} else {
		tprintf("OK.\n");
	}

	return 0;
}

static int memory_check_ar(unsigned long *sp, unsigned long *ep)
{
	unsigned long *dp;
	char rd;
	int err = 0;

	tprintf("Writing invert address data...\n");
	
	for(dp=sp; dp<=ep; dp++) {
		*dp = ~((unsigned long)dp);
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 1;
			}
		}
	}

	tprintf("Checking...");
	for(dp=sp; dp<=ep; dp++) {
		if(*dp != ~((unsigned long)dp)) {
			err = 1;
			tprintf("\nVerify Error at " ADDR_PRINT_FORMAT
				" W:%08lX R:%08lX", dp, (unsigned long)dp,
				*dp);
		}
		if(cgetc(&rd) == 0) {
			if(rd == CTRL_C) {
				tprintf("Abort.\n");
				return 1;
			}
		}
	}

	if(err) {
		tprintf("\nNG.\n");
	} else {
		tprintf("OK.\n");
	}

	return 0;
}

#define MEMCHK
#ifdef MEMCHK
/**
 * @brief	メモリチェック
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_memchk(int argc, char *argv[])
{
	unsigned long *sp = (unsigned long *)0x400000;
	unsigned long *ep = (unsigned long *)0x5fffff;

	if(argc > 1) {
		sp = (unsigned long *)hstol(argv[1]);
	}

	if(argc > 2) {
		ep = (unsigned long *)hstol(argv[2]);
	}

	tprintf("Memory check.\n");
	tprintf("Start  : " ADDR_PRINT_FORMAT "\n", (long)sp);
	tprintf("End    : " ADDR_PRINT_FORMAT "\n", (long)ep);

	if(memory_check(sp, ep, 0x00000000)) goto end;
	if(memory_check(sp, ep, 0xffffffff)) goto end;
	if(memory_check(sp, ep, 0xaaaaaaaa)) goto end;
	if(memory_check(sp, ep, 0x55555555)) goto end;
	if(memory_check_a(sp, ep)) goto end;
	if(memory_check_ar(sp, ep)) goto end;
	if(memory_check(sp, ep, 0x55aa55aa)) goto end;
	if(memory_check(sp, ep, 0x00ff00ff)) goto end;
	if(memory_check(sp, ep, 0x00000000)) goto end;

end:
	
	return 0;
}
#endif

#ifdef USE_NET

#include "net/rtl8019as.h"
#include "net/ip.h"
#include "net/net.h"
#include "net/netstr.h"

unsigned char net_act;
static struct IP_ADDR ipaddr;

/**
 * @brief	IP アドレス設定
 */
static void init_ipaddr(void)
{
	net_act = 0;
}

static void print_ipaddr(void)
{
	tprintf("IP address : %d.%d.%d.%d\n",
		ipaddr.addr[0],
		ipaddr.addr[1],
		ipaddr.addr[2],
		ipaddr.addr[3]);
}

static int com_ipaddr(int argc, char *argv[])
{
	if(argc < 2) {
		if(net_act == 0) {
			tprintf("Usage : %s <address>\n", argv[0]);
		} else {
			print_ipaddr();
		}
		
		return 0;
	}

	if(str2ipaddr(argv[1], &ipaddr) != 0) {
		tprintf("Invalid address \"%s\"\n", argv[1]);
		return 0;
	}

	print_ipaddr();

	if(rtl8019as_init() != 0) {
		return 0;
	}

	rtl8019as_open();
	init_network();
	set_ipaddr(&ipaddr);
	net_act = 1;
	tprintf("Start network\n");
	
	return 0;
}

#include "net/udp.h"
#include "net/tftpget.h"
#include "net/elf.h"

#define DEF_LOAD_ADDR	0x500000

static int com_tftpb(int argc, char *argv[])
{
	struct IP_ADDR ipaddr;
	char *fname;
	long rcv_size;

	unsigned char flg_dw = 0;
	unsigned char *elfp = (unsigned char *)DEF_LOAD_ADDR;
	unsigned long entry;
	int rt;
	
	if(net_act == 0) {
		tprintf("Network not active\n");
		return 0;
	}
	
	tprintf("TFTP boot\n");
	
	if(argc < 3) {
		tprintf("Usage: %s [-d] <ipaddr> <filename> [loadaddr]\n");
		return 0;
	}

	if(argv[1][0] == '-') {
		switch(argv[1][1]) {
		case 'd':
			tprintf("Download only\n");
			flg_dw = 1;
			break;

		default:
			tprintf("Invalid option \"%c\"\n", argv[1][1]);
			return 0;
			break;
		}
	}
	
	if(str2ipaddr(argv[flg_dw + 1], &ipaddr) != 0) {
		tprintf("Invalid address \"%s\"\n", argv[1]);
		return 0;
	}

	fname = argv[flg_dw + 2];

	if((argc + flg_dw) >= 4) {
		elfp = (unsigned char *)hstol(argv[flg_dw + 3]);
	}
	
	tprintf("Target IP : %d.%d.%d.%d\n", ipaddr.addr[0], ipaddr.addr[1],
		ipaddr.addr[2], ipaddr.addr[3]);
	tprintf("File name : %s\n", fname);
	tprintf("Load Addr : %06lX\n", (INTPOINTER)elfp);

	rt = tftp_get(&ipaddr, fname, elfp, &rcv_size);

	if(rt != 0) {
		tprintf("TFTP get abort\n");
		return 0;
	}

	if(flg_dw != 0) {
		// ダウンロードのみ
		return 0;
	}
	
	if(load_elf(&entry, elfp) != 0) {
		tprintf("Cannot decode ELF binary\n");
		return 0;
	}

	tprintf("Entry address = %06lX\n", entry);
	
	task_reg.name.pc = (INTPOINTER)entry;
	
	dispose_shell_line();	// 戻れないのでここで編集ラインは破棄
	exec_usertask();
	
	return 0;
}


static int com_execelf(int argc, char *argv[])
{
	unsigned char *elfp = (unsigned char *)DEF_LOAD_ADDR;
	unsigned long entry;

	if(argc > 1) {
		elfp = (unsigned char *)hstol(argv[1]);
	}

	tprintf("ELF bin address = %06lX\n", (unsigned long)elfp);
	
	if(load_elf(&entry, elfp) != 0) {
		tprintf("Cannot decode ELF binary\n");
		return 0;
	}

	tprintf("Entry address = %06lX\n", entry);
	
	task_reg.name.pc = (INTPOINTER)entry;
	
	dispose_shell_line();	// 戻れないのでここで編集ラインは破棄
	exec_usertask();
	
	return 0;
}

#endif

/**
 * @brief	プログラム呼出
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_call(int argc, char *argv[])
{
	void (* func)(void);
	
	if(argc == 2) {
		func = (void *)hstol(argv[1]);
	} else {
		func = (void *)task_reg.name.pc;
	}

	func();
	
	return 0;
}

#define TCSR	(*(volatile unsigned short *)0xffff8c)

/**
 * @brief	リセット(ウォッチドックタイマよる)
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_reset(int argc, char *argv[])
{
	TCSR = 0xa578;	// ウォッチドッグタイマリセット

	return 0;
}
	
/**
 * @brief	バージョン表示
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_version(int argc, char *argv[])
{
	extern void print_version(void);

	print_version();

	return 0;
}
	
/*----------------------------------------------------------------------------
  ヘルプメッセージ表示
----------------------------------------------------------------------------*/
static int com_help(int argc, char *argv[]);

const struct shell_command mon_com[] = {
  /* コマンド	*/
  { "load",	"l", 0,		com_load,	0, "Load [x] [v] [a]"},
  { "reg",	"r", 0,		com_reg,	0, "Reg [[reg] val]"},
  { "go",	"g", 0,		com_go,		0, "Go [address]"},
  { "step",	"s", 0,		com_step,	CMDATTR_CTAL, "Step"},
  { "break",	"b", 0,		com_break,	0, 
    "Break [[address]|[- address]]"},
  { "dump",	"d", init_memdump,com_memdump,	CMDATTR_CTAL,
    "Dump [-b|w|l] [start [end]]"},
  { "mem",	"m", init_memedit,com_memedit,	CMDATTR_CTAL,
    "Mem [-b|w|l] address [value ...]"},
  { "da",	"a", init_disa,	com_disa,	CMDATTR_CTAL,
    "disAsemble [address [end]]"},
  { "fill",	"f", 0, 	com_fill,	0, 
    "Fill address end|@length value"},
  { "call",	"c", 0, 	com_call,	0,
    "Call [address]"},
#ifdef MEMCHK
  { "memck",	"", 0,		com_memchk,	0,
    "Memory check [Start [End]]"},
#endif
#ifdef USE_NET
  { "ip",	"i", init_ipaddr,com_ipaddr,	0, "ip [address]"},
  { "tftpb",	"t", 0,		com_tftpb,	0, "tftpb <address> <file>"},
  { "exelf",	"e", 0,		com_execelf,	0, "exelf [address]"},
#endif
  { "reset",	"", 0,		com_reset,	0, "RESET"},
  { "ver",	"v", 0,		com_version,	0, "Version"},
  { "help",	"h", 0,		com_help,	0, "Help"},
  { "", "", 0, 0, 0, 0}	// コマンドリスト終了
}; //!< ヘルプメッセージ

/**
 * @brief	ヘルプメッセージ表示
 * @param[in]	argc	コマンド引数の数
 * @param[in]	argv	コマンド引数文字列のポインタ
 * @return	コマンド実行結果
 */
static int com_help(int argc, char *argv[])
{
	const struct shell_command *cp = mon_com;

	while(cp->name[0] != 0) {
		
		if(cp->help_str != 0) {
			tprintf(" [%1s] %5s : %s\n", cp->sname, cp->name,
				cp->help_str);
		}
		
		cp ++;
	}
	
	return 0;
}
