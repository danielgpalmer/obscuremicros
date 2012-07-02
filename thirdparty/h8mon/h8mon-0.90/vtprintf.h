/** @file
 * @brief	機能限定printf
 *
 * @date	2007.03.10
 *		2002.03.02
 * @author	Takashi SHUDO
*/

#ifndef _H_vtprintf
#define _H_vtprintf

#include <stdarg.h>

#define MAXFORMATSTR	10	//!< 1引数デコードの最大文字列(%n?:のnの最大数)

long vtprintf(void (* putc)(char), const char *fmt, va_list args);

#endif // _H_vtprintf
