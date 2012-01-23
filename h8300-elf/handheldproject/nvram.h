uint16_t nvramCounter __attribute__((section(".ibss")));
void nvramWriteBlock() __attribute__((section(".itext")));
void nvramReadBlock(uint8_t *buffer) __attribute__((section(".itext")));