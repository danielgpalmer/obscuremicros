#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
#define PAGESIZE 64

uint8_t volatile * const eeprom = (uint8_t volatile * const ) 0xC00000;
static volatile uint8_t romdata[ROMSIZE + 1];

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
	for (int i = 0; i < 512; i++) {
		for (int j = 0; j < 512; j++) {
			// spin
		}
	}
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

static void ymodemsend(void) {
	tiny_printf("Start your ymodem receive now..\n");
	ymodem_send(romdata, ROMSIZE, "rom.bin");
}

static bool ymodemrecv(void) {
	memset((void*) romdata, 0, ROMSIZE);
	romdata[ROMSIZE] = 0xAA;
	tiny_printf("Start your ymodem send now..\n");
	int len = (ymodem_receive(romdata, ROMSIZE));
	if (romdata[ROMSIZE] != 0xAA) {
		for (int i = 0; i < 255; i++) {
			tiny_printf("canary got killed\n");
		}
	}
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
		default:
			return "unknown error\n";
	}

	return NULL;
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

#define PRINTABLECHAR(ch) ( ((ch >= 0x20) && (ch <= 0x7e)) ? ch : 0x20 )

#define HEXLINE "0x%02x 0x%02x 0x%02x 0x%02x [%c%c%c%c]  "

// offset isn't an offset in the bytes, it's just for the printing!
static void printfhexblock(uint8_t* bytes, unsigned offset, unsigned rows) {
	for (unsigned r = 0; r < rows * 16; r += 16) {
		tiny_printf("0x%04x - "
		HEXLINE
		HEXLINE
		HEXLINE
		HEXLINE
		"\n", (int) offset + r, (int) bytes[r + 0], (int) bytes[r + 1], (int) bytes[r + 2], (int) bytes[r + 3],
				(int) PRINTABLECHAR(bytes[r + 0]), (int) PRINTABLECHAR(bytes[r + 1]), (int) PRINTABLECHAR(bytes[r + 2]),
				(int) PRINTABLECHAR(bytes[r + 3]), (int) bytes[r + 4], (int) bytes[r + 5], (int) bytes[r + 6],
				(int) bytes[r + 7], (int) PRINTABLECHAR(bytes[r + 4]), (int) PRINTABLECHAR(bytes[r + 5]),
				(int) PRINTABLECHAR(bytes[r + 6]), (int) PRINTABLECHAR(bytes[r + 7]), (int) bytes[r + 8],
				(int) bytes[r + 9], (int) bytes[r + 10], (int) bytes[r + 11], (int) PRINTABLECHAR(bytes[r + 8]),
				(int) PRINTABLECHAR(bytes[r + 9]), (int) PRINTABLECHAR(bytes[r + 10]),
				(int) PRINTABLECHAR(bytes[r + 11]), (int) bytes[r + 12], (int) bytes[r + 13], (int) bytes[r + 14],
				(int) bytes[r + 15], (int) PRINTABLECHAR(bytes[r + 12]), (int) PRINTABLECHAR(bytes[r + 13]),
				(int) PRINTABLECHAR(bytes[r + 14]), (int) PRINTABLECHAR(bytes[r + 15]));
	}
}
static volatile uint8_t dummy[64];

static void writebuffertochip() {
	for (uint16_t page = 0; page < ROMSIZE; page += 64) {
		tiny_printf("Offset 0x%04x\n", (int) page);
		// FIXME this fucking sucks.. copying the data from DRAM causes 0 to get written!!
		for (int i = 0; i < 64; i++) {
			dummy[i] = romdata[page + i];
			if (dummy[i] != romdata[page + i]) {
				tiny_printf("ON NO!!!\n");
			}
		}
		tiny_printf("data in buffer\n");
		printfhexblock(dummy, page, 4);
		atmel_writepage(false, false, page, dummy);
		for (uint8_t i = 0; i < 64; i++) {
			if (eeprom[page + i] != dummy[i]) {
				tiny_printf("Write failed at page 0x%x byte 0x%x; Wrote 0x%x got 0x%x\n", page, i, dummy[i],
						eeprom[page + i]);
			}
		}
	}
}

int main(void) {

	sysEnableChipSelect(6);
	P2DDR = 0xFF;
	sysDRAMRASUp();

	for (int i = 0; i < sizeof(dummy); i++) {
		dummy[i] = 0xFF;
	}

	SCR1 &= 0x3f; // turn off the interrupts.. shame it breaks the monitor
	tiny_printf("A crappy little flash writer thingy\n");

	while (1) {
		tiny_printf("-- Menu --\n");
		tiny_printf("i - identify\n");
		tiny_printf("l - load buffer via ymodem\n");
		tiny_printf("s - save buffer via ymodem\n");
		tiny_printf("D - dump current temp memory content to output\n");
		tiny_printf("d - dump ROM to output\n");
		tiny_printf("r - read ROM to buffer\n");
		tiny_printf("w - write buffer to the ROM\n");
		tiny_printf("t - write a pattern to the rom\n");
		tiny_printf("e - erase chip\n");
		tiny_printf("q - GTFO!\n");

		char ch = 0;
		crappygetchar(&ch);

		switch (ch) {

			case 'i': {
				jedecid_t* id = atmel_identify();
				tiny_printf("mfr: 0x%02x dev: 0x%02x\n", (unsigned) id->mfrid, (unsigned) id->deviceid);
			}
				break;

			case 's':
				ymodemsend();
				break;
			case 'r':
				// read rom
				tiny_printf("Reading ROM into memory..");
				for (uint16_t i = 0; i < ROMSIZE; i++) {
					romdata[i] = flash_read_byte(i);
				}
				tiny_printf("Done\n");
				break;

			case 'd':
				tiny_printf("Dumping ROM .. hold tight!\n");
				for (unsigned r = 0; r < ROMSIZE; r += PAGESIZE) {
					tiny_printf("-- page --\n");
					printfhexblock(&(eeprom[r]), r, 4);
				}
				break;

			case 'D':
				tiny_printf("Dumping temp data.. hold tight!\n");

				for (unsigned r = 0; r < ROMSIZE; r += PAGESIZE) {
					tiny_printf("-- page --\n");
					printfhexblock(&(romdata[r]), r, 4);
				}
				break;

			case 'l':
				if (ymodemrecv()) {
					tiny_printf("buffer saved\n");
				}
				else {
					tiny_printf("ymodem receive failed - %d, %s\n", (int) errno, decodeymodemerror(errno));
				}
				break;

			case 'w':
				// write rom there

				//atmel_identify();
				_sleep(0);
				tiny_printf("Writing ROM..\n");
				writebuffertochip();
				tiny_printf("Done\n");

				break;

			case 't':
				//atmel_identify();
				_sleep(0);
				tiny_printf("Writing test pattern to ROM..\n");
				for (uint16_t page = 0; page < ROMSIZE; page += 64) {
					for (int i = 0; i < 64; i++) {
						romdata[page + i] = (page & 0xF0) + i;
						if (romdata[page + i] & 0x1) {
							romdata[page + i] = ~romdata[page + i];
						}
					}
				}
				writebuffertochip();
				tiny_printf("Done\n");

				break;

			case 'e':
				tiny_printf("Erasing chip..");
				atmel_chiperase();
				tiny_printf("Done\n");
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

