#include "settings.h"
#include "trackerVar.h"

bool too_low(){
    return (tracker.CurrentAZ + BACKLASH < tracker.TargetAZ);//reached lower position need to move forward
}

bool too_high(){
    return (tracker.CurrentAZ - BACKLASH > tracker.TargetAZ); //reached top position need to move backward
}

bool wake_up(){    
    return too_low() || too_high(); //target 40, BACKLASH 1, current 38, 39, 40, 41, 42: true, false, false, false, true
}


void auto_tracker(){
// Read target position
// Read current position
    
if(wake_up())//if current_position +- BACKLASH is not equal to target_position
    tracker.State = T_ENABLE;
else
    tracker.State = T_SLEEP;

switch(tracker.State){
    case T_ENABLE:
        stepper.callbacks.enable();
        tracker.State = T_DIR_PREP;
    break;
    case T_DIR_PREP:
        if (too_low()) {
            stepper.callbacks.dir_backward();
            // Spin forward until:
            // current_position >= target_position - NEAR_ERROR
        }
        else if (too_high()) {
            stepper.callbacks.dir_backward();
            // Spin backward until:
            // current_position <= target_position + NEAR_ERROR
        }        
    break;
}
    
}
