#ifndef SIO_H_
#define SIO_H_

__sfr __at 0x18 sio_cha_srbuffer; // Send/Receive buffer
__sfr __at 0x19 sio_cha_csregister; // Command/Status register
__sfr __at 0x1a sio_chb_srbuffer;
__sfr __at 0x1b sio_chb_csregister;

#endif
