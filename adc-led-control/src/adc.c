/*
 * adc.c
 *
 *  Created on: 08.07.2021
 *      Author: rb
 */

#include "adc.h"
#include "logger.h"
#include "ring_buffer.h"



uint32_t adc_value = 0;

void initADC()
{
	static ADC_CLOCK_SETUP_T ADCSetup;

	LOG_INFO("init ADC");

	// URef_2.5V
	Chip_IOCON_PinMuxSet(LPC_IOCON, IOCON_PIO1_0, IOCON_FUNC2);


	/* ADC Init */
	Chip_ADC_Init(LPC_ADC, &ADCSetup);
	Chip_ADC_EnableChannel(LPC_ADC, ADC_CH1, ENABLE);
}

bool readADC()
{
	static Status status;
	static uint16_t dataADC;

	/* Start A/D conversion */
	Chip_ADC_SetStartMode(LPC_ADC, ADC_START_NOW, ADC_TRIGGERMODE_RISING);

	/* Waiting for A/D conversion complete */
	while (Chip_ADC_ReadStatus(LPC_ADC, ADC_CH1, ADC_DR_DONE_STAT) != SET) {}

	/* Read ADC value */
	status = Chip_ADC_ReadValue(LPC_ADC, ADC_CH1, &dataADC);
	if (status == SUCCESS) {
		adc_value = dataADC;
	}

	return (status == SUCCESS);
}

// precondition: call readADC() for physical device read
uint16_t getADCVoltage()
{
	uint16_t voltage = 0;

	// 2500 correspond to URef_2.5V (measured)
	voltage = 3300 / 0x03FF * adc_value;

	return voltage;
}


