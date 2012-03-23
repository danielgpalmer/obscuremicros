#define sysDisableInterrupts asm("orc.b #0x80,ccr")
#define sysEnableInterrupts asm("andc.b #0x7f,ccr")

#define sysSetUI asm("orc.b #0x40,ccr")
#define sysClearUI asm("andc.b #0xbf,ccr")

#define sysNop asm("nop")

#define EIGHTBITCLOCKCLEAR	0xF8
#define EIGHTBITCLOCKDIV8	0x01
#define EIGHTBITCLOCKDIV64	0x02
#define EIGHTBITCLOCKDIV8096	0x03
#define EIGHTBITCLOCKOVERFLOW	0x04

#define EIGHTBITINTCLEAR	0x1F
#define EIGHTBITINTCMIA		0x80
#define EIGHTBITINTCMIB 	0x40
#define EIGHTBITINTOVI		0x20

void sysEnableDigitalToAnalogueConverter(uint8_t dan);
void sysDisableDigitalToAnalogueConverter(uint8_t dan);
void sysEnableChipSelect(uint8_t csn);
void sysDisableChipSelect(uint8_t csn);
void sysEnableClockOutput();
void sysDisableClockOutput();
void sysEnableWaite();
void sysSetEightBitTimerClockSource(uint8_t tn, uint8_t sourcemask);
void sysDRAMRASUp();
bool sysIsUISet() __attribute__((section(".itext")));