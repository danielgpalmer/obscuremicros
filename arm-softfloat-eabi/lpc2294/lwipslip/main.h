#ifndef MAIN_H_
#define MAIN_H_

void net_init();
void net_loop();



#define TOKENSEPERATOR 0x20
#define CMD_NOCMD 0x00
#define CMD_READ 0x01
#define CMD_FPGA 0x02

#define CMD_PING 0x03

char* command;
char* arg0;
char* arg1;

int parsecmd();

void cmdfpga(char* arg0, char* arg1);
uint16_t fpgagetword(uint8_t bank, uint8_t address);
void fpgaputword(uint8_t bank, uint8_t address, uint16_t word);

void disableInterrupts(void);

#endif
