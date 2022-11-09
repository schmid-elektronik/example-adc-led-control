/*
 * adiaControl.c
 *
 *  Created on: 12.07.2021
 *      Author: rb
 */

#include "chip.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <cr_section_macros.h>
#include "stdlib.h"

#include "uartHandler.h"
#include "adc.h"
#include "gpio.h"
#include "logger.h"


volatile unsigned int tick = 0;

volatile bool go_main = false;
volatile bool go_100ms = false;
volatile bool go_1000ms = false;


uint16_t voltage = 0;



void initTimer()
{
    /* Enable timer 1 clock */
    Chip_TIMER_Init(LPC_TIMER16_1);

    /* Timer rate is system clock rate */
    uint32_t timerFreq = Chip_Clock_GetSystemClockRate();

    /* Timer setup for match and interrupt at TICKRATE_HZ */
    Chip_TIMER_Reset(LPC_TIMER16_1);
//    Chip_TIMER_PrescaleSet(LPC_TIMER16_1,timerFreq/1000); //select ms prescale
    Chip_TIMER_MatchEnableInt(LPC_TIMER16_1, 1);
    Chip_TIMER_SetMatch(LPC_TIMER16_1, 1, timerFreq/1000 - 1);	// 1ms per tick

    Chip_TIMER_ResetOnMatchEnable(LPC_TIMER16_1, 1);
    Chip_TIMER_Enable(LPC_TIMER16_1);

    /* Enable timer interrupt */
    NVIC_ClearPendingIRQ(TIMER_16_1_IRQn);
    NVIC_EnableIRQ(TIMER_16_1_IRQn);
}

void TIMER16_1_IRQHandler(void)
{
	if (Chip_TIMER_MatchPending(LPC_TIMER16_1, 1))
	{
		Chip_TIMER_ClearMatch(LPC_TIMER16_1, 1);
		tick++;
		go_main = true;


		if (tick % 100 == 0) {
			go_main = true;
			go_100ms = true;
		}
		if (tick % 1000 == 0) {
			go_main = true;
			go_1000ms = true;
		}
	}
}




int main(void)
{
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();

    initGPIO();
    initTimer();
    initUART(115200);
    initADC();



    while (true)
    {

    	while (!go_main) {
    		__WFI(); /* Go to Sleep */
    	}

    	if (go_1000ms) {
    		LOG_INFO("Up and running...");
    		LOG_INFO_INT("Voltage:", voltage);
    		setGPIO(IO_BOARD_LED, TOGGLE);
    		go_1000ms = false;
    	}


    	if (go_100ms)
    	{
    		if (readADC()) {
    			voltage = getADCVoltage();
    		}


    		if (voltage < 50) {
    			setGPIO(IO_LED1, OFF);
    			setGPIO(IO_LED2, OFF);
    			setGPIO(IO_LED3, OFF);
    			setGPIO(IO_LED4, OFF);
    		}
    		else if (voltage < 150) {
    			setGPIO(IO_LED1, ON);
    			setGPIO(IO_LED2, OFF);
    			setGPIO(IO_LED3, OFF);
    			setGPIO(IO_LED4, OFF);
    		}
    		else if (voltage < 250) {
    			setGPIO(IO_LED1, ON);
    			setGPIO(IO_LED2, ON);
    			setGPIO(IO_LED3, OFF);
    			setGPIO(IO_LED4, OFF);
    		}
    		else if (voltage < 350) {
    			setGPIO(IO_LED1, ON);
    			setGPIO(IO_LED2, ON);
    			setGPIO(IO_LED3, ON);
    			setGPIO(IO_LED4, OFF);
    		}
    		else {
    			setGPIO(IO_LED1, ON);
    			setGPIO(IO_LED2, ON);
    			setGPIO(IO_LED3, ON);
    			setGPIO(IO_LED4, ON);
    		}
    		go_100ms = 0;
    	}

    	go_main = false;
    }
    
    return 0;
}

