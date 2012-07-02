/** @file
 * @brief	機能限定printf
 *
 * @date	2007.03.10
 *		2002.03.02
 * @author	Takashi SHUDO
*/

#include "vtprintf.h"
#include "str.h"

/**
 * @brief	文字列出力
 * @param[in]	putc	1文字出力関数
 * @param[in]	str	出力する文字列
 */
static void putstr(void (* putc)(char), char *str)
{
	while(*str) {
		putc(*str);
		str ++;
	}
}

/**
 * @brief	簡易printf、float,doubleは使えない
 *
 * %ns,%nd,%nx,%nXのみ<br>
 * %[n]s	- 文字列表示<br>
 *	  [n] 省略時:文字列長<br>
 * %[n]d	- 10進数表示<br>
 *	  [n] 省略時:デコード文字列長<br>
 *	  %の次が"0"ならば"0"付き表示<br>
 * %[n]X - 16進数表示(大文字)<br>
 * %[n]x	- 16進数表示(小文字)(必ず"0"付きで桁数分表示)<br>
 *	  [n] 0-8 省略時:桁数分<br>
 * %c	- 文字表示<br>
 *
 * @param[in]	putc	1文字出力関数
 * @param[in]	fmt	フォーマット文字列
 * @param[in]	args	引数リスト
 */
long vtprintf(void (* putc)(char), const char *fmt, va_list args)
{
	char str[MAXFORMATSTR];	// フォーマットデコードバッファ
	char dec = 0;		// 1:%デコード中,
	long keta = 0;		// 桁数
	char zero = 0;		// "0"付きか 0:不明,1:"0"付き,-1:"0"無し
	char flong = 0;		// long ?
	long len = 0;

	while(*fmt != 0) {
	    len ++;
	    
	    if(dec == 0) {
		    if(*fmt == '%') {
			    keta = 0;
			    zero = 0;
			    dec = 1;
		    } else {
			    putc(*fmt);
		    }
	    } else {
		if(('0' <= *fmt) && (*fmt <= '9')) {
			if(zero == 0) {
				if(*fmt == '0') {
					zero = 1;
				} else {
					zero = -1;
					keta = (keta*10) + *fmt - '0';
				}
			} else {
				keta = (keta*10) + *fmt - '0';
			}
		} else {
			switch(*fmt) {
			case '%':
				putc('%');
				dec = 0;
				break;
				
			case 'l':
				flong = 1;
				break;
				
			case 's':
				if(keta) {
					long i;
					char *p = va_arg(args, char *);
					for(i=0; i<keta; i++) {
						if(*p) {
							putc(*p);
							p++;
						} else {
							putc(' ');
						}
					}
				} else {
					putstr(putc,
					       va_arg(args, char *));
				}
				dec = 0;
				break;
					
			case 'i':
			case 'd':
				if(keta == 0) {
					char *p;
					if(flong) {
						ltods(str, MAXFORMATSTR-1,
						      va_arg(args,
							     long));
					} else {
						ltods(str,
						      MAXFORMATSTR-1,
						      va_arg(args,
							     int));
					}
					for(p=str;
					    p<&str[MAXFORMATSTR];
					    p++) {
						if(*p != ' ') {
							putstr(putc, p);
							break;
						}
					}
				} else {
					if(zero == 1) {
						if(flong) {
							ltodsz(str, keta,
							       va_arg(args,
								      long));
						} else {
							ltodsz(str, keta,
							       va_arg(args,
								      int));
						}
					} else {
						if(flong) {
							ltods(str, keta,
							      va_arg(args,
								     long));
						} else {
							ltods(str, keta,
							      va_arg(args,
								     int));
						}
					}
					putstr(putc, str);
				}
				dec = 0;
				break;
					
			case 'u':
				if(keta == 0) {
					char *p;
					if(flong) {
						ultods(str,
						      MAXFORMATSTR-1,
						      va_arg(args,
							     unsigned long));
					} else {
						ultods(str,
						      MAXFORMATSTR-1,
						      va_arg(args,
							     unsigned int));
					}
					for(p=str;
					    p<&str[MAXFORMATSTR];
					    p++) {
						    if(*p != ' ') {
							    putstr(putc, p);
							    dec = 0;
							    break;
						    }
					}
				} else {
					if(zero == 1) {
						if(flong) {
							ultodsz(str, keta,
							       va_arg(args,
								      unsigned long));
						} else {
							ultodsz(str, keta,
							       va_arg(args,
								      unsigned int));
						}
					} else {
						if(flong) {
							ultods(str, keta,
							      va_arg(args,
								     unsigned long));
						} else {
							ultods(str, keta,
							      va_arg(args,
								     unsigned int));
						}
					}
					putstr(putc, str);
				}
				dec = 0;
				break;
				
			case 'x':
			case 'X':
				if(flong) {
					if(keta == 0) {
						keta = sizeof(long)*2;
					}
					ltohs(str, va_arg(args, long), keta);
				} else {
					if(keta == 0) {
						keta = sizeof(int)*2;
					}
					ltohs(str, va_arg(args, int), keta);
				}
				if(*fmt == 'X') {
					str2cap(str);
				}
				putstr(putc, str);
				dec = 0;
				break;
					
			case 'c':
				putc(va_arg(args, int));
				dec = 0;
				break;  
			}
		}
	    }
		
	    fmt ++;
	}

	return len;
}
