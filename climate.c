/*
 * climate.c
 *
 * Environmental control. See climate.h.
 */

#include "climate.h"
#include "main_config.h"
#include "ADC.h"
#include "PWM_Servo.h"
#include "Bitmath.h"
#include <avr/io.h>

void Climate_Update(void)
{
	unsigned short temperature = ADC_Read(TEMP_SENSOR_CHANNEL);
	temperature = temperature / ADC_SCALE_DIVISOR;

	if (temperature >= TEMP_AC_ON)
	{
		SetBit(PORTB, AC_RELAY1_BIT);
		SetBit(PORTB, AC_RELAY2_BIT);
		ClearBit(PORTB, AC_FAN_BIT);
	}
	else if (temperature <= TEMP_AC_OFF)
	{
		ClearBit(PORTB, AC_RELAY2_BIT);
	}
}

void Fan_Update(void)
{
	int pot_val = ADC_Read(POT_CHANNEL);
	pot_val = pot_val / ADC_SCALE_DIVISOR;
	PWM_duty_Timer2(pot_val);
}