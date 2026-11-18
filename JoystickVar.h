/* 
 * File:   JoystickVar.h
 * Author: Saulius
 *
 * Created on Tre?iadienis, 2026, lapkritis 18, 22.55
 */

#ifndef JOYSTICKVAR_H
#define	JOYSTICKVAR_H

#include "Joystick.h"


#ifdef	__cplusplus
extern "C" {
#endif

ADCJoy_t Joystick = {
    .X_Axis = 0,
    .Y_Axis = 0,
};


#ifdef	__cplusplus
}
#endif

#endif	/* JOYSTICKVAR_H */

