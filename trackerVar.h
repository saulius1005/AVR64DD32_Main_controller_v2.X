/* 
 * File:   trackerVar.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, lapkritis 27, 19.19
 */

#ifndef TRACKERVAR_H
#define	TRACKERVAR_H

#include "tracker.h"

#ifdef	__cplusplus
extern "C" {
#endif

tracking_t tracker = {
    .CurrentAZ = &sensor_data.AZ,
    .CurrentEL = &sensor_data.EL,
    .TargetAZ = &ws_data.AZ,
    .TargetEL = &ws_data.EL
};


#ifdef	__cplusplus
}
#endif

#endif	/* TRACKERVAR_H */

