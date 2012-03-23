#include <stdint.h>
#include "vect3069.h"
#include "monitor.h"
#include "io3069.h"

void defaultinthandler() {
  
  TCSR0 &= 0x31;
  print("x");
}

void installdefaulthandler(){
  uint32_t *vectors = (uint32_t*) 0xFFBF20;
  
  int i;
  for (i = 0; i < 0x40; i++){
    *vectors++ = (uint32_t) &defaultinthandler;
  }
}

void intInstallHandler(uint8_t vector, uint32_t handleraddress){
  uint32_t *vectors = (uint32_t*) 0xFFBF20;
  *(vectors + vector) = handleraddress;
}
