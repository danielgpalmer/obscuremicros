#include <stdint.h>
#include "nvram.h"

volatile uint8_t *nvram  = (uint8_t*) 0x200000;


uint16_t nvramCounter;

void nvramWriteBlock(){
        for (nvramCounter = 0; nvramCounter <= 0xff; nvramCounter++){
		//The databus on my creation is a little bit wonky.
		//until I can properly debug that I've decide to A:
		//do a dummy write before writing, B: bash the data
		//into the NVRAM until it sticks.

                *nvram = 0xFF; 				
		while(*nvram != (uint8_t)nvramCounter){		
			*nvram = (uint8_t)nvramCounter;		
		}
		nvram++;
        }
}

void nvramReadBlock(uint8_t *buffer){
  for (nvramCounter = 0; nvramCounter <= 0xff; nvramCounter++){
                *buffer++ = *nvram++;
  }
}
