#define BUTTONUP	0x40
#define	BUTTONDOWN	0x10
#define	BUTTONRIGHT	0x08 
#define	BUTTONLEFT	0x20
#define BUTTONFIRE	0x80

void inputUpdate();
bool inputIsUp();
bool inputIsDown();
bool inputIsFire();
bool inputIsLeft();
bool inputIsRight();