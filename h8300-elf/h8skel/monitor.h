#ifndef MON_H_
#define MON_H_

void mon_putch(char* ch) __attribute__((section(".itext")));
void mon_getch(char* ch) __attribute__((section(".itext")));
void mon_print(char *str) __attribute__((section(".itext")));

#endif
