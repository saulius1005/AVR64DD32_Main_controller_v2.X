#include "settings.h"
#include "trackerVar.h"

bool below_tolerance(){
    return (tracker.CurrentAZ + POSITION_TOLERANCE < tracker.TargetAZ);//reached lower position of allowed target
}

bool above_tolerance(){
    return (tracker.CurrentAZ - POSITION_TOLERANCE > tracker.TargetAZ); //reached top position of allowed target
}

bool within_tolerance(){
    return below_tolerance() || above_tolerance();
}

bool below_wakeup_threshold(){
    return (tracker.CurrentAZ + WAKEUP_THRESHOLD < tracker.TargetAZ);//reached lower position need to move forward
}

bool above_wakeup_threshold(){
    return (tracker.CurrentAZ - WAKEUP_THRESHOLD > tracker.TargetAZ); //reached top position need to move backward
}

bool should_wake_up(){    
    return below_wakeup_threshold() || above_wakeup_threshold(); //target 40, BACKLASH 1, current 38, 39, 40, 41, 42: true, false, false, false, true
}


void auto_tracker(){
    switch(tracker.State){
        case T_ENABLE:
            stepper.callbacks.enable();//if already enabled it will be skiped
            tracker.State = T_DIR_PREP;
        break;
        case T_DIR_PREP:
            if (below_wakeup_threshold())
                stepper.callbacks.dir_forward(); //same. skip if not changed
            else if (above_wakeup_threshold())
                stepper.callbacks.dir_backward();
            tracker.State = T_SPIN;
        break;
        case T_SPIN:
            stepper.callbacks.run(); //spin little
            tracker.State = T_STOP;
        break;
        case T_STOP:
            stepper.callbacks.stop(); //stop after spin
            
            // Read current position //not reading new targed //read only top controller
            //if out of range stop
            //if errors stop
            
            if (within_tolerance()) //if target is reached +- tolerance
                tracker.State = T_DISABLE;
            else
                tracker.State = T_DIR_PREP; //if not change direction accordingly
        break;
        case T_SLEEP: //in sleep checking 
            
            // Read target position //weather station read //new target
            // Read current position // top controller read
            
            //errors, range controll
            
            //show data //easy ssd1306
            //check inputs //joystick button :D none for now. but exmpl set flag and continue with it only after Tracker reach sleep
            
            if(should_wake_up()) //checking if current position +- wakeup range
                tracker.State = T_ENABLE; //if above or below enable driver
        break;
        case T_DISABLE:
            stepper.callbacks.disable(); //disable driver
            tracker.State = T_SLEEP; //go back to checking target and current positions
        break;
    }      
}
