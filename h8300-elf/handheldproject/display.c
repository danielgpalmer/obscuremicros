#include <stdbool.h>
#include <stdint.h>

#include "io3069.h"
#include "display.h"
#include "input.h"
#include "monitor.h"
#include "sys.h"
#include "vect3069.h"

#include "debugleds.h"


uint8_t displayDirtyCols[8] __attribute__((section(".ibss")));
uint8_t displayDirtyRows[16] __attribute__((section(".ibss")));

uint8_t displayFrameBuffer[1024] __attribute__((section(".ibss")));
bool	displayFrameBufferReady __attribute__((section(".ibss")));
int displayVar __attribute__((section(".ibss"))); //
char displayChar __attribute__((section(".ibss"))); //

uint8_t displayFrameBuffer[1024];
bool	displayFrameBufferReady = false;
int displayVar;

bool	displayRefreshDone __attribute__((section(".ibss")));

void displayCommand(uint8_t command, volatile uint8_t *display){
  //while((*display & 0x80) == 0x80){
  //                     sysNop;
  //}
 
  *display = command;
  
}

void displayData(uint8_t command, volatile uint8_t *display){
  //while((*display & 0x80) == 0x80){
   //                    sysNop;
  //}
 
  *(display + 1) = command; 
  
}

void displayOn(){
  volatile uint8_t *displayPanel1  = (uint8_t*) 0xC00000;
  volatile uint8_t *displayPanel2 = (uint8_t*) 0xE00000;
  
  displayCommand(KS0108_ON, displayPanel1);
  displayCommand(0x40, displayPanel1);
  displayCommand(0xB8, displayPanel1);
  displayCommand(0xC0, displayPanel1);

  displayCommand(KS0108_ON, displayPanel2);
  *displayPanel2 = 0x40;
  *displayPanel2 = 0xB8;
  *displayPanel2 = 0xC0;
// DIVCR = 0x00; // restore system clock.;
}

void displayOff(){
  volatile uint8_t *displayPanel1  = (uint8_t*) 0xC00000;
  volatile uint8_t *displayPanel2 = (uint8_t*) 0xE00000;
  displayCommand(KS0108_OFF, displayPanel1);
  displayCommand(KS0108_OFF, displayPanel2);
}

void displayReset(){

  //PADDR = 0x03;
  
  for(displayVar = 0; displayVar < 0xff; displayVar++){
    PADR &= 0xFD; // Put reset for screen high
  }

  for(displayVar = 0; displayVar < 0xff; displayVar++){
    PADR |= 0x02; // Put reset for screen high
  }
}


void displayRefresh(){
  volatile uint8_t *displayPanel1 = (uint8_t*) 0xC00000;
  volatile uint8_t *displayPanel2  = (uint8_t*) 0xE00000;
  
  if(displayFrameBufferReady == true){
  
  

	uint8_t x, y, line;
	
	displayCommand(0xC0, displayPanel1);
	displayCommand(0xC0, displayPanel2);
			DIVCR = 0x02; // Slow the machines system clock down 
			for(x = 0; x < 8; x++){
			  //displayCommand(0x40, displayPanel1);
			  displayCommand((0xB8 + x), displayPanel1);
			  for(y = 0; y < 8; y++){
			    if((displayDirtyCols[x] = 0x01) && (displayDirtyRows[y] = 0x01)){
			      displayCommand((0x40 + (y * 8)), displayPanel1);
			      for(line = 0; line < 8; line++){		
				displayData(displayFrameBuffer[(128 * x) + (y * 8) + line], displayPanel1);
				displayDirtyCols[x] = 0x00;
				displayDirtyRows[y] = 0x00;
				
			      }
			    }
			  }
			}

			for(x = 0; x < 8; x++){
			  //displayCommand(0x40, displayPanel1);
			  displayCommand((0xB8 + x), displayPanel2);
			  for(y = 0; y < 8; y++){
			    if((displayDirtyCols[x] = 0x01) && (displayDirtyRows[y] = 0x01)){
			      displayCommand((0x40 + (y * 8)), displayPanel2);
			      for(line = 0; line < 8; line++){		
				displayData(displayFrameBuffer[(128 * x) + (y * 8) + line + 64], displayPanel2);
				displayDirtyCols[x] = 0x00;
				displayDirtyRows[y] = 0x00;
				
			      }
			    }
			  }
			}
			
			DIVCR = 0x00;
    displayFrameBufferReady = false;
  }
}

void displayClear(){
  volatile uint8_t *displayPanel1 = (uint8_t*) 0xC00000; // bottom 64x64
  volatile uint8_t *displayPanel2  = (uint8_t*) 0xE00000; // top 64x64

	DIVCR = 0x02; // Slow the machines system clock down 

	uint8_t x, y;

	displayCommand(0xC0, displayPanel1);
	displayCommand(0xC0, displayPanel2);
			
			for(x = 0; x < 8; x++){
			  displayCommand(0x40, displayPanel1);
			  displayCommand((0xB8 + x), displayPanel1);
			  for(y = 0; y < 64; y++){
			    if((displayDirtyCols[x] = 0x01) && (displayDirtyRows[y] = 0x01)){
			      displayData(0xaa, displayPanel1);
			    }
			  }
			}
			
			for(x = 0; x < 8; x++){
			  displayCommand(0x40, displayPanel2);
			  displayCommand((0xB8 + x), displayPanel2);
			  for(y = 0; y < 64; y++){
			    if((displayDirtyCols[x] = 0x01) && (displayDirtyRows[y] = 0x01)){
			    displayData(0x00, displayPanel2);
			    }
			  }
			}
			
	DIVCR = 0x00; // restore system clock.
}

