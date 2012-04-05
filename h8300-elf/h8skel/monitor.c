/*
	This file simplifies calling the monitor traps to do stuff like basic serial IO

*/

#include "monitor.h"

void mon_print(char *str)
{
	while(*str) {
		mon_putch(str);
		str ++;
	}
}

void mon_putch(char* ch)
{
        asm("push.l er0");
        asm("push.l er2");
        asm("mov.l er0, er0");
        asm("mov.b @%0, r0l" :: "r" (ch));
        asm("mov.l #0, er2");
        asm(" trapa #2");
        asm("pop.l er2");
        asm("pop.l er0");
}

void mon_getch(char* ch){
	asm("push.l er0");
        asm("push.l er2");
        asm("mov.l er0, er0");
        asm("mov.l #1, er2");
        asm(" trapa #2");
	asm("mov.b r0l, @%0" :: "r" (ch));
        asm("pop.l er2");
        asm("pop.l er0");
}
