#include <stdbool.h>
#include <stdint.h>

#include "io3069.h"
#include "input.h"

uint8_t inputCurrentButtons;

void inputUpdate(){
  PADR &= 0xFE;
   inputCurrentButtons= PADR;
  PADR |= 0x01;
}

bool inputIsUp(){
  if ((inputCurrentButtons & BUTTONUP) == BUTTONUP){
    return true;
  }
  else {
    return false;
  }
}

bool inputIsDown(){
  if ((inputCurrentButtons & BUTTONDOWN) == BUTTONDOWN){
    return true;
  }
  else {
    return false;
  }
}

bool inputIsFire(){
  if ((inputCurrentButtons & BUTTONFIRE) == BUTTONFIRE){
    return true;
  }
  else {
    return false;
  }
}

bool inputIsLeft(){
  if ((inputCurrentButtons & BUTTONLEFT) == BUTTONLEFT){
    return true;
  }
  else {
    return false;
  }
}

bool inputIsRight(){
  if ((inputCurrentButtons & BUTTONRIGHT) == BUTTONRIGHT){
    return true;
  }
  else {
    return false;
  }
}