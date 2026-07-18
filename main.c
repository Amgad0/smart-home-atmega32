/*
* main.c
*
* Smart Home access-control & appliance-control system (ATmega32).
*
* Original author : amgad bassam (2021)
* Refactored into modules (bsp / auth / session / climate / remote) for
* readability; application behavior is unchanged.
*
* main() is now just the orchestration loop: bring up the board, provision or
* restore login state, then repeatedly service climate/fan, session timeout,
* login, and remote commands.
*/
#define F_CPU 16000000UL
#include <util/delay.h>
#include "main_config.h"
#include "bsp.h"
#include "auth.h"
#include "session.h"
#include "climate.h"
#include "remote.h"
#include "LCD.h"
#include "Keypad.h"

static void Display_Welcome(void)
{
	LCD_String("welcome to smart");
	LCD_movecursor(2, 1);
	LCD_String("home system");
	_delay_ms(1000);
	LCD_Cmd(0X01);
}

int main(void)
{
	BSP_Init();
	Display_Welcome();
	Auth_Begin();

	uint8 login_mode = NO_MODE;
	Uint8 data = 0;

	while (1)
	{
		Climate_Update();
		Fan_Update();

		if (Session_TimedOut() == TRUE)
		{
			Session_Stop();
			Session_Reset();
			Session_ClearTimeout();
			login_mode = NO_MODE;
			LCD_Cmd(0x01);
			LCD_String("session time out");
			_delay_ms(1000);
		}

		if (login_mode == NO_MODE)
		{
			login_mode = Auth_Login();
		}

		Remote_Dispatch(login_mode);

		/* Echo any keypad press to the LCD. */
		data = PrintKey();
		if (data != 0)
		{
			LCD_Char(data);
			_delay_ms(200);
		}
	}
}