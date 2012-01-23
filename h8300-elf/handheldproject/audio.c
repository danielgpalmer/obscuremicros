#include <stdint.h>
#include <stdbool.h>


#include "io3069.h"
#include "vect3069.h"
#include "audio.h"
#include "sys.h"
#include "monitor.h"

#include <math.h>

static uint8_t sinetable[SINETABLEMAX] __attribute__((section(".idata")));
static uint8_t increment0;
static uint8_t increment1;

// enable the audio stuff
void audioInit(){
	sysDisableInterrupts;
  	mon_putch("i");
	 
	
	DACR = 0xFF; // turn on the DA?
	
	uint16_t i;
	for(i = 0; i < SINETABLEMAX; i++){
		mon_putch("x");
		sinetable[i] = (sin(SINETABLEENTRYPHASE * i) * 127) + 127;
	}
	
	increment0 = 0; // 
	increment1 = 0;

	mon_putch("e");

	intInstallHandler(CMIA0, (uint32_t) &audioInt);
	
	
	// Setup timers and interrupts
	// Channel 0 on the 8bit timer generates interrupts to update the DA output
	TCNT0_8 = 0x00; // 

	
	mon_putch("b");
	
	TCORA0 = 0x40; // Generate an interrupt every 200 machine cycles.
	TCR0_8 = 0x49; // Enable interrupt on "compare match A" and "clear on compare match A" 

	mon_putch("v");	

	mon_putch("i");
//	sysEnableInterrupts;
}



// output interrupt
void audioInt(){

	static uint16_t current0;
	static uint16_t current1;
		
	DADR0 = sinetable[current0];
	DADR1 = sinetable[current1];

	current0 = current0 + increment0;
	if (current0 >= SINETABLEMAX)
		current0 = 0;

	current1 = current1 + increment1;
	if (current1 >= SINETABLEMAX)
                current1 = 0;
	
	TCSR0 &= 0x31;
}