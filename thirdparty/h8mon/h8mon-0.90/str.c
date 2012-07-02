/** @file
 * @brief	文字列処理
 *
 * @date	2002.03.02
 * @author	Takashi SHUDO
 */

#include "str.h"

/*
 *	16進数文字列<->16進数変換
 */

/**
 * @brief	long dataをketa桁の16進数字列へ変換してstrへ出力
 * @param[out]	str	出力文字列ポインタ
 * @param[in]	val	文字列にする値
 * @param[in]	keta	出力文字数(0-8)
 * @return	出力文字列ポインタ
 */
char *ltohs(char *str, unsigned long val, long keta)
{
	long i;
	unsigned long	mask = (0xf0000000>>((8-keta)*4));
	long tmp;

	for(i=0; i<keta; i++) {
		tmp = (val & mask) >> ((keta-1-i)<<2);
		if(tmp < 10) {
			*str = tmp + '0';
		} else {
			*str = tmp - 10 + 'a';
		}
		str ++;
		mask >>= 4;
	}
	*str = 0;

	return str;
}

/**
 * @brief	strの16進数文字列8文字からlongへ変換
 * @param[in]	str		- 16進数文字列
 * @return	変換した値
 */
long hstol(char *str)
{
	long rt = 0;

	while(*str != 0) {
		rt <<= 4;
		if((('0' <= *str) && (*str <= '9'))
		   || (('A' <= *str) && (*str <= 'F'))
		   || (('a' <= *str) && (*str <= 'f'))) {
			if(('0' <= *str) && (*str <= '9')) {
				rt += *str - '0';
			} else
				if(('A' <= *str) && (*str <= 'F')) {
					rt += *str - 'A' + 10;
				} else
					if(('a' <= *str) && (*str <= 'f')) {
						rt += *str - 'a' + 10;
					}
		}
		str ++;
		if(*str == ';') {	// !!!
			break;
		}
	}

	return rt;
}

/**
 * @brief	strの16進数文字列2文字からunsigned charへ変換
 * @param[in]	str		- 16進数文字列
 * @return	変換した値
 */
unsigned char hs2c(char *str)
{
	unsigned char i, rt = 0;
	
	for(i=0; i<2; i++) {
		rt <<= 4;
		if((('0' <= *str) && (*str <= '9'))
		   || (('A' <= *str) && (*str <= 'F'))
		   || (('a' <= *str) && (*str <= 'f'))) {

			if(('0' <= *str) && (*str <= '9')) {
				rt += *str - '0';
			} else if(('A' <= *str) && (*str <= 'F')) {
				rt += *str - 'A' + 10;
			} else if(('a' <= *str) && (*str <= 'f')) {
				rt += *str - 'a' + 10;
			}
		}
		str ++;
	}

	return rt;
}

/**
 * @brief	strの16進数文字列1文字からunsigned charへ変換
 * @param[in]	str	16進数文字列
 * @return	変換した値
 */
unsigned char h2c(char *str)
{
	unsigned char rt = 0;
	
	if(('0' <= *str) && (*str <= '9')) {
		rt = *str - '0';
	} else if(('A' <= *str) && (*str <= 'F')) {
		rt = *str - 'A' + 10;
	} else if(('a' <= *str) && (*str <= 'f')) {
		rt = *str - 'a' + 10;
	}

	return rt;
}

/**
 * @brief	long valをkt桁の10進数文字列に変換してstrへ出力
 * @param[out]	str	10進数文字列
 * @param[in]	keta	桁数
 * @param[in]	val	変換する値
 * @return	変換した文字列ポインタ
 */
char *ltods(char *str, long keta, long val)
{
	long i, j;
	int m = 0;

	if(val < 0) {
		val = 0-val;
		m = 1;
	}

	for(i=0; i<keta; i++) {
		if(val == 0) {
			if(i == 0) {
				str[keta-1-i] = '0';
				i++;
			}
			for(j=0; j<(keta-i); j++) {
				str[j] = ' ';
			}
			if(m) {
				if((keta-i-1) >= 0) {
					str[keta-i-1] = '-';
				}
			}
			goto end;
		}
		str[keta-1-i] = (val % 10) + '0';
		val /= 10;
	}

 end:
	str[keta] = 0;

	return str;
}

/**
 * @brief	unsigned long valをkt桁の10進数文字列に変換してstrへ出力(符号無し)
 * @param[out]	str	10進数文字列
 * @param[in]	keta	桁数
 * @param[in]	val	変換する値
 * @return	変換した文字列ポインタ
 */
char *ultods(char *str, long keta, unsigned long val)
{
	long i, j;

	for(i=0; i<keta; i++) {
		if(val == 0) {
			if(i == 0) {
				str[keta-1-i] = '0';
				i++;
			}
			for(j=0; j<(keta-i); j++) {
				str[j] = ' ';
			}
			goto end;
		}
		str[keta-1-i] = (val % 10) + '0';
		val /= 10;
	}

 end:
	str[keta] = 0;

	return str;
}

/**
 * @brief	long valをketa桁の10進数文字列に変換してstrへ出力("0"付き)
 * @param[out]	str	10進数文字列
 * @param[in]	keta	桁数
 * @param[in]	val	変換する値
 * @return	変換した文字列ポインタ
 */
char *ltodsz(char *str, long keta, long val)
{
	long i, j;

	if(val < 0) {
		val = 0-val;
		str[0] = '-';
	} else {
		str[0] = '0';
	}

	for(i=0; i<keta; i++) {
		if(val == 0) {
			for(j=1; j<(keta-i); j++) {
				str[j] = '0';
			}
			goto end;
		}
		str[keta-1-i] = (val % 10) + '0';
		val /= 10;
	}

 end:
	str[keta] = 0;

	return str;
}

