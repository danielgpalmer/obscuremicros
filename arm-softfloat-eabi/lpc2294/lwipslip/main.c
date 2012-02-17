#include "lpc22xx.h"
#include "uart.h"
#include "config.h"
#include <stdbool.h>
#include <stdio.h>

#include "exceptions.h"
#include "interrupts.h"
#include "net.h"
#include "rtc.h"

#include <lwip/inet.h>
#include "core.h"

#include "ff.h"

void starttimer() {

	printf("Starting timer0..\n");
	dumpInterruptRegisters();

	T0TCR = 0x0003; // enable and reset the counter.. hold it.
	T0IR = 0x0001;
	T0PR = 0xFFFF; // timer0 prescaler
	T0MR0 = 0x0001; // match
	T0MCR = 0x0003;
	//T0MR1 = 0x01FE;
	//T0MCR = 0x0019;  // match; fire interrupt, reset counter.
	VICIntEnable |= 0x0010; // enable timer0 interrupt

	VICVectCntl0 = 0x0024;
	VICVectAddr0 = (uint32_t) IRQ_Routine;

	//VICIntSelect = 0x0010; // make timer0 interrupts FIQ
	T0TCR = 0x0001; // go!

	enableIRQ();
	//enableFIQ();

	dumpInterruptRegisters();
}

static void initfs() {
	FRESULT f_err_code;
	static FATFS FATFS_Obj;
	char buffer[256];
	f_err_code = f_mount(0, &FATFS_Obj);
	printf("Mount %d\n", f_err_code);
	f_err_code = f_chdir("/");
	printf("chdir %d\n", f_err_code);
	f_err_code = f_getcwd(buffer, sizeof(buffer));
	printf("%d cwd %s\n", f_err_code, buffer);
	FIL file;
	f_err_code = f_open(&file, "/text.txt", FA_READ | FA_OPEN_EXISTING);
	printf("f_open %d\n", f_err_code);

	f_gets(buffer, 256, &file);
	printf("read %s\n", buffer);

	printf("listing \n");
	DIR dir;
	FILINFO fileinfo;
	f_err_code = f_opendir(&dir, "/");
	printf("%d\n", f_err_code);
	while (1) {
		f_err_code = f_readdir(&dir, &fileinfo);
		if (fileinfo.fname[0] == NULL) {
			break;
		}
		printf("%d - %s\n", f_err_code, fileinfo.fname);
	}

}

void main() {

	VICIntSelect = 0;
	VICIntEnClr = 0xFFFF;

	// Initialize the system
	initialize();
	PINSEL1 = 0;
	IODIR0 |= 0x60000000; // P0.29 & P0.30 output
	IOSET0 = 0x00000000;

	//bring UART0 and UART1 up
	uart0Init(UART_BAUD(115200), UART_8N1, UART_FIFO_8);
	uart1Init(UART_BAUD(115200), UART_8N1, UART_FIFO_14);

	rtc_start();
	initfs();
	net_init();
	sio_setuprxint();

	while (1) {

		for (int i = 0; i < 500; i++) {

		}

		net_loop();
	}

}

void FIQ_Routine(void) {
	IOCLR0 = 0x40000000;
	sio_rxinthandler();
	IOSET0 = 0x40000000;
}
