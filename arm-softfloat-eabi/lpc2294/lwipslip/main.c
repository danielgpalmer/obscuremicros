#include "lpc22xx.h"
#include "uart.h"
#include "config.h"
#include "main.h"
#include <stdbool.h>
#include <stdio.h>

#include "exceptions.h"
#include "interrupts.h"
#include "net.h"

#include <lwip/inet.h>
#include "ping.h"


//#include <string.h>



#define ERAMBASE 0x81000000
#define ERAMSIZE   0x100000 // 1MB
#define ERAMTOP  0x81100000

void testeram(){


		uart0Puts("Writing to SRAM..\n");

		uint32_t data = 0;

		uint32_t *eram = (uint32_t*) ERAMBASE;


		while(eram != (uint32_t*) ERAMTOP){
			*eram++ = data;
			data++;
		}

		uart0Puts("Reading back SRAM..\n");

		eram = (uint32_t*) ERAMBASE;
		data = 0;

		bool failed = false;

		while(eram != (uint32_t*) ERAMTOP){
			if(*eram++ != data){
				uart0Puts("SRAM wasn't what I expected\n");
				failed = true;
			}
			data++;
		}

		if(!failed){
			uart0Puts("SRAM looks ok\n");
		}

}


void starttimer(){

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
	VICVectAddr0=(uint32_t) IRQ_Routine;


	//VICIntSelect = 0x0010; // make timer0 interrupts FIQ
	T0TCR = 0x0001; // go!

	enableIRQ();
	//enableFIQ();

	dumpInterruptRegisters();
}

void buzzer(){

	PINSEL1 |= 0x400; // pwm5

	PWMPR = 0x000F; // prescaler

	PWMPCR = 0x2000; // pwm config

	PWMMCR = 0x2; // match on match0 and reset

	PWMMR0 = 0xFF; // load match register 0
	PWMMR5 = 0x7F; // 50% duty cycle
	PWMLER = 0x21; // latch mr0 and mr1
	PWMTCR = 0x2; // reset and hold
	PWMTCR = 0x9; // go!


}

void spi(){

	PINSEL0 |= 0x5500;
	S0SPCR = 0x20; // Master
	S0SPCCR = 0x80; // clock
}


/**********************************************************
                       MAIN
**********************************************************/

void maxwrite(uint8_t high, uint8_t low){

	uint8_t spiinh, spiinl;
	uint8_t spisr;

	for(int i = 0; i < 1024; i++){

			}

	IOCLR0 = 0x800000;
	S0SPDR = high;

	int exit = 0;
	while(exit == 0){
		if((S0SPSR & 0x80) == 0x80){
			spiinh = S0SPDR;
			exit = 1;
		}
	}

	S0SPDR = low;

	exit = 0;
	while(exit == 0){
		if((S0SPSR & 0x80) == 0x80){
			spiinl = S0SPDR;
			exit = 1;
		}
	}


	IOSET0 = 0x800000;

	printf("++0x%x - %x\n", spiinh, spiinl);

}


void main(){ 
int		j;// loop counter (stack variable)

	VICIntSelect = 0;
	VICIntEnClr = 0xFFFF;

	
	// Initialize the system
	Initialize();
	PINSEL1=0;
	IODIR0 |= 0x60000000;	// P0.29 & P0.30 output
	IOSET0 =  0x00000000;
 
	//bring UART0 up
	uart0Init(UART_BAUD(115200), UART_8N1, UART_FIFO_8);
	uart1Init(UART_BAUD(115200), UART_8N1, UART_FIFO_14);
	

	printf("hello\n");

	//testeram();


	//buzzer();

	//net_init();
	//starttimer();
	//sio_setuprxint();

	//ping_init();

	spi();


//	fpgaputword(0x4444);

	char commandbuffer[256];
	int commandpos = 0;


	IODIR0 |= 0x800000;
	while(1){
		maxwrite(0xC0, 0x0F);
		maxwrite(0x40, 0x00);
		maxwrite(0x80, 0xAA);
	}


	while (1) {
		uart0Puts("\n> ");

		while((commandbuffer[commandpos] = (char) uart0GetchW()) != 0x0D){

			if(commandbuffer[commandpos] >= 0x20 && commandbuffer[commandpos] <= 0x7E){
				uart0Putch(commandbuffer[commandpos]);
				commandpos++;
			}
		}

		commandbuffer[commandpos] = 0;

		int parsedcmd = parsecmd(&commandbuffer);
		if(parsedcmd == CMD_NOCMD){
			uart0Puts("\nSorry.. I have no idea what \"");
			uart0Puts(commandbuffer);
			uart0Puts("\" is..");
		}
		else if(parsedcmd == CMD_FPGA){
			cmdfpga(arg0, arg1);
		}
		else if(parsedcmd == CMD_PING){
			ping_send_now();
		}


		uart0Puts("\nCMD - ");
		uart0Puts(commandbuffer);

		if(arg0 != 0){
			uart0Puts("; arg 0: ");
			uart0Puts(arg0);
		}
		else {
			uart0Puts(";");
		}

		if(arg1 != 0){
			uart0Puts("; arg 1: ");
			uart0Puts(arg1);
		}

		uart0Puts(";");


		commandpos = 0;
	}
	
}



