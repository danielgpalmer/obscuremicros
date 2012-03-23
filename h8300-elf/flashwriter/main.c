#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "io3069.h"
#include "vect3069.h"
#include "monitor.h"
#include "sys.h"


#define ROMSIZE 32768

int main(void)
{

	sysEnableChipSelect(6);
	P2DDR = 0xFF;

	uint8_t romdata[ROMSIZE];
 	volatile uint8_t* eeprom = (uint8_t*) 0xC00000;

	print("flash write thingy\n");
	print("press r to read the ROM or press w to write the ROM\n");
	
	// read rom
	for(uint16_t i = 0; i < ROMSIZE; i++){
		romdata[i] = eeprom[i];
		print(".");
	}

	print("Done\n");

	return 0;
}


