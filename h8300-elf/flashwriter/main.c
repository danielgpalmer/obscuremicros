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
#include "printf.h"
#include <errno.h>

#define ROMSIZE 32768

volatile uint8_t* eeprom = (volatile uint8_t*) 0xC00000;
static uint8_t romdata[ROMSIZE];

void _putchar(int c) {
	char ch = (char) c;
	mon_putch(&ch);
}

int _getchar(int timeout) {
	char ch;
	mon_getch(&ch);
	return ch;
	//return -1;
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

static void ymodemsend() {
	mon_print("Start your ymodem receive now..\n");
	ymodem_send(romdata, ROMSIZE, "rom.bin");
}

static bool ymodemrecv() {
	mon_print("Start your ymodem send now..\n");
	return (ymodem_receive(romdata, ROMSIZE)) > 0;
}

int main(void) {

	sysEnableChipSelect(6);
	P2DDR = 0xFF;
	sysDRAMRASUp();

	static uint8_t dummy[64];
	for (int i = 0; i < sizeof(dummy); i++) {
		dummy[i] = 0xFF;
	}

	tiny_printf("flash writer thingy\n");

	while (1) {
		tiny_printf("press i to identify, press r to read the ROM or press w to write the ROM or q to quit\n");
		char ch = 0;

		while (ch == 0) { // Spin here.
			mon_getch(&ch);
		}

		switch (ch) {

			case 'i': {
				jedecid_t* id = atmel_identify();
				tiny_printf("mfr: 0x%02x dev: 0x%02x\n", (unsigned) id->mfrid, (unsigned) id->deviceid);
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
				if (ymodemrecv()) {
					atmel_identify();
					mon_print("Writing ROM..");
					mon_print("Done\n");
				}
				{
					tiny_printf("ymodem receive failed - errno 0x%02x\n", (int) errno);
				}
				break;

			case 'e':
				tiny_printf("Erasing chip..");
				atmel_chiperase();
				tiny_printf("Done\n");
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

