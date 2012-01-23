#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "monitor.h"
#include "display.h"

#define CHECKTABLESIZE 16

#define BLANK 0x26
#define BLANKLINESIZE 10
#define BLANKLINECANARY 0xaa
uint8_t blankline[BLANKLINESIZE + 1] = { 8, 1,
				     BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
				     BLANKLINECANARY
				   };

/** Detects whether a tetromap would cause a collision in the game state
    @param blockposx 
    @param blockposy
    @param checktable
    @param tetromap
    @return true if the tetro map causes a collision, false otherwise.
*/
				   
bool detectcollision(uint8_t blockposx, uint8_t blockposy, uint8_t checktable[], uint8_t *tetromap){
  
  // Some uint8_t's that we'll be using today
  uint8_t x,y;
  
  // get the width and height out of the tetro map.
  uint8_t width = *(tetromap);
  uint8_t height = *(tetromap + 1);
  
  // I honestly can't remember what this is for
  if(blockposy == 0){
    return false;
  }
  
  // Quick checks before doing anything intensive
  // if all the lines the tetro map is being checked against are
  // empty just return false. If any of the lines are 
  // totally full we can safely return true with out checking
  // any further.
  
  // x is being used here to represent a bool .. it's initially
  // set to 0x01 .. if a line isn't clear it is set to 0x00.
  // at the end x is checked to see if it's still 0x01; If it 
  // is all the lines were clean and false is returned.
  
  x = 0x01;
  for ( y = 0; y < height; y++){
    uint8_t value = checktable[blockposy + y];
    if(value == 0xFF){
      return true;
    }
    else if (value != 0x00){
      x = 0x00;
    }
  }
  if (x == 0x01){
    return false;
  }
    
  // more intensive checks
  for (y = 0; y < height; y++){
    for (x = 0; x <  width; x++){
      uint8_t checkbit = (checktable[blockposy + y] >> (blockposx + x)) & 0x01;
      if((checkbit == 0x01) && (*(tetromap + (width * y) + x + 2) != 0xFF)){
	return true;
      }
    }
  }
  return false;
  
  
}

/** 
    @return true if complete lines have been detected
*/

bool detectcompletelines(uint8_t checktable[], uint8_t playfield[]){
  bool returnval = false;
  uint8_t y;
  uint8_t foo, bar;
  uint8_t count = 0;
  
  // find complete lines and clear them;
  for (y = 0; y < CHECKTABLESIZE; y++){
    if(checktable[y] == 0xFF){
      checktable[y] = 0x00;
      //count++;
     //score++;
      returnval = true;
      displayCopyTilemapIntoTilemap(blankline, playfield, 0, y);
    //  mon_putch("r");
    }
  }
  
  if(returnval == true){
  for(foo = 0; foo < 16; foo++){
  // shiftlines down
   // mon_putch("y");
    for (y = (CHECKTABLESIZE - 1); y > 0; y--){
      if(checktable[y] != 0x00){
	if(checktable[y - 1] == 0x00){
	  checktable[y - 1] = checktable[y];
	  checktable[y] = 0x00;
	//  mon_putch("o");
	  
	  
	  uint8_t dstoffset = (8 * (y - 1)) + 2;
	  uint8_t srcoffset = (8 * y) + 2;
	  for (bar = 0; bar < 8; bar++){
	    playfield[dstoffset + bar] = playfield[srcoffset + bar]; 
	  }
	  displayCopyTilemapIntoTilemap(blankline, playfield, 0, y);
	}
      }
    }
  }
  }
  return returnval;
}