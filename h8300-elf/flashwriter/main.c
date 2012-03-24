#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "io3069.h"
#include "vect3069.h"
#include "monitor.h"
#include "sys.h"
#include "ymodem.h"
#include "flash/atmel.h"
#include "flash/flashstubs.h"

#define ROMSIZE 32768

int  _getchar(int timeout){

}

void _sleep(unsigned long seconds){

}

int  serial_read(void){

}

void flash_write_byte(uint32_t address, uint8_t data){
}
uint8_t flash_read_byte(uint32_t address){
}

void ymodemsend(){
	print("Start your ymodem send now..\n");
}

void ymodemrecv(){
	print("Start your ymodem recv now..\n");
}

void _putchar(int c){

}

int main(void)
{

	sysEnableChipSelect(6);
	P2DDR = 0xFF;

	uint8_t romdata[ROMSIZE];
 	volatile uint8_t* eeprom = (uint8_t*) 0xC00000;

	print("flash write thingy\n");

	while(1){
	print("press r to read the ROM or press w to write the ROM or q to quit\n");
	char ch = 0;

	while(ch == 0){ // Spin here.
		mon_getch(&ch);
	}

	switch(ch){
		case 'r':
		// read rom
		print("Reading ROM into memory..");
		for(uint16_t i = 0; i < ROMSIZE; i++){
			romdata[i] = eeprom[i];
		}
		print("Done\n");
		ymodemsend();
		break;
		
		case 'w':
		// write rom there
		ymodemrecv();
		atmel_identify();
		print("Writing ROM..");
        	print("Done\n");
		break;

		case 'q':
			return 0;

		default:
			mon_putch(&ch);
			print(" - eh?\n");
			break;
	}

	while(ch != 0){ // Drain any chars out
		ch = 0;
		mon_getch(&ch); 
	}

	}
	return 0;
}


