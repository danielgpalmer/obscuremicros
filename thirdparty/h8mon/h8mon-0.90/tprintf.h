/** @file
 * @brief	機能限定printf
 *
 * @date	2007.03.10
 *		2002.03.02
 * @author	Takashi SHUDO
*/

#ifndef _H_tprintf
#define _H_tprintf

long tprintf(const char *fmt, ...);

#ifdef DEBUG
long dprintf(const char *fmt, ...);
#define DPRINTF(x)	dprintf(x)
#else
#define DPRINTF(x)
#endif

#endif // _H_tprintf
