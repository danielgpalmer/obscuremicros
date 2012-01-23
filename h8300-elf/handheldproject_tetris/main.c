#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "io3069.h"
#include "vect3069.h"
#include "monitor.h"
#include "sys.h"
#include "display.h"
#include "audio.h"
#include "input.h"
#include "splash.h"
#include "tileset.h"
#include "main.h"
#include "tetromap.h"
#include "debugleds.h"


#define DEBUG
#include "assert.h"



// this is a special tile map that contains the current game state
	// use this for redrawing the whole screen!

#define BLANK 0x26
#include "playfield.h"


				  
void clearplayfield(){
 uint8_t foo; 
 for (foo = 2; foo < 130; foo++){
  playfield[foo] = BLANK;
 }
}
				  
#include "checktable.h"
				   


#include "detection.h"



int main(void)
{
  
	uint16_t score = 0;
uint8_t blockposx = 0;
uint8_t blockposy = 16;


  
	SYSCR |= 0x08;
	PADDR = 0x07;
	P5DDR = 0xFF;
	P5DR = 0x00;
	P5DR = 0x0F;
	// enable CS6 and CS7
	sysEnableChipSelect(1);
	sysEnableChipSelect(2);
	sysEnableChipSelect(6);
	sysEnableChipSelect(7);

	sysDRAMRASUp();
	
	
	
	
	sysEnableWaite();
	displayReset();
	displayOn();
	displayClear();
	
	
	uint8_t myTilemap[10] = { 8, 1, 7, 8, 19, 38, 5, 8, 17, 4};
	
	debugledsInit();
	
	installdefaulthandler();
	//audioInit();
	displayInit();
	//return 0;
	
	sysEnableInterrupts;
	
	displayOverlayTilemap(tileset, (uint8_t*) myTilemap, 0, 3, 0);
	
	uint8_t state = 0x00; 
	uint8_t framecounter = 0x00;
	uint8_t subcounter = 0x00;
	
	const uint8_t *tetrofirst = (uint8_t*) &tetroL;
	const uint8_t *tetrolast = (tetrofirst + (TETROTABLESIZE - 32));
	uint8_t *tetromapstart = (uint8_t*) &tetroL;
	uint8_t *tetromapcurrent = (uint8_t*) &tetroL;
	
	displayClearRefreshStatus();
	
	bool hasoverflown = false;
	displayLoadBitmap(splash);
	while(state != STATE_EXIT){
	  if(displayRefreshIsDone()){
	    
	    
	    switch(state){
	      case 0x00 :
		if(inputIsFire()){
		  state = STATE_GAME_DROPPING_0;
		  hasoverflown = false;
		  framecounter = 0;
		  displayClearFrameBuffer();
		}
		
		else {
		  displayOverlayTilemap(tileset, (uint8_t*) myTilemap, 0, 3, 0);
		  state = STATE_INTRO_1;
		  displaySetFrameBufferReady();
		}
		
		break;
			    
	    case 0x01 :	    
	      
	      if(framecounter > 29){
		state = STATE_INTRO_2;
		framecounter = 0;
	      }
			    
	      if(inputIsFire()){
		state = STATE_GAME_DROPPING_0;
		hasoverflown = false;
		framecounter = 0;
		displayClearFrameBuffer();
	      }
	      
	      break;
			    
	    case 0x02 :	    
			    if(inputIsFire()){
	    			state = STATE_GAME_DROPPING_0;
				framecounter = 0;
				hasoverflown = false;
				displayClearFrameBuffer();
			    }
			    else {
			      displayClearTiles(8,1,0,3,0);
			      state = STATE_INTRO_3;
			      displaySetFrameBufferReady();
			    }
			    break;
			    
	    case 0x03 :	    if(framecounter > 29){
			      state = STATE_INTRO_0;
			      framecounter = 0;
			    }
			    if(inputIsFire()){
	    			state = STATE_GAME_DROPPING_0;
				framecounter = 0;
				displayClearFrameBuffer();
				hasoverflown = false;
			    }
			    break;
	    case 0x04:	    
			    if((blockposy == 0) || detectcollision(blockposx, blockposy - 1, checktable, tetromapcurrent)) {
			      //mon_putch("f");
			      uint8_t x, y;
			      uint8_t *tetrodata = (tetromapcurrent + TETROHEADEROFFSET); 
			      // mark the check table
			      for ( x = 0; x <  *(tetromapcurrent); x++){
				for ( y = 0; y < *(tetromapcurrent + 1); y++ ){
				  if(*tetrodata++ != 0xFF) 
				  {
				    checktable[blockposy + y] |= (0x01 << (blockposx + x));
				    
				  }
				}
			      }
			      
			      displayCopyTilemapIntoTilemap(tetromapcurrent, playfield, blockposx, blockposy);
			      
			      if(detectcompletelines(checktable, playfield)){
				displayOverlayTilemap(tileset, playfield, 0, 0, 0);
			      }
			      // dumpplayfield();
			      
			      //for ( y = 0; y < 8; y++){
				//mon_putch((char *) &checktable[y]);
			      //}
			      
			      if(blockposy == 16){
				hasoverflown = true;
			      }
			      
			      if(blockposy == 15 && (*(tetromapcurrent + 1) > 1)){
				hasoverflown = true;
			      }
			      
			      blockposy = 16;
			      
			      
			      tetromapcurrent = tetromapstart;
			      
			      
			      
			    } 
			    else {
				displayClearTiles(*(tetromapcurrent), *(tetromapcurrent + 1), blockposx, blockposy, 0); // clear the previous blocks
				blockposy--;
			    }
			    
			    
			    
			    displayOverlayTilemap(tileset, tetromapcurrent, blockposx, blockposy, 0);
			    displaySetFrameBufferReady();
			    if(hasoverflown){
			      clearplayfield();
			      displayClearFrameBuffer();
			      displayLoadBitmap(splash);
			      uint8_t foobar;
			      for(foobar = 0; foobar < 16; foobar++){
			      checktable[foobar] = 0x00;
			      }
			      blockposy = 16;
			      state = STATE_INTRO_0;
			    }
			     else{ 
			      state = STATE_GAME_DROPPING_1;
			    
			    }
			    framecounter = 0;
			    subcounter=0;
			    break;
	    case 0x05: 
			    framecounter++;
			    subcounter++;
			    if(subcounter > 2){
			      subcounter = 0;
			    
			      if(inputIsRight()){
				if((blockposx + *(tetromapcurrent)) < 8){
				  if(!detectcollisionright(blockposx, blockposy, checktable)){
				    displayClearTiles(*(tetromapcurrent), *(tetromapcurrent + 1), blockposx, blockposy, 0);
				    blockposx++;
				    displayOverlayTilemap(tileset, tetromapcurrent, blockposx, blockposy, 0);
				    displaySetFrameBufferReady();
				  }
				}
			      }
			      
			      if(inputIsLeft()){
				if(blockposx > 0){
				  if(!detectcollisionleft(blockposx, blockposy, checktable)){
				    displayClearTiles(*(tetromapcurrent), *(tetromapcurrent + 1), blockposx, blockposy, 0);
				    blockposx--;
				    displayOverlayTilemap(tileset, tetromapcurrent, blockposx, blockposy, 0);
				    displaySetFrameBufferReady();
				  }
				}
			      }
			      
			      if(inputIsFire()){
				mon_putch("R");
				tetromapcurrent = (tetromapcurrent + 8);
				if(tetromapcurrent > (tetromapstart + 24)){
				  tetromapcurrent = tetromapstart;
				}
			      }
			      
			    }
			    
			    if(inputIsDown()){
			     framecounter++; 
			    }
			    
			    if(framecounter > 10){
			      state = STATE_GAME_DROPPING_0;
			    }
			    break;  
			    
	  }

	  
	  framecounter++;
	  displayClearRefreshStatus();
	  }
	}
	
	return 0;
}


