/** @file
 * @brief motorola S-records デコーダ
 *
 * @date	2002.03.01
 * @author	Takashi SHUDO
 */

#include "tprintf.h"
#include "str.h"
#include "sformat.h"
#include "task.h"

/**
 * @brief	strのSフォーマット文字列をデコードする
 * @param[in]	str	デコードする文字列
 * @param[in]	add	デコードしたデータのアドレスポインタのポインタ
 * @param[out]	len	デコードしたデータのバイト数のポインタ
 * @param[out]	bbuf	デコードしたデータを出力するポインタ
 * @return	1-9: Sヘッダ<br>
 *	 	 -1: フォーマットエラー
 */
int DecodeSFormat(char *str, unsigned char **add, unsigned long *len,
		   unsigned char *bbuf)
{
	long i, fl, al;
	unsigned char *dp = bbuf;
	unsigned char tmp, sum = 0;
	unsigned char *tadd;

	if(str[0] != 'S') {
//		mprintf("Not S-records:%s\n", str);
		return -1;
	}
	
	tadd = 0;

	fl = hs2c(&str[2]);	// sumまでのデータ数
//	mprintf("(%d)\n", fl);
	sum += fl;

	switch(str[1]) {
	case '0':
	case '1':
		al = 2;
		break;
		
	case '2':
		al = 3;
		break;
		
	case '3':
		al = 4;
		break;

	case '4':
	case '5':
	case '6':
		*len = 0;
		return str[1]-'0';	// 未サポート
		break;
		
	case '7':
	case '8':
	case '9':
		al = 0;
		break;
		
	default:
		return -1;
		break;
	}
	
	for(i=0; i<al; i++) {
		tmp = hs2c(&str[4+(i*2)]);
		tadd = (unsigned char *)((((INTPOINTER)tadd) << 8) + tmp);
		sum += tmp;
	}
	
	*add = tadd;
//	mprintf("%08x\n", tadd);

	for(i=1+al; i<fl; i++) {
		tmp = hs2c(&str[2+i*2]);
		*dp = tmp;
		sum += tmp;
		dp ++;
	}

	tmp = hs2c(&str[2+i*2]);

	if((0xff - sum) != tmp) {
//		mprintf("Sum Error %02x,%02x\n", ~sum, tmp);
		return -1;
	}
	
	*len = (fl-al-1);

//	printf("S:%d A:%08x L:%08x\n", str[1], *add, *len);
	
	return str[1]-'0';
}
