/** @file
 * @brief	GDB Remote Serial Protocol
 *
 * @date	2002.03.23
 * @author	Takashi SHUDO
 */

#include "stub.h"
#include "str.h"
#include "task.h"
#include "monio.h"
#include "tprintf.h"
#include "section.h"
#include "stub.h"

enum {
	RSPS_WDOL,	//!< $待ち
	RSPS_RCV,	//!< コマンド受信中
	RSPS_WSUM0,	//!< SUM 1バイト目
	RSPS_WSUM1	//!< SUM 2バイト目
} rsp_stat;	//!< 現在のコマンド受信状態(RSPS_*)

#define MAXCOMBUF	80	//!< コマンドバッファのサイズ
#define SUMLEN		2	//!< チェックサムのサイズ

static char combuf[MAXCOMBUF];	//!< コマンドバッファ
static char sumbuf[SUMLEN+1];	//!< チェックサムバッファ
static char *comptr;		//!< バッファポインタ

/**
 * @brief	サム値を計算する
 * @param[in]	str	文字列ポインタ
 * @param[in]	len	strのバイト数
 * @return	計算したサム値
 */
static unsigned char calc_sum(char *str, int len)
{
	unsigned char sum = 0;
	
	while(len) {
		sum += *str;
		str ++;
		len --;
	}
	
	return sum;
}

/**
 * @brief	文字列を送信して文字列のサム値を返す
 * @param[in]	str	文字列ポインタ
 * @return	計算したサム値
 */
static unsigned char send_str(char *str)
{
	unsigned char sum = 0;
	
	while(*str) {
		stubputc(*str);
		sum += *str;
		str ++;
	}

	return sum;
}

/**
 * @brief	文字列を送信する
 * @param[in]	str	文字列ポインタ
 */
static void put_str(char *str)
{
	while(*str) {
		stubputc(*str);
		str ++;
	}
}

/**
 * @brief	stubレスポンスデータを送信する
 * @param[in]	str	送信するデータ
 */
static void send_responce(char *str)
{
	unsigned char sum = 0;
	char sumstr[3];

	put_str("+$");
	
	sum += send_str(str);
	
	stubputc('#');
	ltohs(sumstr, sum, 2);
	put_str(sumstr);
}

/**
 * @brief	ステップ実行シグナル送信
 */
void stub_send_signal(void)
{
	send_responce("S05");
}

/**
 * @brief	ブレークシグナル送信
 */
void stub_send_expected(void)
{
	unsigned char sum = 0;
	char str[10];
	int i;
	
	put_str("+$");
	
	sum += send_str("T05");

	for(i=0; i<10; i++) {
		// レジスタ番号
		ltohs(str, i, 1);
		str[1] = ':';
		str[2] = 0;
		sum += send_str(str);

		// レジスタ値
		ltohs(str, task_reg.data[i], 8);
		str[8] = ';';
		str[9] = 0;
		sum += send_str(str);
	}
	
	stubputc('#');
	ltohs(str, sum, 2);
	put_str(str);
}

/**
 * @brief	Read Registersコマンド<br>
 *  		全レジスタ値を送信
 */
static void com_read_registers(void)
{
	unsigned char sum = 0;
	char str[10];
	int i;
	
	put_str("+$");

	for(i=0; i<10; i++) {
		ltohs(str, task_reg.data[i], 8);
		sum += send_str(str);
	}
	
	stubputc('#');
	ltohs(str, sum, 2);
	put_str(str);
}

/**
 * @brief	queryコマンド実行
 * @param[in]	str	コマンド引数文字列
 */
static void com_question(char *str)
{
	if(strcomp(str, "Offsets") == 0) {
		send_responce("Text=0;Data=0;Bss=0");
	} else if(strcomp(str, "C") == 0) {
		send_responce("QC1");	// ???
	} else {
		send_responce("");	// 未サポート
	}
}

/**
 * @brief	Write registerコマンド
 * @param[in]	str	コマンド引数文字列
 */
static void com_write_registerNN(char *str)
{
	task_reg.data[str[0]-'0'] = hstol(&str[2]);

	send_responce("OK");
}

/**
 * @brief	Write registersコマンド
 * @param[in]	str	コマンド引数文字列
 */
static void com_write_registers(char *str)
{
	int i;
	
	for(i=0; i<10; i++) {
		task_reg.data[i] = hstol(&str[i*8]);
	}

	send_responce("OK");
}

/**
 * @brief	Read memoryコマンド
 * @param[in]	str	コマンド引数文字列
 */
static void com_read_memory(char *str)
{
	unsigned long add = 0;
	short len = 0;
	unsigned char sum = 0;
	int i;
	char buf[3];

	// アドレス
	while(*str != ',') {
		add = ((add << 4) + h2c(str));
		str ++;
	}
		
	str ++;
	
	// サイズ
	while(*str != 0) {
		len = ((len << 4) + h2c(str));
		str ++;
	}
	
	// データ
	put_str("+$");
	
	for(i=0; i<len; i++) {
		ltohs(buf,
		      (unsigned long)(*(unsigned char *)((INTPOINTER)add)),2);
		sum += send_str(buf);
		add ++;
	}

	stubputc('#');
	ltohs(buf, sum, 2);
	put_str(buf);
}

/**
 * @brief	Write memoryコマンド
 * @param[in]	str	コマンド引数文字列
 */