/**********************************************************
                      Initialize
**********************************************************/

#define PLOCK 0x400

void Initialize(void)  {

   
	// Setting Multiplier and Divider values
  	//PLLCFG=0x23;
  	PLLCFG=0x23;
	feed();
  
	// Enabling the PLL */	
	PLLCON=0x1;
	feed();
  
	// Wait for the PLL to lock to set frequency
	while(!(PLLSTAT & PLOCK)) ;
  
	// Connect the PLL as the clock source
	PLLCON=0x3;
	feed();
  


	//PINSEL2 = 0x0F000924;
	//BCFG0=0x1000348F;
	//BCFG1=0x2000040F;


	//MEMMAP = 2;

	// Enabling MAM and setting number of clocks used for Flash memory fetch (4 cclks in this case)
	MAMCR=0x2;
	MAMTIM=0x4;
  
	//clear external SRAM
	//uint32_t *esram = (uint32_t) 0x81000000;
	//while(esram < 0x81040000){
	//	*esram = 0;
	//	esram++;
	//}


	// Setting peripheral Clock (pclk) to System Clock (cclk)
	VPBDIV = VPBDIV_VAL;


/*



*/
}


void feed(void)
{
  PLLFEED = PLL_FEED1;
  PLLFEED = PLL_FEED2;
}

int parsecmd(char* cmd){

	uart0Putch('\n');

	int tokencounter = 0;
	char curchar;

	command = cmd;
	arg0 = 0;
	arg1 = 0;

	while((curchar = *cmd) != 0){
		if(curchar == TOKENSEPERATOR){
			uart0Putch('T');
			*cmd = 0;
			if(tokencounter == 0){
				arg0 = cmd + 1;
			}
			if(tokencounter == 1){
				arg1 = cmd + 1;
			}
			tokencounter++;
		}
		else {
			uart0Putch(curchar);
		}
		cmd++;
	}

	if(strcmp(command, "r") == 0){
		uart0Puts("Read!");
		return CMD_READ;
	}

	else if(strcmp(command, "fpga") == 0){
		return CMD_FPGA;
	}

	else if(strcmp(command, "fpga") == 0){
			return CMD_FPGA;
		}

	else if(strcmp(command, "ping") == 0){
				return CMD_PING;
	}

	else {
		uart0Puts("\nCouldn't decode command");
	}

	return CMD_NOCMD;
}


#define FPGAREADY 0x4
void cmdfpga(char* arg0, char* arg1){
	uint32_t port = IOPIN0;
	port &= FPGAREADY;

	// FPGAREADY is active low
	if(strcmp(arg0, "ready") == 0){
		if(port == FPGAREADY){
			uart0Puts("FPGA is not ready\n");
		}
		else {
			uart0Puts("FPGA is ready\n");
		}

		return;
	}

	else if (strcmp(arg0, "write") == 0){
		fpgaputword(0x1, 0x1, 0xABAB);
		return;
	}

	else if(strcmp(arg0, "read") == 0){
		fpgagetword(0x1, 0x1);
		return;
	}
}



#define DATAPINS 0xFFFF0
#define NOTDATAPINS 0xFFF0000F

#define REGPINS 0x3C00000
#define REGPAGEPINS 0x38000000

uint8_t regnumber = 0;
uint8_t regpage = 0;

#define STROBE 0x100000
#define READ 0x200000


uint16_t fpgagetword(uint8_t bank, uint8_t address){

	IODIR0 &= NOTDATAPINS;
	IOCLR0 = (REGPAGEPINS | REGPINS ); // Clear the bank and address


	uint32_t addr = ((address & 0x1F) << 22) | ((bank & 0x7) << 27) ;
	IOSET0 = (addr | READ | STROBE); // Pull up read, strobe and put the address on the bus

	int i = 0;
	int j = 0;

	for(i = 0; i < 2048; i++){
				for(j = 0; j < 1024; j++){

					}
			}

	IOCLR0 = 0x100000; // strobe the data.

	for(i = 0; i < 1024; i++){
				for(j = 0; j < 128; j++){

					}
			}



	uint32_t data = IOPIN0;

	uint16_t cutdata = data >> 4;

	uint8_t highbyte = (cutdata >> 8);

	uart0Putch(highbyte);

	return 0x0;

}

void fpgaputword(uint8_t bank, uint8_t address, uint16_t word){
	IODIR0 |= (0x3FFFF0 | REGPAGEPINS | REGPINS | STROBE );
	IOCLR0 = (READ | 0xFFFF0 | REGPAGEPINS | REGPINS ); // Clear the R/not W line .. this is a write, and clear any data off the bus


	uint32_t addr = ((address & 0x1F) << 22) | ((bank & 0x7) << 27) ;
	IOSET0 = (STROBE | addr); // bring the Strobe line up

	int i = 0;
	int j = 0;

	for(i = 0; i < 2048; i++){
		for(j = 0; j < 1024; j++){}
	}

	IOSET0 = 0x00000000 | (word << 4);

	for(i = 0; i < 1024; i++){
		for(j = 0; j < 128; j++){}
	}

	IOCLR0 = 0x100000; // strobe the data.


	for(i = 0; i < 1024; i++){
		for(j = 0; j < 128; j++){}
	}
}

