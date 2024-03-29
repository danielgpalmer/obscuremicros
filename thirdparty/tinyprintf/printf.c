/*
 Copyright 2001, 2002 Georges Menie (www.menie.org)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 putchar is the only external dependency for this file,
 if you have a working putchar, just remove the following
 define. If the function should be called something else,
 replace outbyte(c) by your own function call.
 */

#include "monitor.h"

#include <stddef.h>
#include <stdint.h>
#include <math.h>

#define putchar(c) mon_putch(c)

static void printchar(char **str, int c) {
	static char cr = '\r';
	if (str) {
		**str = c;
		++(*str);
	}
	else {
		char ch = c;

		if (ch == '\n') {
			putchar(&cr);
		}

		putchar(&ch);
	}
}

#define PAD_RIGHT 1
#define PAD_ZERO 2

static int prints(char **out, const char *string, int width, int pad) {
	int pc = 0, padchar = ' ';

	if (width > 0) {
		int len = 0;
		const char *ptr;
		for (ptr = string; *ptr; ++ptr)
			++len;
		if (len >= width)
			width = 0;
		else
			width -= len;
		if (pad & PAD_ZERO)
			padchar = '0';
	}
	if (!(pad & PAD_RIGHT)) {
		for (; width > 0; --width) {
			printchar(out, padchar);
			++pc;
		}
	}
	for (; *string; ++string) {
		printchar(out, *string);
		++pc;
	}
	for (; width > 0; --width) {
		printchar(out, padchar);
		++pc;
	}

	return pc;
}

/* the following should be enough for 32 bit int */
#define PRINT_BUF_LEN 12

static int printi(char **out, int32_t i, int b, int sg, int width, int pad, int letbase) {
	char print_buf[PRINT_BUF_LEN];
	char *s;
	int32_t t, neg = 0, pc = 0;
	uint32_t u = i;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return prints(out, print_buf, width, pad);
	}

	if (sg && b == 10 && i < 0) {
		neg = 1;
		u = -i;
	}

	s = print_buf + PRINT_BUF_LEN - 1;
	*s = '\0';

	while (u) {
		t = u % b;
		if (t >= 10)
			t += letbase - '0' - 10;
		*--s = t + '0';
		u /= b;
	}

	if (neg) {
		if (width && (pad & PAD_ZERO)) {
			printchar(out, '-');
			++pc;
			--width;
		}
		else {
			*--s = '-';
		}
	}

	return pc + prints(out, s, width, pad);
}

#define GETINTFROMSTACK(a) (*((uint32_t*) a) & 0xFFFF)
#define NEXTINT(a) (a += sizeof(uint32_t))

static int print(char **out, const char* format, void* varg) {
	int width, pad;
	int pc = 0;
	char scr[2];

	for (; *format != 0; ++format) {
		if (*format == '%') {
			format++;
			width = pad = 0;
			if (*format == '\0')
				break;
			if (*format == '%')
				goto out;
			if (*format == '-') {
				++format;
				pad = PAD_RIGHT;
			}
			while (*format == '0') {
				format++;
				pad |= PAD_ZERO;
			}
			for (; *format >= '0' && *format <= '9'; ++format) {
				width *= 10;
				width += *format - '0';
			}
			if (*format == 's') {
				char *s = *((char **) varg++);
				pc += prints(out, s ? s : "(null)", width, pad);
				continue;
			}
			if (*format == 'd') {
				pc += printi(out, GETINTFROMSTACK(varg), 10, 1, width, pad, 'a');
				NEXTINT(varg);
				continue;
			}
			if (*format == 'x') {
				pc += printi(out, GETINTFROMSTACK(varg), 16, 0, width, pad, 'a');
				NEXTINT(varg);
				continue;
			}
			if (*format == 'X') {
				pc += printi(out, GETINTFROMSTACK(varg), 16, 0, width, pad, 'A');
				NEXTINT(varg);
				continue;
			}
			if (*format == 'u') {
				pc += printi(out, GETINTFROMSTACK(varg), 10, 0, width, pad, 'a');
				NEXTINT(varg);
				continue;
			}
			if (*format == 'c') {
				/* char are converted to int then pushed on the stack */
				scr[0] = (char) GETINTFROMSTACK(varg);
				NEXTINT(varg);
				scr[1] = '\0';
				pc += prints(out, scr, width, pad);
				continue;
			}
		}
		else {
			out: printchar(out, *format);
			++pc;
		}
	}

	if (out)
		**out = '\0';
	return pc;
}

int tiny_printf(const char *format, ...) {
	void *varg = (void *) &format;
	varg += sizeof(void*);
	return print(NULL, format, varg);
}

int tiny_sprintf(char *out, const char *format, ...) {
	void *varg = (void *) (&format);
	varg += sizeof(void*);
	return print(&out, format, varg);
}
