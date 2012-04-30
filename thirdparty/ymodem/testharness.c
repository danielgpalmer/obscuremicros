#include <stdio.h>
#include <argtable2.h>
#include "ymodem.h"

int _getchar(int timeout){
	return -1;
}
void _sleep(unsigned long seconds){

}
void _putchar(int c){

}

int main(int argc, char* argv[]){

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

	if((send->count) == 0 && (recv->count == 0)){
		printf("you need to specify either send or receive\n");
		return 1;
	}

	arg_free(argtable);


}

