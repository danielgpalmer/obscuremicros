#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "io3069.h"
#include "vect3069.h"
#include "monitor.h"
#include "sys.h"
#include "ymodem.h"
#include "flash/jedec.h"
#include "flash/atmel.h"
#include "flash/flashstubs.h"
#include "itoa.h"

#define ROMSIZE 32768

volatile uint8_t* eeprom = (volatile uint8_t*) 0xC00000;
static uint8_t romdata[ROMSIZE];

void _putchar(int c) {
	char ch = (char) c;
	mon_putch(&ch);
}

int _getchar(int timeout) {
	//mon_print("_getchar\n");
	return -1;
}

void _sleep(unsigned long seconds) {

}

int serial_read(void) {
	return -1;
}

void flash_write_byte(uint32_t address, uint8_t data) {
	*(eeprom + address) = data;
}
uint8_t flash_read_byte(uint32_t address) {
	return *(eeprom + address);
}

void flash_sleep(uint8_t millis) {
	for (uint16_t j = 0; j < millis * 100; j++) {
		char ch = '.';
		mon_putch(&ch);
	}
}

void ymodemsend() {
	mon_print("Start your ymodem send now..\n");
}

void ymodemrecv() {
	mon_print("Start your ymodem recv now..\n");
	ymodem_receive(romdata, ROMSIZE);
}

int main(void) {

	sysEnableChipSelect(6);
	P2DDR = 0xFF;
	sysDRAMRASUp();

	static uint8_t dummy[64];
	for (int i = 0; i < sizeof(dummy); i++) {
		dummy[i] = 0xFF;
	}

	mon_print("flash write thingy\n");

	while (1) {
		mon_print("press i to identify, press r to read the ROM or press w to write the ROM or q to quit\n");
		char ch = 0;

		while (ch == 0) { // Spin here.
			mon_getch(&ch);
		}

		switch (ch) {

			case 'i': {
				jedecid_t* id = atmel_identify();
				mon_print("mfr ");
				mon_print(itoa(id->mfrid));
				mon_print(" dev ");
				mon_print(itoa(id->deviceid));
				mon_print("\n");
			}
				break;
			case 'r':
				// read rom
				mon_print("Reading ROM into memory..");
				for (uint16_t i = 0; i < ROMSIZE; i++) {
					romdata[i] = flash_read_byte(i);
				}
				mon_print("Done\n");
				ymodemsend();
				break;

				//case 'R':
				//for(uint16_t i = 0; i < ROMSIZE/8; i++){
				//	for(uint8_t j = 0; j < 8; j++){
				//		mon_print(itoa(romdata[(i * 8) + j]));
				//		mon_print(" ");
				//		}
				//		mon_print("\n");
				//	}
				//break;
			case 'w':
				// write rom there
				ymodemrecv();
				atmel_identify();
				mon_print("Writing ROM..");
				mon_print("Done\n");
				break;

			case 'e':
				mon_print("Erasing chip..");
				atmel_chiperase();
				mon_print("Done\n");
				break;

			case 'f':
				for (uint16_t page = 0; page < ROMSIZE; page += 64) {
					atmel_writepage(false, false, page, dummy);
				}
				break;

			case 'q':
				return 0;

			default:
				mon_putch(&ch);
				mon_print(" - eh?\n");
				break;
		}

		while (ch != 0) { // Drain any chars out
			ch = 0;
			mon_getch(&ch);
		}

	}
	return 0;
}

