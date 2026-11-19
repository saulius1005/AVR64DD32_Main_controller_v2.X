/* 
 * File:   timerVar.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, lapkritis 19, 20.54
 */

#ifndef TIMERVAR_H
#define	TIMERVAR_H

#include "timers.h"


#ifdef	__cplusplus
extern "C" {
#endif

timer_t timer = {
    .top_controller_respond_time = 500      
};


#ifdef	__cplusplus
}
#endif

#endif	/* TIMERVAR_H */

