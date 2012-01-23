#define DISPLAYMAXX 0x07
#define DISPLAYMAXY 0x0F

#define KS0108_ON 0x3F
#define KS0108_OFF 0x3E



void displayLoadBitmap(uint8_t bitmap[128][8]) __attribute__((section(".itext")));
void displayRefresh() __attribute__((section(".itext")));
void displayOn() __attribute__((section(".itext")));
void displayReset() __attribute__((section(".itext")));
void displayClear() __attribute__((section(".itext")));
void displayCommand(uint8_t command, volatile uint8_t *display) __attribute__((section(".itext")));
void displayData(uint8_t command, volatile uint8_t *display) __attribute__((section(".itext")));
void displayOverlayTilemap(uint8_t tiles[512][1], uint8_t *map, uint8_t originx, uint8_t originy, uint8_t offset) __attribute__((section(".itext")));
void displayClearTiles(uint8_t width, uint8_t height, uint8_t originx, uint8_t originy, uint8_t offset);
void displayInit();
void displayInterrupt(void) __attribute__((section(".itext"))) __attribute__((interrupt_handler));
bool displayRefreshIsDone() __attribute__((section(".itext")));
void displayClearRefreshStatus() __attribute__((section(".itext")));
void displaySetFrameBufferReady() __attribute__((section(".itext")));
void displayClearFrameBuffer() __attribute__((section(".itext")));
void displayCopyTilemapIntoTilemap(uint8_t *srcmap, uint8_t *destmap, uint8_t originx, uint8_t originy) __attribute__((section(".itext")));