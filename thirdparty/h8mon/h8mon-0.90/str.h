/** @file
 * @brief	文字列処理
 *
 * @date	2002.03.02
 * @author	Takashi SHUDO
 */

#ifndef	_H_str
#define	_H_str

char *ltohs(char *str, unsigned long val, long keta);
long hstol(char *str);
unsigned char hs2c(char *str);
unsigned char h2c(char *str);
char *ltods(char *str, long keta, long val);
char *ultods(char *str, long keta, unsigned long val);
char *ltodsz(char *str, long keta, long val);
char *ultodsz(char *str, long keta, unsigned long val);
long dstol(char *str);
long strcomp(const char *s1, const char *s2);
long strncomp(const char *s1, const char *s2, long n);
char *strcopy(char *dest, const char *src);
char *strncopy(char *dest, const char *src, long n);
long strleng(const char *str);
char *str2cap(char *str);
char *str2small(char *str);
long hdstol(char *str);
void *memoryset(void *dest, unsigned char data, long count);
void *memorycopy(void *dest, const void *src, long num);

#endif	/* _H_str	*/
