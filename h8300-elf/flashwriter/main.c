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
#include "printf.h"
#include <errno.h>

#define ROMSIZE 32768

uint8_t volatile * const eeprom = (uint8_t volatile * const ) 0xC00000;
static volatile uint8_t romdata[ROMSIZE];

void _putchar(int c) {
	char ch = (char) c;
	mon_putch(&ch);
}

int _getchar(int timeout) {
	char ch = 0;
	int i, j = 0;
	while (!(SSR1 & 0x40)) {
		i++;
		if (i == 512) {
			i = 0;
			j++;
			if (j == 128) {
				return -1;
			}
		}
	}
	ch = RDR1;
	SSR1 &= ~0x40;
	return ch;
}

void _sleep(unsigned long seconds) {

}

///int serial_read(void) {
//	return -1;
//}

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
	tiny_printf("Start your ymodem receive now..\n");
	ymodem_send(romdata, ROMSIZE, "rom.bin");
}

static bool ymodemrecv() {
	tiny_printf("Start your ymodem send now..\n");
	int len = (ymodem_receive(romdata, ROMSIZE));
	return len > 0;
}

static char* decodeymodemerror(int error) {

	switch (error) {
		case YMODEM_ERROR_ABORT:
			return "Abort";
		case YMODEM_ERROR_BUFFEROVERFLOW:
			return "Buffer Overflow";
		case YMODEM_ERROR_BUFFERTOOSMALL:
			return "Buffer too small";
		case YMODEM_ERROR_TOOMANYRECVERRORS:
			return "Too many recv errors";
	}

}

void crappygetchar(char* ch) {
	while (!(SSR1 & 0x40)) { // Spin here
	};
	*ch = RDR1;
	SSR1 &= ~0x40;

	char junk = 0;
	while (junk != 0x0d) { // drain
		while (!(SSR1 & 0x40)) {
		};
		junk = RDR1;
		SSR1 &= ~0x40;
	}

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

	for (int i = 0; i < 64; i++) {
		romdata[0 + i] = i;
	}

	for (int i = 0; i < 64; i++) {
		dummy[i] = romdata[0 + i];
		tiny_printf("%d - %x\n", i, (int) romdata[i]);
		tiny_printf("%d - %x\n", i, (int) dummy[i]);
	}

	SCR1 &= 0x3f; // turn off the interrupts.. shame it breaks the monitor

	while (1) {
		tiny_printf("press i to identify, press r to read the ROM or press w to write the ROM or q to quit\n");

		char ch = 0;
		crappygetchar(&ch);

		switch (ch) {

			case 'i': {
				jedecid_t* id = atmel_identify();
				tiny_printf("mfr: 0x%02x dev: 0x%02x\n", (unsigned) id->mfrid, (unsigned) id->deviceid);
			}
				break;
			case 'r':
				// read rom
				tiny_printf("Reading ROM into memory..");
				for (uint16_t i = 0; i < ROMSIZE; i++) {
					romdata[i] = flash_read_byte(i);
				}
				tiny_printf("Done\n");
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
					//atmel_identify();

					tiny_printf("Writing ROM..\n");
					for (uint16_t page = 0; page < 128; page += 64) {
						tiny_printf("Offset 0x%04x\n", (int) page);
						for (int i = 0; i < 64; i++) {
							dummy[i] = romdata[page + i];
							tiny_printf("%d - %x\n", i, (int) romdata[page + i]);
							tiny_printf("%d - %x\n", i, (int) dummy[i]);
						}
						atmel_writepage(false, false, page, dummy);
					}
					tiny_printf("Done\n");

					for (int i = 0; i < 128; i++) {
						tiny_printf("v - %d\n", (int) eeprom[i]);
					}
				}
				else {
					tiny_printf("ymodem receive failed - %s\n", decodeymodemerror(errno));
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
				tiny_printf("0x%02x - eh?\n", (unsigned) ch);
				break;
		}

	}

	return 0;
}

