#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "io3069.h"
#include "vect3069.h"
#include "monitor.h"
#include "sys.h"

//int main(void) __attribute__((section(".itext")));




/*
 * The 6 lines below show how to take advantage of relocation
 * from DRAM into SRAM, code running in SRAM using data in SRAM
 * is ALOT faster than anything in DRAM.
 * You may want to use this for interrupt handlers. 
 */
uint8_t iramvar __attribute__((section(".ibss"))); //
uint8_t iramvar = 0x00;
const char iramdata[] __attribute__((section(".idata"))); //
const char iramdata[] = "Hello, world!";
void iramfunction() __attribute__((section(".itext"))); //
void iramfunction(){
	mon_print(iramdata);
	iramvar++;
}



int main(void)
{

	//jump into code in the internal SRAM
	iramfunction();

	// enable CS6 and CS7
	sysEnableChipSelect(6);
	sysEnableChipSelect(7);

	installdefaulthandler();
	
	TCORA0 = 0xDD; // Generate an interrupt every 200 machine cycles.
	TCR0_8 = 0x49; // Enable interrupt on "compare match A" and "clear on compare match A" 

	sysEnableInterrupts;
	sysEnableClockOutput();

	mon_print("Hello, world!\n");
	
	return 0;
}


