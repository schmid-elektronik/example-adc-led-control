/*
 * uartHandler.c
 *
 *  Created on: 15.10.2018
 *      Author: rb
 */

#include "chip.h"
#include "uartHandler.h"



RINGBUFF_T rxRing;
RINGBUFF_T txRing;

char rxBuffer[RING_SIZE];
char txBuffer[RING_SIZE];



void UART_IRQHandler() {
	Chip_UART_IRQRBHandler(LPC_USART, &rxRing, &txRing);
}


void initRingBuffers()
{
	RingBuffer_Init(&rxRing, rxBuffer, 1, RING_SIZE);
	RingBuffer_Init(&txRing, txBuffer, 1, RING_SIZE);
}


void initUART(int baudrate)
{
	initRingBuffers();

	NVIC_DisableIRQ(UART0_IRQn);
	/* Setup UART */
	Chip_UART_Init(LPC_USART);
	Chip_UART_SetBaud(LPC_USART, baudrate);
	Chip_UART_TXEnable(LPC_USART);
	Chip_UART_IntEnable(LPC_USART, UART_IER_THREINT | UART_IER_RBRINT); // receive and transmit int

	NVIC_EnableIRQ(UART0_IRQn);
}

void sendUART(const void *data, int dlc)
{
//	Chip_UART_SendRB(LPC_USART, &txRing, data, dlc);
	Chip_UART_SendBlocking(LPC_USART, data, dlc);
}


