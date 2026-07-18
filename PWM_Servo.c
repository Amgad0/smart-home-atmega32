/*
 * PWM_Servo.c
 *
 * Created: 12/23/2021 3:32:27 PM
 *  Author: Wafeek
 */
#include "PWM_Servo.h"

void Timer1_ServoPWM(void)
{
	/* Pin direction (OC1A / PD5 as output) is owned by the DIO_GFC PinCFG table
	 * and applied by DIO_Init(); this function configures only the timer. */
	TCCR1A = (1<<COM1A1)|(1<<WGM11);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS11)|(1<<CS10);
	/* ICR1 (Fast-PWM top) is set once in BSP_Init() as SERVO_ICR1_TOP. */
}

void Servo(long angle)
{
	OCR1A = (((angle-MinAngle)*(MaxCount-MinCount))/(MaxAngle-MinAngle))+MinCount-1;
}

void Timer2_PhasePWM(void)
{
	/* Pin direction (OC2 / PD7 as output) is owned by the DIO_GFC PinCFG table
	 * and applied by DIO_Init(); this function configures only the timer. */
	TCCR2 |= (1<<WGM20)|(1<<COM21)|(1<<CS21);
}

void PWM_duty_Timer2(int duty)
{
	OCR2 = (duty);
}