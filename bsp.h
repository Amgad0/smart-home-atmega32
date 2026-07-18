/*
 * bsp.h
 *
 * Board support: a single BSP_Init() that brings up every peripheral the
 * application needs (global interrupts, servo/appliance PWM timers, UART, ADC,
 * the DIO/LCD/keypad drivers) and configures the direction bits that main.c and
 * PWM_Servo.c previously poked ad hoc.
 */


#ifndef BSP_H_
#define BSP_H_

/* Initialize the MCU and all peripherals. Call once at the top of main(). */
void BSP_Init(void);

#endif /* BSP_H_ */