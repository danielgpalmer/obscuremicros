// vectors

#ifndef _VECT3069_H
#define _VECT3069_H

void defaultinthandler(void) __attribute__((interrupt_handler));
void installdefaulthandler(void);
void intInstallHandler(uint8_t vector, uint32_t handleraddress);

#define NMI	7
#define IRQ0	12
#define	IRQ1	13
#define	IRQ2	14
#define	IRQ3	15
#define	IRQ4	16
#define	IRQ5	17

#define	WOVI	20
#define CMI	21

#define ADI	23
#define IMIA0	24
#define IMIB0	25
#define OVI0	26

#define IMIA1	28
#define IMIB1	29
#define	OVI1	30

#define IMIA2	32
#define IMIB2	33
#define OVI2	34

#define CMIA0	36
#define CMIB0	37
#define CMIAB1	38

#endif
