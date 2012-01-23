#ifndef _AUDIO_H
#define _AUDIO_H

void audioInit();
void audioInt(void) __attribute__((section(".itext"))) __attribute__((interrupt_handler));

#define SINETABLEMAX 0x1FF
#define SINETABLEENTRYPHASE ((2 * 3.141592)/SINETABLEMAX)

#endif
