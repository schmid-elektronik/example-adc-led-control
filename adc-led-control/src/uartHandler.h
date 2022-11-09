/*
 * uartHandler.h
 *
 *  Created on: 15.10.2018
 *      Author: rb
 */

#ifndef UARTHANDLER_H_
#define UARTHANDLER_H_


#define RING_SIZE (0x100)

void initUART();
void sendUART(const void *data, int dlc);

#endif /* UARTHANDLER_H_ */
