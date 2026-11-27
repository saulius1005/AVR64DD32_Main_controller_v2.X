/* 
 * File:   tracker.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, lapkritis 27, 19.19
 */

#ifndef TRACKER_H
#define	TRACKER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define POSITION_TOLERANCE 20 // 0.20 degree near to target is "good enough"
#define WAKEUP_THRESHOLD 100 // 1.00 degree. Change angle if current angle is above this

typedef enum {    
    T_SLEEP = 0,
    T_ENABLE,
    T_DIR_PREP,
    T_SPIN,
    T_STOP,
    T_DISABLE
}tracking_states_t;    
    
    
typedef struct{
    tracking_states_t State;
    uint16_t *CurrentAZ;
    int16_t *CurrentEL;
    uint16_t *TargetAZ;
    int16_t *TargetEL;
}tracking_t;

extern tracking_t tracker;


#ifdef	__cplusplus
}
#endif

#endif	/* TRACKER_H */

