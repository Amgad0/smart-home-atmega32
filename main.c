/*
* Keypad2_Project.c
*
* Created: 12/21/2021 2:50:26 AM
* Author : youssef rashad
*/
#define F_CPU 16000000UL
#include <avr/io.h>
#include "LCD.h"
#include "DIO.h"
#include "Keypad.h"
#include "timer_driver.h"
#include "EEPROM.h"
#include "PWM_Servo.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "ADC.h"
#include "UART.h"
#include "main_config.h"
volatile unsigned char UartV=0;
uint8 timeout_flag = FALSE;
volatile uint16 session_counter = 0;
#include <avr/interrupt.h>
int main(void)
{
	sei();
	Timer1_ServoPWM();
	Timer2_PhasePWM();
	UART_Init();
	Servo(0);
	DIO_Init();
	LCD_Init();
	keypad_init();
	Uint8 data=0;
	//_________motorPWM__________//
	DDRD |= (1<<5);
	DDRB |= (1<<5)|(1<<6)|(1<<7);
	/*
	TCCR1A = (1<<COM1A1)|(1<<WGM11);
	TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS11)|(1<<CS10);//divide CLK by 64 - FastPWM - Non Inverting
	ICR1 = 1000;
	*/
	//_________ADC__________//
	ADC_Init();
	//Timer1_ServoPWM();
	//Servo(0);
	ICR1 = 5000;
	//________LAMPS___________//
	DDRD |=(1<<2);
	DDRD |=(1<<3);



	unsigned short Temperature = 0;
	int Pot_Val = 0;
	DDRA |= (1<<1);
	char array[10];


	uint8 login_mode = UartV;
	uint8 block_mode_flag = FALSE;
	uint8 key_pressed = notpressed;
	LCD_String("welcome to smart");
	LCD_movecursor(2,1);
	LCD_String("home system");
	_delay_ms(1000);
	LCD_Cmd(0X01);
	if ((EEPROM_ui8ReadByteFromAddress(ADMIN_PASS_STATUS_ADDRESS)!=PASS_SET)||(EEPROM_ui8ReadByteFromAddress(GUEST_PASS_STATUS_ADDRESS)!=PASS_SET))

	{
		LCD_String("LOGIN FOR");
		LCD_movecursor(2,1);
		LCD_String("FIRST TIME");
		_delay_ms(1000);
		LCD_Cmd(0X01);
		/*ADMIN*/
		LCD_String("SET ADMIN PASS");
		LCD_movecursor(2,1);
		LCD_String("ADMIN PASS");
		/*SET ADMIN PASS*/
		Uint8 PASS_COUNTER=0;
		Uint8 PASS[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};
		while (PASS_COUNTER<PASS_SIZE)
		{
			key_pressed =notpressed;
			while(key_pressed==notpressed){
				key_pressed = keypad_checkpress();
			}
			PASS[PASS_COUNTER]=key_pressed;
			LCD_Char(key_pressed);
			_delay_ms(CHARACTER_PREVIEW_TIME);
			LCD_movecursor(2,12+PASS_COUNTER);
			LCD_Char(PASSWORD_SYMBOL);
			_delay_ms(100);
			PASS_COUNTER++;
		}

		EEPROM_vWriteBlockToAddress(EEPROM_ADMIN_ADDRESS,PASS,PASS_SIZE);
		EEPROM_vWriteByteToAddress(GUEST_PASS_STATUS_ADDRESS,PASS_SET);
		LCD_Cmd(0X01);
		_delay_ms(500);
		LCD_Cmd(0x01);




		PASS_COUNTER=0;
		LCD_String("Set Guest Pass");
		LCD_movecursor(2,1);
		LCD_String("Guest Pass:");
		while (PASS_COUNTER<PASS_SIZE)//loop till the user finish inserting the pass
		{
			key_pressed = notpressed;//return the variable that holds the pressed key from keypad to its initial value
			while (key_pressed == notpressed)//repeat till the user press any key
			{
				key_pressed = keypad_checkpress();//if the user pressed any button in keypad save the value in key_pressed
			}

			PASS[PASS_COUNTER]=key_pressed;//add the entered character to the pass array
			LCD_Char(key_pressed);//print the entered character
			_delay_ms(CHARACTER_PREVIEW_TIME);//Halt the system for the given time in (ms)
			LCD_movecursor(2,12+PASS_COUNTER);//move the lcd cursor to the previous location to write the password symbol over the character
			LCD_Char(PASSWORD_SYMBOL); // to display (Password sign *)
			_delay_ms(100);//Halt the system for the given time in (ms)
			PASS_COUNTER++;//increase the characters count
		}
		EEPROM_vWriteBlockToAddress(EEPROM_GUEST_ADDRESS,PASS,PASS_SIZE);//save the entire password as a block to the EEPROM
		EEPROM_vWriteByteToAddress(GUEST_PASS_STATUS_ADDRESS,PASS_SET);//write the status of pass as it is set
		LCD_Cmd(0x01);//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		LCD_String("Pass Saved");//move the lcd cursor to the previous location
		_delay_ms(500);//Halt the system for the given time in (ms)
		LCD_Cmd(0x01);//remove all previously printed characters on the LCD and move the cursor to the first column of the first row
		//EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,FALSE);



















	}
	else{
		block_mode_flag=EEPROM_ui8ReadByteFromAddress(LOGIN_BLOCKED_ADDRESS);


	}


	while (1)
	{
		Temperature = ADC_Read(0);
		Temperature = Temperature/4;


		if (Temperature >= 28)
		{

			PORTB |= (1<<6)|(1<<7);
			PORTB &=~ (1<<5);
			//LCD_String("AC ON");
			//_delay_ms(200);
			//LCD_Cmd(0x01);
		}
		else if (Temperature <= 21)
		{
			PORTB &=~ (1<<7);
		}

		Pot_Val = ADC_Read(2);
		Pot_Val = Pot_Val/4;
		PWM_duty_Timer2(Pot_Val);








		key_pressed=notpressed;
		uint8 pass_tries=0;
		if (timeout_flag==TRUE)
		{
			timer0_stop();
			session_counter=0;
			timeout_flag=FALSE;
			login_mode=NO_MODE;
			key_pressed=notpressed;
			LCD_Cmd(0x01);
			LCD_String("session time out");
			_delay_ms(1000);
		}
		while (login_mode==NO_MODE)
		{
			if (block_mode_flag==TRUE)
			{
				LCD_Cmd(0x01);
				LCD_String("login blocked");
				LCD_movecursor(2,1);
				LCD_String("wait 20 second");
				_delay_ms(BLOCK_MODE_TIME);
				pass_tries=0;
				block_mode_flag=FALSE;
				EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,FALSE);
			}
			LCD_Cmd(0x01);
			LCD_String("select mode:");
			LCD_movecursor(2,1);
			LCD_String("0:ADMIN 1:USER");
			while(key_pressed==notpressed){
				key_pressed=keypad_checkpress();


			}
			if (key_pressed!=CHECK_ADMIN_MODE && key_pressed!=CHECK_GUEST_MODE)
			{
				LCD_Cmd(0x01);
				LCD_String("wrong input");
				key_pressed=notpressed;
				_delay_ms(1000);
				continue;

			}
			uint8 pass_counter=0;
			uint8 pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};
			uint8 stored_pass[PASS_SIZE]={NOT_STORED,NOT_STORED,NOT_STORED,NOT_STORED};
			switch(key_pressed)
			{

				case CHECK_ADMIN_MODE:
				while(login_mode!=ADMIN){
					key_pressed=notpressed;
					LCD_Cmd(0x01);
					LCD_String("admin mode");
					LCD_movecursor(2,1);
					LCD_String("enter pass");
					_delay_ms(200);
					pass_counter=0;
					while(pass_counter<PASS_SIZE){



						while(key_pressed==notpressed)
						{
							key_pressed=keypad_checkpress();
						}

						pass[pass_counter]=key_pressed;

						LCD_Char(key_pressed);
						_delay_ms(CHARACTER_PREVIEW_TIME);
						LCD_movecursor(2,12+pass_counter);
						LCD_Char(PASSWORD_SYMBOL);
						_delay_ms(100);
						pass_counter++;
						key_pressed=notpressed;
					}
					EEPROM_vReadBlockFromAddress(EEPROM_ADMIN_ADDRESS,stored_pass,PASS_SIZE);
					if (ui8ComparePass(pass,stored_pass,PASS_SIZE)==TRUE)///////////////////////
					{
						login_mode=ADMIN;
						pass_tries=0;
						LCD_Cmd(0X01);
						LCD_String("RIGHT PASS");
						LCD_movecursor(2,1);
						LCD_String("ADMIN MODE");
						_delay_ms(500);
						LCD_Cmd(0X01);

						timer0_initializeCTC();
						LCD_Cmd(0X01);
						///////////////////


					}
					else{
						pass_tries++;
						login_mode=NO_MODE;
						LCD_Cmd(0X01);
						LCD_String("WRONG PASS");
						LCD_movecursor(2,1);
						LCD_String("TRIES LEFT");
						LCD_Char(TRIES_ALLOWED-pass_tries+ASCII_ZERO);
						_delay_ms(1000);
						LCD_Cmd(0x01);
						if (pass_tries>=TRIES_ALLOWED)
						{ EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,TRUE);
							block_mode_flag=TRUE;
							break;
						}

					}
				}
				break;
				case CHECK_GUEST_MODE :
				while(login_mode!=GUEST){
					key_pressed=notpressed;
					LCD_Cmd(0x01);
					LCD_String("guest mode");
					LCD_movecursor(2,1);
					LCD_String("enter pass");
					_delay_ms(200);
					pass_counter=0;
					while(pass_counter<PASS_SIZE){

						while(key_pressed==notpressed)
						{
							key_pressed=keypad_checkpress();
						}


						pass[pass_counter]=key_pressed;
						LCD_Char(key_pressed);
						_delay_ms(CHARACTER_PREVIEW_TIME);
						LCD_movecursor(2,12+pass_counter);
						LCD_Char(PASSWORD_SYMBOL);
						_delay_ms(100);
						pass_counter++;
						key_pressed=notpressed;
					}
					EEPROM_vReadBlockFromAddress(EEPROM_GUEST_ADDRESS,stored_pass,PASS_SIZE);
					if (ui8ComparePass(pass,stored_pass,PASS_SIZE)==TRUE)
					{
						login_mode=GUEST;
						pass_tries=0;
						LCD_Cmd(0x01);
						LCD_String("right pass");
						LCD_movecursor(2,1);
						LCD_String("guest mode");
						_delay_ms(500);
						timer0_initializeCTC();
						LCD_Cmd(0x01);
					}
					else{
						pass_tries++;
						login_mode =NO_MODE;
						LCD_Cmd(0x01);
						LCD_String("wrong pass");
						LCD_movecursor(2,1);
						LCD_String("tries left");
						LCD_Char(TRIES_ALLOWED-pass_tries+ASCII_ZERO);
						_delay_ms(1000);
						if (pass_tries>=TRIES_ALLOWED)
						{
							EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS,TRUE);
							block_mode_flag = TRUE;
							break;
						}
					}
				}
				break;
			}


		}////////
		if (login_mode == ADMIN)
		{
			//LCD_String("Enter 1 on mobile to open door");




			if(UartV == '1'){

				Servo(90);

			}
			else if (UartV == '2')
			{
				Servo(0);
			}
			else if(UartV=='3')
			{
				PORTD ^=(1<<2);
				_delay_ms(500);
			}
			else if(UartV=='4')
			{
				PORTD ^=(1<<3);
				_delay_ms(500);
			}

		}
		if(login_mode==GUEST)
		{
			if (UartV=='1')
			{
				PORTD ^=(1<<2);
				_delay_ms(500);
			}
			else if (UartV=='2')
			{
				PORTD ^=(1<<3);
				_delay_ms(500);
			}
		}


		data=PrintKey();


		if(data!=0)
		{
			LCD_Char(data);
			_delay_ms(200);
		}
		//ToggleBit(PORTD_Reg,4);




		//_delay_ms(200);



		/*while((GetBit(PORTA_Reg,1))==0)
		{
		Temperature = ADC_Read(0);
		Temperature = Temperature/4;

		itoa(Temperature,array,10); //change from int to asci and puts it in the array
		LCD_String("Temp=");
		LCD_String(array);
		_delay_ms(200);
		LCD_Cmd(0x01); //clear LCD

		}
		if(UART_Recieve()=='1'){
		PORTD ^= (1<<2);
		}*/

	}
}



ISR(TIMER0_COMP_vect)
{
	session_counter++;
}

ISR(USART_RXC_vect)
{
	UartV=UDR;
}
