#include <stdint.h>
#include "io3069.h"

void debugledsInit(){
  P6DDR |= 0x06;
}

void debugledsOn(uint8_t led){
  PADR |= 0x04;
  //P6DR |= 0x06;
}

void debugledsOff(uint8_t led){
  PADR &= 0xFB;
  //P6DR &= 0xF9;
}