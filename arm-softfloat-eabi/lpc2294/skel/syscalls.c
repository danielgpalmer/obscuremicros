#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>

#include <string.h>
#include <sys/time.h>

#include "uart.h"

#define STDIN_FILENO 0 /* standard input file descriptor */
#define STDOUT_FILENO 1 /* standard output file descriptor */
#define STDERR_FILENO 2 /* standard error file descriptor */

int _gettimeofday(struct timeval *tv, struct timezone *tz) {
	return 0;
}

int _write(int file, char * ptr, int len) {
	if (file == STDOUT_FILENO) {
		//int pos = 0;
		for (int pos = 0; pos < len; pos++) {
			uart_putc(*ptr++);
		}
		return 0;
	}

	return 0;
}

int _read(int file, char * ptr, int len) {
	//uart_putc('r');

	if (file == STDIN_FILENO) {
		*ptr = uart_getc();
		uart_putc(*ptr);
		return 1;
	}

	return 0;

}

int _close(int file) {
	return 0;
}

int _lseek(int file, int ptr, int dir) {
	return 0;
}

int _isatty(int file) {
	if (file == STDOUT_FILENO) {
		return 1;
	}

	return 0;
}

int _fstat(int file, struct stat * st) {
	if (file == STDOUT_FILENO) {
		memset(st, 0, sizeof(*st));
		st->st_mode = S_IFCHR;
		st->st_blksize = 1024;
		return 0;
	}
	else {
		return (-1);
	}
}

/* Register name faking - works in collusion with the linker.  */
register char * stack_ptr asm ("sp");

caddr_t _sbrk(int incr) {
	extern char end asm ("end"); /* Defined by the linker.  */
	static char * heap_end;
	char * prev_heap_end;

	if (heap_end == NULL) {
		heap_end = &end;
	}

	prev_heap_end = heap_end;

	if (heap_end + incr > stack_ptr) {
		/* Some of the libstdc++-v3 tests rely upon detecting
		 out of memory errors, so do not abort here.  */
#if 0
		extern void abort (void);
		_write (1, "_sbrk: Heap and stack collision\n", 32);
		abort ();
#else
		errno = ENOMEM;
		return (caddr_t) -1;
#endif
	}

	heap_end += incr;

	return (caddr_t) prev_heap_end;
}
