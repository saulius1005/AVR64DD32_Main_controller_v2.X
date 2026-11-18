/* 
 * File:   Joystick.h
 * Author: Saulius
 *
 * Created on Tre?iadienis, 2026, lapkritis 18, 22.55
 */

#ifndef JOYSTICK_H
#define	JOYSTICK_H

#ifdef	__cplusplus
extern "C" {
#endif

#define Joystick_steps 819
    
typedef struct {
	int16_t X_Axis;
	int16_t Y_Axis;
	bool Button;
} ADCJoy_t;

extern ADCJoy_t Joystick; //Joke for nerds "ADC JOY"


#ifdef	__cplusplus
}
#endif

#endif	/* JOYSTICK_H */

