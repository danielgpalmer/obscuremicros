/** @file
 * @brief	機能限定printf
 *
 * @date	2007.03.10
 *		2002.03.02
 * @author	Takashi SHUDO
*/

#include "str.h"
#include "vtprintf.h"
#include "conio.h"

/**
 * @brief	機能限定printf
 * @param[in]	fmt	フォーマット文字列
 */
long tprintf(const char *fmt, ...)
{
	va_list	args;
	int len = 0;
	
	va_start(args, fmt);	
	len += vtprintf(cputc, fmt, args);
	va_end(args);

	return len;
}

#ifdef DEBUG
long dprintf(const char *fmt, ...)
{
	va_list	args;
	int len = 0;
	
	va_start(args, fmt);	
	len += vtprintf(cputc, fmt, args);
	va_end(args);

	return len;
}
#endif
