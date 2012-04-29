#define WDT_CLEAR 0x4e
#define WDT_DISABLE 0xb1

__sfr __at 0xf0 wdt_mode; // "Watch Dog Timer Stand-by mode Register"
__sfr __at 0xf1 wdt_command;