static void com_write_memory(char *str)
{
	unsigned char *add = 0;
	short len = 0;
	int i;

	// アドレス
	while(*str != ',') {
		add = (unsigned char *)((((INTPOINTER)add) << 4) + h2c(str));
		str ++;
	}

	str ++;
	
	// サイズ
	while(*str != ':') {
		len = ((len << 4) + h2c(str));
		str ++;
	}
	
	str ++;

	// データ
	for(i=0; i<len; i++) {

		if((VECT_START <= (INTPOINTER)add) &&
		   ((INTPOINTER)add < VECT_END)) {
			*(add + VVECT_START)=hs2c(str);
		} else {
			*add = hs2c(str);
		}
		
		str += 2;
		add ++;
	}

	send_responce("OK");
}

/**
 * @brief	Write memory(binary)コマンド
 * @param[in]	str	コマンド引数文字列
 */
static void com_write_memory_bin(char *str)
{
	unsigned char *add = 0;
	short len = 0;
	int i;

	// アドレス
	while(*str != ',') {
		DPUTC(*str);
		add = (unsigned char *)((((INTPOINTER)add) << 4) + h2c(str));
		str ++;
	}
	DPUTC(*str);

	str ++;
	
	// サイズ
	while(*str != ':') {
		DPUTC(*str);
		len = ((len << 4) + h2c(str));
		str ++;
	}
	DPUTC(*str);
	
	str ++;

	// データ
	for(i=0; i<len; i++) {
		unsigned char *vadd = 0;
		
		DPUTC(*str);
		if((VECT_START <= (INTPOINTER)add) &&
		   ((INTPOINTER)add < VECT_END)) {
			vadd = add + VVECT_START;
		} else {
			vadd = add;
		}
		
		switch(*str) {
		case '#':
		case '$':
		case 0x7d:
			str ++;
			*vadd = (*str + 0x20);
			break;
			
		default:
			*vadd = *str;
			break;
		}
	
		str ++;
		add ++;
	}

	send_responce("OK");
}

/**
 * @brief	stubタスク
 * @param[in]	ch	入力文字
 * @return	1:kill受信 !=1:それ以外
 */
short stub_task(char ch)
{
	switch(rsp_stat) {
	case RSPS_WDOL:	// $待ち
		if(ch == '$') {
			rsp_stat = RSPS_RCV;
			comptr = combuf;
		}
		break;
		
	case RSPS_RCV:	// コマンド受信中
		if(ch == '#') {
			rsp_stat = RSPS_WSUM0;
		} else {
			if(comptr < &combuf[MAXCOMBUF-SUMLEN-1]) {
				*comptr = ch;
				comptr ++;
				*comptr = 0;
			} else {
				// バッファフル
				put_str("-$#00");
				DPRINTF("Buffer full.\n");
				rsp_stat = RSPS_WDOL;
			}
		}
		break;

	case RSPS_WSUM0:	// サム1バイト目
		sumbuf[0] = ch;
		rsp_stat = RSPS_WSUM1;
		break;

	case RSPS_WSUM1:	// サム2バイト目
		{
			unsigned char csum, sum;
			
			// コマンド受信完了
			sumbuf[1] = ch;
			sumbuf[2] = 0;
			
			// サムチェック
			csum = calc_sum(combuf, comptr-combuf);
			sum = hstol(sumbuf);
			
			if(csum != sum) {
				// サムが合わない
				put_str("-$#00");
				DPRINTF("Bad check sum.\n");
			} else {
				switch(combuf[0]) {
					/*
					 * レジスタ関連コマンド
					 */
				case 'g': // "Read registers"
					com_read_registers();
					break;
					
				case 'G': // "Write registers"
					com_write_registers(&combuf[1]);
					break;
					
				case 'P': // "Write register nn"
					com_write_registerNN(&combuf[1]);
					break;

					/*
					 * メモリ関連コマンド
					 */
				case 'm': // "Read Memory"
					com_read_memory(&combuf[1]);
					break;
					
				case 'M': // "Write Memory"
					com_write_memory(&combuf[1]);
					break;

					/*
					 * ターゲット情報
					 */ 
				case 'q': // "query"
					com_question(&combuf[1]);
					break;
					
					/*
					 * プログラムコントロール
					 */ 
				case 'H': // "Set thread"
					// "Hc-1"とか来るがスレッドは１つしか無いので何でもOK.
					send_responce("OK");
					break;
					
				case 's': // "Step"
					send_responce("OK");
					rsp_stat = RSPS_WDOL;	// 次のコマンド待ちへ

					step_exec_usertask();
					break;

				case 'c': // "Continue"
					send_responce("OK");
					rsp_stat = RSPS_WDOL;	// 次のコマンド待ちへ

					exec_usertask();
					break;

				case '?': // "Last signal"
					stub_send_signal();
					break;

				case 'k': // "Kill"
					stubputc('+');
					return 1;
					break;
					
				case 'X': // "write mem (binary)"
					com_write_memory_bin(&combuf[1]);
					break;
					
				default:
					send_responce("");	// 未サポート
					break;
				}
			}
			
			rsp_stat = RSPS_WDOL;	// 次のコマンド待ちへ
		}
		break;
	}

	return 0;
}

/**
 * @brief	stubの初期化
 */
void stub_init(void)
{
	rsp_stat = RSPS_WDOL;
}
