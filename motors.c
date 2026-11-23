#include "settings.h"
#include "motorsVar.h"


// -------------------------
// Notice
// -------------------------
// for CL86T need swap encoder pair lines 
// -------------------------
// 86HB250-156B MOTOR		CL86T 4.1
// -------------------------
// EA+						EB+			
// EA-						EB-
// EB+						EA+
// EB-						EA-
// -------------------------

void Stepper_init() {
	TCD0_init_stepper_PWM(25600, 50); // 51.2kHz, 50% duty for one motor shaft spin, so ~20s for gearbox

}

bool Read_Stepper_PEND(){ // true if position reached
	return !(PORTF.IN & PIN5_bm);
}

bool Read_Stepper_ALM(){ // true if driver has a error (overheat, stepepr stuck and so on)
	return !(PORTF.IN & PIN4_bm);
}

void stepper_actions(){
    switch(motor.stepper.action){
        case ENABLING:
        case DISABLING:
            if (motor.stepper.condition != (motor.stepper.action == ENABLING ? DISABLED : ENABLED))//if condition is not the same as action if enabled != enabling if disabled != disabling
                break; //if so condition and action is the same skip further code

            switch(motor.stepper.state) {
                case SET:
                    if (motor.stepper.action == ENABLING)
                        PORTF.OUTCLR = PIN1_bm;
                    else
                        PORTF.OUTSET = PIN1_bm;
                    TCB0_ON(300); //set timer to 300ms according to stepper driver datasheet
                    motor.stepper.state = WAIT;
                break;

                case WAIT:
                    if (!TCB0_Checker()) { // after wait 
                        TCB0_OFF();
                        motor.stepper.state = DONE;
                    }
                break;

                case DONE:
                    motor.stepper.state = SET; //reseting state for further actions
                    motor.stepper.condition = (motor.stepper.action == ENABLING) ? ENABLED : DISABLED; //set motor condition
                break;
            }
        break;
        
        case RUN:
        case STOPING:
            if (motor.stepper.condition != (motor.stepper.action == RUN ? STOPED : RUNING)) // same sasme as before
                break;

            switch(motor.stepper.state) {
                case SET:
                    if (TCD0.STATUS & TCD_ENRDY_bm) {
                        if (motor.stepper.action == RUN)
                            TCD0.CTRLA |= TCD_ENABLE_bm;
                        else
                            TCD0.CTRLA &= ~TCD_ENABLE_bm;

                        motor.stepper.state = DONE;
                    }
                break;
                
                case WAIT: //not used but for compilator to be happy
                break;

                case DONE:
                    motor.stepper.state = SET;
                    motor.stepper.condition = (motor.stepper.action == RUN) ? RUNING : STOPED;
                break;
            }
        break;   

        case SET_DIRECTION:
            if(motor.stepper.direction != motor.stepper.lastDirection){ //only if new direction is not the same as before
                switch(motor.stepper.state){
                    case SET:
                        if(motor.stepper.direction == FORWARD){
                            PORTF.OUTSET = PIN3_bm;
                            motor.stepper.lastDirection = FORWARD;
                        }
                        else{
                            PORTF.OUTCLR = PIN3_bm;
                            motor.stepper.lastDirection = BACKWARD;
                        }
                        TCB2_ON(3);//set timer to wait  5us
                        motor.stepper.state = WAIT;
                    break;
                    
                    case WAIT:
                        if(!TCB2_Checker()){//return false if time pass
                            motor.stepper.state = DONE;
                            TCB2_OFF();
                        }
                    break; 
                    
                    case DONE:
                        motor.stepper.state = SET;
                    break;   
                }
            }           
        break;
    } 
}
