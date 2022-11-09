/*
 * gpio.c
 *
 *  Created on: 12.07.2021
 *      Author: rb
 */

#include "gpio.h"
#include "chip.h"



uint8_t portPinIO[IO_LAST_PORT][5] = {
        // port, pin, io, state, inverted
		{0,  3, OUT, OFF, 0}, 	// LED1
		{2,  6, OUT, OFF, 0}, 	// LED2
		{2,  7, OUT, OFF, 0}, 	// LED3
		{0,  6, OUT, OFF, 0}, 	// LED4
		{0,  9, OUT, OFF, 0}, 	// BOARD LED
};

void initGPIO() {

    // GPIO system
    Chip_GPIO_Init(LPC_GPIO);


    // UART
    /* we use only PIO1_7 (TX) for UART, PIO 1_6 is used for PWM)
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_6, IOCON_FUNC1);
    */
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_7, IOCON_FUNC1);



    // LED41
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_3, IOCON_FUNC0);

    // LED42
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_6, IOCON_FUNC0);

    // LED3
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO2_7, IOCON_FUNC0);

    // LED4
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_6, IOCON_FUNC0);

    // BOARD LED
    Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO0_9, IOCON_FUNC0);


    for (int i=0; i<IO_LAST_PORT; i++)
    {
    	uint8_t port = portPinIO[i][0];
    	uint8_t pin = portPinIO[i][1];
    	uint8_t isOut = portPinIO[i][2];
    	uint8_t state = portPinIO[i][3];
    	uint8_t inverted = portPinIO[i][4];

        if (isOut) {
            Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
            Chip_GPIO_SetPinState(LPC_GPIO, port, pin, (inverted ? !state : state));
        }
        else {
        	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
        }
    }
}

void setGPIO(IOPort port, IOState state)
{
	if (portPinIO[port][2]) // is output
	{
		if (state == TOGGLE) {
			Chip_GPIO_SetPinToggle(LPC_GPIO, portPinIO[port][0], portPinIO[port][1]);
		}
		else if (portPinIO[port][4]) { // is inverted
			Chip_GPIO_SetPinState(LPC_GPIO, portPinIO[port][0], portPinIO[port][1], !state);
		}
		else {
			Chip_GPIO_SetPinState(LPC_GPIO, portPinIO[port][0], portPinIO[port][1], state);
		}
	}
}


IOState getGPIO(IOPort port)
{
	IOState ret = OFF;
	if (Chip_GPIO_GetPinState(LPC_GPIO, portPinIO[port][0], portPinIO[port][1])) {
		ret = ON;
	}
	if (portPinIO[port][4]) { // is inverted
		ret = !ret;
	}
	return ret;
}
