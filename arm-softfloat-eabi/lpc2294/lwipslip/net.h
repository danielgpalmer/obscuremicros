/*
 * net.h
 *
 *  Created on: 30 Apr 2011
 *      Author: daniel
 */

#ifndef NET_H_
#define NET_H_

void net_init();
void net_loop();
void sio_setuprxint();
void sio_rxinthandler();

#endif /* NET_H_ */
