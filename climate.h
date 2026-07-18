/*
 * climate.h
 *
 * Environmental control: temperature-driven AC relay switching and
 * potentiometer-driven fan/dimmer PWM. Both are polled once per main-loop pass.
 */


#ifndef CLIMATE_H_
#define CLIMATE_H_

/*
	Function Name        : Climate_Update
	Function Returns     : void
	Function Arguments   : void
	Function Description : Read the temperature sensor and switch the AC relay outputs: on at/above TEMP_AC_ON, off at/below TEMP_AC_OFF (hysteresis in between).
*/
void Climate_Update(void);

/*
	Function Name        : Fan_Update
	Function Returns     : void
	Function Arguments   : void
	Function Description : Read the potentiometer and drive the Timer2 PWM appliance/fan duty cycle.
*/
void Fan_Update(void);

#endif /* CLIMATE_H_ */