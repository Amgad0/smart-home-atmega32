/*
 * timer_driver.c
 *
 * Created: 3/2/2018 4:44:59 PM
 *  Author: Mohamed Zaghlol
 */
#include "timer_driver.h"
#include "Bitmath.h"
void timer0_initializeCTC(void)
{
	//Config OCR0
	//OCR0 = the value to compare with
	OCR0 = 78; //to achieve tick equal to 10 msec
	
	//Set timer mode
	//Set the mode as Compare match
	SetBit(TCCR0, WGM01);
	ClearBit(TCCR0, WGM00);
	
	//Configure clock
	//set as clk/1024
	SetBit(TCCR0, CS00);
	ClearBit(TCCR0, CS01);
	SetBit(TCCR0, CS02);
	
	//Enable global interrupts
	sei();
	
	//Enable timer 0 interrupt for compare match
	SetBit(TIMSK, OCIE0);
}

void timer0_stop(void)
{
	//disable the clock to stop the counter
	ClearBit(TCCR0, CS00);
	ClearBit(TCCR0, CS01);
	ClearBit(TCCR0, CS02);
}

void timer_initializefastpwm(void)
{
	//Config OCR0
	//OCR0 = the value to compare with
	OCR0 = 128;
	
	//Set timer mode
	//adjust fast pulse width modulation mode
	SetBit(TCCR0, WGM00);
	SetBit(TCCR0, WGM01);
	
	//Enable global interrupts
	sei();
	
	SetBit(TCCR0, COM01); //adjust non inverting mode
	
	//Enable timer0 interrupt for overflow mode
	SetBit(TIMSK,TOIE0);
	
	//Configure clock
	//set as clk/1024
	SetBit(TCCR0, CS00);
	ClearBit(TCCR0, CS01);
	SetBit(TCCR0, CS02);
}
void change_dutycycle(float64 duty)
{
	OCR0 = (duty / 100) * 256;//Set the OCR0 depending on the duty percentage
}
