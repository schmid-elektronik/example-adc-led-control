/*
 * adc.h
 *
 *  Created on: 08.07.2021
 *      Author: rb
 */

#ifndef ADC_H_
#define ADC_H_

#include "chip.h"


void initADC();
bool readADC();
uint16_t getADCVoltage();


#endif /* ADC_H_ */
