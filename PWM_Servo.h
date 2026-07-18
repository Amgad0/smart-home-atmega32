/*
 * PWM_Servo.h
 *
 * Created: 12/23/2021 3:32:41 PM
 *  Author: Amgad Bassam
 */


#ifndef PWM_SERVO_H_
#define PWM_SERVO_H_

#include <avr/io.h>

/*
	Function Name        : Timer2_PhasePWM
	Function Returns     : void
	Function Arguments   : void
	Function Description : Configure Timer2 for phase-correct PWM, non-inverting, to drive the fan/appliance output.
*/
void Timer2_PhasePWM(void);

/*
	Function Name        : PWM_duty_Timer2
	Function Returns     : void
	Function Arguments   : int duty
	Function Description : Set Timer2's OCR2 duty cycle, driving the fan/appliance PWM output.
*/
void PWM_duty_Timer2(int duty);

/*
	Function Name        : Timer1_ServoPWM
	Function Returns     : void
	Function Arguments   : void
	Function Description : Configure Timer1 for Fast PWM, non-inverting, to drive the door-lock servo. Pin direction is owned by the DIO_GFC PinCFG table, not this function.
*/
void Timer1_ServoPWM(void);

#define MinCount 250
#define MaxCount 500
#define MinAngle -90
#define MaxAngle 90

/*
	Function Name        : Servo
	Function Returns     : void
	Function Arguments   : long angle
	Function Description : Map an angle in [MinAngle, MaxAngle] to the Timer1 OCR1A pulse width driving the door-lock servo.
*/
void Servo(long angle);




#endif /* PWM_SERVO_H_ */