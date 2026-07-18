/*
 * auth.c
 *
 * Access control: provisioning, PIN entry, login and lockout. See auth.h.
 *
 * NOTE (behavior-preserving refactor): the Admin and Guest login paths are kept
 * as separate helpers on purpose. Their on-screen sequences differ in the
 * original firmware (upper- vs lower-case prompts and a differing number of
 * display-clear commands); merging them would change what the LCD shows, so the
 * duplication is deliberate. Only the identical masked PIN-entry loop, which was
 * copy-pasted four times, has been de-duplicated into Auth_ReadPin().
 */

#define F_CPU 16000000UL
#include "auth.h"
#include "main_config.h"
#include "LCD.h"
#include "Keypad.h"
#include "EEPROM.h"
#include "session.h"
#include <util/delay.h>

/* Persisted-across-attempts lockout flag; restored from EEPROM in Auth_Begin(). */
static uint8 block_mode_flag = FALSE;

uint8 Auth_ComparePin(uint8* pin1, uint8* pin2, uint8 size)
{
	uint8 i = 0;
	uint8 ret_value = TRUE;
	while (i < size)
	{
		if (pin1[i] != pin2[i])
		{
			ret_value = FALSE;
			break;
		}
		i++;
	}
	return ret_value;
}

void Auth_ReadPin(uint8* out_pin)
{
	uint8 pass_counter = 0;
	while (pass_counter < PASS_SIZE)
	{
		uint8 key_pressed = notpressed;
		while (key_pressed == notpressed)
		{
			key_pressed = keypad_checkpress();
		}
		out_pin[pass_counter] = key_pressed;
		LCD_Char(key_pressed);
		_delay_ms(CHARACTER_PREVIEW_TIME);
		LCD_movecursor(2, 12 + pass_counter);
		LCD_Char(PASSWORD_SYMBOL);
		_delay_ms(100);
		pass_counter++;
	}
}

uint8 Auth_IsProvisioned(void)
{
	return (EEPROM_ui8ReadByteFromAddress(ADMIN_PASS_STATUS_ADDRESS) == PASS_SET)
		&& (EEPROM_ui8ReadByteFromAddress(GUEST_PASS_STATUS_ADDRESS) == PASS_SET);
}

void Auth_Provision(void)
{
	uint8 pass[PASS_SIZE] = { NOT_STORED, NOT_STORED, NOT_STORED, NOT_STORED };

	LCD_String("LOGIN FOR");
	LCD_movecursor(2, 1);
	LCD_String("FIRST TIME");
	_delay_ms(1000);
	LCD_Cmd(0X01);

	/* Admin PIN */
	LCD_String("SET ADMIN PASS");
	LCD_movecursor(2, 1);
	LCD_String("ADMIN PASS");
	Auth_ReadPin(pass);
	EEPROM_vWriteBlockToAddress(EEPROM_ADMIN_ADDRESS, pass, PASS_SIZE);
	/* NOTE: original stamps the GUEST status address here, not the ADMIN one.
	 * Preserved verbatim as part of the behavior-preserving refactor. */
	EEPROM_vWriteByteToAddress(GUEST_PASS_STATUS_ADDRESS, PASS_SET);
	LCD_Cmd(0X01);
	_delay_ms(500);
	LCD_Cmd(0x01);

	/* Guest PIN */
	LCD_String("Set Guest Pass");
	LCD_movecursor(2, 1);
	LCD_String("Guest Pass:");
	Auth_ReadPin(pass);
	EEPROM_vWriteBlockToAddress(EEPROM_GUEST_ADDRESS, pass, PASS_SIZE);
	EEPROM_vWriteByteToAddress(GUEST_PASS_STATUS_ADDRESS, PASS_SET);
	LCD_Cmd(0x01);
	LCD_String("Pass Saved");
	_delay_ms(500);
	LCD_Cmd(0x01);
}

void Auth_Begin(void)
{
	if (Auth_IsProvisioned())
	{
		block_mode_flag = EEPROM_ui8ReadByteFromAddress(LOGIN_BLOCKED_ADDRESS);
	}
	else
	{
		Auth_Provision();
	}
}

/* Admin unlock loop: re-prompt for the PIN until correct or locked out.
 * Returns ADMIN on success, NO_MODE on lockout. */
