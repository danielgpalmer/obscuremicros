#define _POSIX_SOURCE 1 /* POSIX compliant source */
#include <stdio.h>
#include <argtable2.h>
#include "ymodem.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

#define BAUDRATE B115200

static int fd;

int _getchar(int timeout) {
	char ch = 0;
	int res = read(fd, &ch, 1);
	return res == 1 ? (int) ch : -1;
}
void _sleep(unsigned long seconds) {
	sleep(seconds);
}
void _putchar(int c) {
	char ch = (char) c;
	write(fd, &ch, 1);
}

int main(int argc, char* argv[]) {

	uint8_t* buff = malloc(5000);

	struct arg_file *device = arg_file1("d", "device", "", "serial device");
	struct arg_file *file = arg_file1("f", "file", "", "file to send or receive too");
	struct arg_lit *send = arg_lit0("s", "send", "send file");
	struct arg_lit *recv = arg_lit0("r", "receive", "receive file");
	struct arg_lit *help = arg_lit0(NULL, "help", "");
	struct arg_end *end = arg_end(20);

	void* argtable[] = { device, file, send, recv, help, end };

	int errors = arg_parse(argc, argv, argtable);

	if (help->count > 0) {
		printf("Usage: %s", argv[0]);
		arg_print_syntax(stdout, argtable, "\n");
		arg_print_glossary(stdout, argtable, "  %-30s %s\n");
		return 1;
	}

	if (errors > 0) {
		/* Display the error details contained in the arg_end struct.*/
		arg_print_errors(stdout, end, argv[0]);
		printf("Try '%s --help' for more information.\n", argv[0]);
		arg_free(argtable);
		return 1;
	}

	if ((send->count) == 0 && (recv->count == 0)) {
		printf("you need to specify either send or receive\n");
		return 1;
	}

//	int c;
	struct termios oldtio, newtio;

	fd = open(*(device->filename), O_RDWR | O_NOCTTY);
	if (fd < 0) {
		return 1;
	}

	tcgetattr(fd, &oldtio); /* save current port settings */

	memset(&newtio, 0, sizeof(newtio));
	newtio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
	newtio.c_iflag = IGNPAR;
	newtio.c_oflag = 0;

	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = 0;

	newtio.c_cc[VTIME] = 1; /* inter-character timer unused */
	newtio.c_cc[VMIN] = 0; /* blocking read until 5 chars received */

	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &newtio);

	if (ymodem_receive(buff, 5000)) {
		printf("done!\n");
	}
	else {
		printf("Failed! Error %d\n", errno);
	}

	tcsetattr(fd, TCSANOW, &oldtio);

	arg_free(argtable);
}

