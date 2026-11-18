/* 
 * File:   motorsVar.h
 * Author: Saulius
 *
 * Created on Tre?iadienis, 2026, lapkritis 18, 20.24
 */

#ifndef MOTORSVAR_H
#define	MOTORSVAR_H

#include "motors.h"


#ifdef	__cplusplus
extern "C" {
#endif

motor_t motor = {
  .stepper = {
      0
  },  
  .linear = {
      0
  }
};


#ifdef	__cplusplus
}
#endif

#endif	/* MOTORSVAR_H */