static uint8 Auth_UnlockAdmin(uint8* pass_tries)
{
	uint8 login_mode = NO_MODE;
	uint8 pass[PASS_SIZE] = { NOT_STORED, NOT_STORED, NOT_STORED, NOT_STORED };
	uint8 stored_pass[PASS_SIZE] = { NOT_STORED, NOT_STORED, NOT_STORED, NOT_STORED };

	while (login_mode != ADMIN)
	{
		LCD_Cmd(0x01);
		LCD_String("admin mode");
		LCD_movecursor(2, 1);
		LCD_String("enter pass");
		_delay_ms(200);

		Auth_ReadPin(pass);
		EEPROM_vReadBlockFromAddress(EEPROM_ADMIN_ADDRESS, stored_pass, PASS_SIZE);

		if (Auth_ComparePin(pass, stored_pass, PASS_SIZE) == TRUE)
		{
			login_mode = ADMIN;
			*pass_tries = 0;
			LCD_Cmd(0X01);
			LCD_String("RIGHT PASS");
			LCD_movecursor(2, 1);
			LCD_String("ADMIN MODE");
			_delay_ms(500);
			LCD_Cmd(0X01);
			Session_Start();
			LCD_Cmd(0X01);
		}
		else
		{
			(*pass_tries)++;
			LCD_Cmd(0X01);
			LCD_String("WRONG PASS");
			LCD_movecursor(2, 1);
			LCD_String("TRIES LEFT");
			LCD_Char(TRIES_ALLOWED - *pass_tries + ASCII_ZERO);
			_delay_ms(1000);
			LCD_Cmd(0x01);
			if (*pass_tries >= TRIES_ALLOWED)
			{
				EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS, TRUE);
				block_mode_flag = TRUE;
				break;
			}
		}
	}
	return login_mode;
}

/* Guest unlock loop: re-prompt for the PIN until correct or locked out.
 * Returns GUEST on success, NO_MODE on lockout. */
static uint8 Auth_UnlockGuest(uint8* pass_tries)
{
	uint8 login_mode = NO_MODE;
	uint8 pass[PASS_SIZE] = { NOT_STORED, NOT_STORED, NOT_STORED, NOT_STORED };
	uint8 stored_pass[PASS_SIZE] = { NOT_STORED, NOT_STORED, NOT_STORED, NOT_STORED };

	while (login_mode != GUEST)
	{
		LCD_Cmd(0x01);
		LCD_String("guest mode");
		LCD_movecursor(2, 1);
		LCD_String("enter pass");
		_delay_ms(200);

		Auth_ReadPin(pass);
		EEPROM_vReadBlockFromAddress(EEPROM_GUEST_ADDRESS, stored_pass, PASS_SIZE);

		if (Auth_ComparePin(pass, stored_pass, PASS_SIZE) == TRUE)
		{
			login_mode = GUEST;
			*pass_tries = 0;
			LCD_Cmd(0x01);
			LCD_String("right pass");
			LCD_movecursor(2, 1);
			LCD_String("guest mode");
			_delay_ms(500);
			Session_Start();
			LCD_Cmd(0x01);
		}
		else
		{
			(*pass_tries)++;
			LCD_Cmd(0x01);
			LCD_String("wrong pass");
			LCD_movecursor(2, 1);
			LCD_String("tries left");
			LCD_Char(TRIES_ALLOWED - *pass_tries + ASCII_ZERO);
			_delay_ms(1000);
			if (*pass_tries >= TRIES_ALLOWED)
			{
				EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS, TRUE);
				block_mode_flag = TRUE;
				break;
			}
		}
	}
	return login_mode;
}

uint8 Auth_Login(void)
{
	uint8 login_mode = NO_MODE;
	uint8 pass_tries = 0;

	while (login_mode == NO_MODE)
	{
		if (block_mode_flag == TRUE)
		{
			LCD_Cmd(0x01);
			LCD_String("login blocked");
			LCD_movecursor(2, 1);
			LCD_String("wait 20 second");
			_delay_ms(BLOCK_MODE_TIME);
			pass_tries = 0;
			block_mode_flag = FALSE;
			EEPROM_vWriteByteToAddress(LOGIN_BLOCKED_ADDRESS, FALSE);
		}

		LCD_Cmd(0x01);
		LCD_String("select mode:");
		LCD_movecursor(2, 1);
		LCD_String("0:ADMIN 1:USER");

		uint8 key_pressed = notpressed;
		while (key_pressed == notpressed)
		{
			key_pressed = keypad_checkpress();
		}

		if (key_pressed != CHECK_ADMIN_MODE && key_pressed != CHECK_GUEST_MODE)
		{
			LCD_Cmd(0x01);
			LCD_String("wrong input");
			_delay_ms(1000);
			continue;
		}

		if (key_pressed == CHECK_ADMIN_MODE)
		{
			login_mode = Auth_UnlockAdmin(&pass_tries);
		}
		else
		{
			login_mode = Auth_UnlockGuest(&pass_tries);
		}
	}

	return login_mode;
}