/*
 * ADC.h
 *
 * Created: 10/22/2021 9:23:55 PM
 *  Author: amgad
 */


#ifndef ADC_H_
#define ADC_H_

#include "LCD.h"

void ADC_Init();
unsigned short ADC_Read(unsigned char channel);



#endif /* ADC_H_ */