/**
 * @brief	unsigned long valをketa桁の10進数文字列に変換してstrへ出力("0"付き,符号無し)
 * @param[out]	str	10進数文字列
 * @param[in]	keta	桁数
 * @param[in]	val	変換する値
 * @return	変換した文字列ポインタ
 */
char *ultodsz(char *str, long keta, unsigned long val)
{
	long i, j;

	for(i=0; i<keta; i++) {
		if(val == 0) {
			for(j=0; j<(keta-i); j++) {
				str[j] = '0';
			}
			goto end;
		}
		str[keta-1-i] = (val % 10) + '0';
		val /= 10;
	}

 end:
	str[keta] = 0;

	return str;
}

/**
 * @brief	strの10進数文字列からlongへ変換
 * @param[in]	str		- 10進数文字列
 * @return	変換した値
 */
long dstol(char *str)
{
	long rt = 0;
	char m = 0;

	while(*str != 0) {
		if(*str == '-') {
			m = 1;
		}
		rt *= 10;
		if(('0' <= *str) && (*str <= '9')) {
			rt += *str - '0';
		}
		str ++;
		if(*str == ';') {
			break;
		}
	}

	if(m) {
		rt = 0-rt;
	}

	return rt;
}

/**
 * @brief	文字列s1とs2を比較して結果を返す
 * @return	0:同じ -1:*s1<*s2 1:*s1>*s2
 */
long strcomp(const char *s1, const char *s2)
{
	while((*s1 != 0) || (*s2 != 0)) {
		if(*s1 > *s2) {
			return 1;
		} else
			if(*s1 < *s2) {
				return -1;
			}
		s1 ++;
		s2 ++;
	}

	return 0;
}

/**
 * @brief	文字列s1とs2をn文字分比較して結果を返す
 * @return	0:同じ -1:*s1<*s2 1:*s1>*s2
 */
long strncomp(const char *s1, const char *s2, long n)
{
	while(n) {
		if(*s1 > *s2) {
			return 1;
		} else
			if(*s1 < *s2) {
				return -1;
			}
		s1 ++;
		s2 ++;
		n --;
	}

	return 0;
}

/**
 * @brief	文字列srcをdestへコピー
 * @param[out]	dest	コピー先文字列ポインタ
 * @param[in]	src	コピー元文字列ポインタ
 * @return	コピー先文字列ポインタ
 */
char *strcopy(char *dest, const char *src)
{
	while(*src != 0) {
		*dest = *src;
		dest ++;
		src ++;
	}
	*dest = 0;

	return dest;
}

/**
 * @brief	文字列srcをdestへn文字分コピー
 * @param[out]	dest	コピー先文字列ポインタ
 * @param[in]	src	コピー元文字列ポインタ
 * @param[in]	n	コピーする文字のバイト数 
 * @return	コピー先文字列ポインタ
 */
char *strncopy(char *dest, const char *src, long n)
{
	while(n) {
		if(*src == 0) {
			break;
		}
		*dest = *src;
		dest ++;
		src ++;
		n --;
	}
	*dest = 0;

	return dest;
}

/**
 * @brief	文字列strの長さを返す
 * @param[in]	str	文字列のポインタ
 * @return	文字列のバイト数
 */
long strleng(const char *str)
{
	long len = 0;

	while(*str) {
		len ++;
		str ++;
	}

	return len;
}

/**
 * @brief	文字列strの小文字を大文字にする
 * @param[in]	str	文字列のポインタ
 * @return	変換した文字列のポインタ
 */
char *str2cap(char *str)
{
	char *p = str;
	while(*str) {
		if(('a' <= *str) && (*str <= 'z')) {
			*str -= ('a' - 'A');
		}
		str ++;
	}

	return p;
}

/**
 * @brief	文字列strの大文字を小文字にする
 * @param[in]	str	文字列のポインタ
 * @return	変換した文字列のポインタ
 */
char *str2small(char *str)
{
	char *p = str;
	while(*str) {
		if(('A' <= *str) && (*str <= 'Z')) {
			*str += ('a' - 'A');
		}
		str ++;
	}

	return p;
}

/**
 * @brief	文字列の最初が"#"の場合
 *		strの10進数文字列からlongへ変換
 *		それ以外
 *		strの16進数文字列8文字からlongへ変換
 * @param[in]	str	文字列のポインタ
 * @return	変換した値
 */
long hdstol(char *str)
{
	if(*str == '#') {
		return dstol(&str[1]);
	} else {
		return hstol(str);
	}
}

/**
 * @brief	メモリを任意の値に設定
 * @param[out]	dest	値を設定されるメモリのポインタ
 * @param[in]	data	設定する値
 * @param[in]	count	設定するバイト数
 * @return	値が設定されたメモリのポインタ
 */
void *memoryset(void *dest, unsigned char data, long count)
{
	unsigned char *p = dest;
	
	while(count) {
		*p = data;
		p ++;
		count --;
	}
	
	return dest;
}

/**
 * @brief	メモリをコピー
 * @param[out]	dest	コピー先メモリのポインタ
 * @param[in]	src	コピー元メモリのポインタ
 * @param[in]	num	設定するバイト数
 * @return	コピー先メモリのポインタ
 */
void *memorycopy(void *dest, const void *src, long num)
{
	unsigned char *d = dest;
	unsigned char *s = (unsigned char *)src;
	
	while(num) {
		*d = *s;
		d ++;
		s ++;
		num --;
	}
	
	return dest;
}
