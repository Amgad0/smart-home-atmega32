/*
 * PWM_Servo.h
 *
 * Created: 12/23/2021 3:32:41 PM
 *  Author: Wafeek
 */


#ifndef PWM_SERVO_H_
#define PWM_SERVO_H_

#include <avr/io.h>

void Timer2_PhasePWM(void);
void PWM_duty_Timer2(int duty);
void Timer1_ServoPWM(void);

#define MinCount 250
#define MaxCount 500
#define MinAngle -90
#define MaxAngle 90

void Servo(long angle);




#endif /* PWM_SERVO_H_ */