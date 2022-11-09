/*
 * gpio.h
 *
 *  Created on: 12.07.2021
 *      Author: rb
 */

#ifndef GPIO_H_
#define GPIO_H_


#include <stdint.h>


typedef enum {
	IO_LED1,
	IO_LED2,
	IO_LED3,
	IO_LED4,
	IO_BOARD_LED,
	IO_LAST_PORT
} IOPort;


typedef enum {
	OFF = 0,
	NO = OFF,
	ON,
	YES = ON,
	TOGGLE
} IOState;

typedef enum {
	IN = 0,
	OUT = 1
} IODirection;



void initGPIO();
void setGPIO(IOPort port, IOState state);
IOState getGPIO(IOPort port);

#endif /* GPIO_H_ */