// this will take a 64x128 linear bitmap and load it into the framebuffer
// in the native format
void displayLoadBitmap(uint8_t bitmap[128][8]){
  uint8_t *framebuffer = (uint8_t*) &displayFrameBuffer;
  int x,y;
  
  for(x = 0; x < 8; x++){
   for (y= 0; y < 128; y++){
      *framebuffer++ = bitmap[y][x];
      displayDirtyRows[x] = 0x01;
	displayDirtyCols[(y/8)] = 0x01;
    }
  }
}

// this will take a map of 8x8 tiles and render them into the framebuffer
// in the native format
void displayLoadTileMap(uint8_t tiles[512][1], uint8_t *map){
  //displayOverlayTilemap(tiles, map, 8, 16, 0, 0);
}

void displayOverlayTilemap(uint8_t tiles[512][1], uint8_t *map, uint8_t originx, uint8_t originy, uint8_t offset){
  uint8_t x,y, tilelinenum;
  
  uint8_t *framebuffer = (uint8_t*) &displayFrameBuffer;
  
  uint8_t width = *map++;
  uint8_t height = *map++;
  
  for (y = 0; y < height; y++){
    for (x = 0; x < width; x++){
      uint8_t tilenum = *map++;
      if(tilenum != 0xFF){
	
	uint8_t tileline;
	
	if((!((originy + y) > DISPLAYMAXY)) && (!((originx + x) > DISPLAYMAXX))){
	  for (tilelinenum = 0; tilelinenum < 8; tilelinenum++){
	    tileline = tiles[(503 - (tilenum * 8)) + tilelinenum][1];
	    *(framebuffer + (128 * (originx + x)) + ( 8 * (originy + y)) + tilelinenum + offset) = tileline;  
	  }
	  
	  displayDirtyRows[originx + x] = 0x01;
	  displayDirtyCols[originy + y] = 0x01;
	}
      }
    }
  }
}

void displayCopyTilemapIntoTilemap(uint8_t *srcmap, uint8_t *destmap, uint8_t originx, uint8_t originy){
  uint8_t srcwidth = *srcmap;
  uint8_t srcheight = *(srcmap + 1);
  
  uint8_t dstwidth = *destmap;
  uint8_t dstheight = *(destmap + 1);
  
  //if((srcwidth + originx) > dstwidth || (srcheight + originy) > dstheight){
  //  return;
  //}
  
  uint8_t x;
  uint8_t y;
  
  uint8_t mapbyte;
  for (y = 0; y < srcheight; y++){
    uint8_t *srcoffset = srcmap + ((srcwidth * y) + 2);
    uint8_t *dstoffset = destmap + ((dstwidth * (y + originy)) + 2 + originx);
    for (x = 0; x < srcwidth; x++){
	mapbyte = *srcoffset++;
	if(mapbyte != 0xFF){
	  *dstoffset++ = mapbyte;
	}
    }
  }
  
  
}


void displayClearTiles(uint8_t width, uint8_t height, uint8_t originx, uint8_t originy, uint8_t offset){
  uint8_t *framebuffer = (uint8_t*) &displayFrameBuffer;
  uint8_t x, y, foo;
  for (x = 0; x < width; x++){
    for (y = 0; y < height; y++){
      if((!((originy + y) > DISPLAYMAXY)) && (!((originx + x) > DISPLAYMAXX))){
      for (foo = 0; foo < 8; foo++){
	 *(framebuffer + (128 * (originx + x)) + ( 8 * (originy + y)) + foo + offset) = 0x00;
	 
      }
      displayDirtyRows[originx + x] = 0x01;
      displayDirtyCols[originy + y] = 0x01;
    }
    }
  }
}

void displayClearFrameBuffer(){
  uint8_t *framebuffer = (uint8_t*) &displayFrameBuffer;
  uint16_t bar;
  for(bar = 0; bar < 1024; bar++){
    *framebuffer++ = 0x00;
  }
}

void displayInit(){
  intInstallHandler(CMIAB1, (uint32_t) &displayInterrupt);
  TCNT1_8 = 0x00; // 
  TCORA1 = 0x65; // Generate an interrupt every 200 machine cycles.
  TCR1_8 = 0x4B; // Enable interrupt on "compare match A" and "clear on compare match A"
 // sysSetEightBitTimerClockSource(1, EIGHTBITCLOCKDIV8096);
}

void displayInterrupt(){  
  debugledsOn(REDLED);
  inputUpdate();
  displayRefresh(); 
  TCSR1 &= 0x3F;
  displayRefreshDone = true;
  debugledsOff(REDLED);
}

// I wanted to use the UI bit for this
// I thought it would be quick...
// but the UI bit gets reset when the interrupt exits
// Put some wrappers here.. I might change how this 
// actually works later.
bool displayRefreshIsDone(){
  return displayRefreshDone;
}

void displayClearRefreshStatus(){
  displayRefreshDone = false;
}

void displaySetFrameBufferReady(){
  displayFrameBufferReady = true;
}