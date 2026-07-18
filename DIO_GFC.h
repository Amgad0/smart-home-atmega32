/*
 * DIO_GFC.h
 *
 * Created: 10/8/2021 10:42:06 PM
 *  Author: user
 */ 


#ifndef DIO_GFC_H_
#define DIO_GFC_H_

#include "DIO.h"
/* keypad Macros */
#define KEYPAD_PORT			(uint8)'C'
#define KEYPAD_FIRST_PIN	(uint8)0
#define KEYPAD_SECOND_PIN	(uint8)1
#define KEYPAD_THIRD_PIN	(uint8)2
#define KEYPAD_FOURTH_PIN	(uint8)3
#define KEYPAD_FIFTH_PIN	(uint8)4
#define KEYPAD_SIXTH_PIN	(uint8)5
#define KEYPAD_SEVENTH_PIN	(uint8)6
#define KEYPAD_EIGHTH_PIN	(uint8)7

typedef struct{
	DIO_DirTypes PinDir;
	STD_LevelTypes Level;
	}DIO_PinCFG;

#define PINCOUNT 32


void DIO_Init(void);


#endif /* DIO_GFC_H_ */