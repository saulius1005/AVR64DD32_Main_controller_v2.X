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
    if(stepper.init)
        return; //skip if initiated already
	TCD0_init_stepper_PWM(25600, 50); // 51.2kHz, 50% duty for one motor shaft spin, so ~20s for gearbox
    PORTF.OUTSET = PIN1_bm; //keep disabled
    stepper.init = true;
}

void Stepper_Enable(){
    if(!stepper.init) //if stepper not initialized skip
        return;
    if(linear.enstate <= EN_WAIT) //if linear is in wait or set state (use rtc)
        return;
    if(stepper.enabled)
        return;
    switch(stepper.enstate){
        case EN_SET:
            PORTF.OUTCLR = PIN1_bm;
            RTC_ON(stepper.wait_time);
            stepper.enstate = EN_WAIT;
        break;
        case EN_WAIT:
            if (!RTC_Checker()) {
                RTC_OFF();
                stepper.enstate = EN_DONE;
            }
        break;
        case EN_DONE:           
            stepper.enabled = true;
            stepper.enstate = EN_SET;
        break;
    }
}

void Stepper_Disable(){
    if(!stepper.enabled)
        return;
    if(linear.enstate <= EN_WAIT) //if linear is in wait ar set state (use rtc)
        return;
    switch(stepper.enstate){
        case EN_SET:
            PORTF.OUTSET = PIN1_bm;
            RTC_ON(stepper.wait_time);
            stepper.enstate = EN_WAIT;
        break;
        case EN_WAIT:
            if (!RTC_Checker()) {
                RTC_OFF();
                stepper.enstate = EN_DONE;
            }
        break;
        case EN_DONE:
            stepper.enabled = false;
            stepper.enstate = EN_SET;
        break;
    }    
}

void Stepper_Set_Dir(){
    if(!stepper.enabled)// if disabled ignore further code
        return;
    if(stepper.run) //if running do not allow to change direction!
        return;
    if(stepper.dir_now == stepper.dir_last) //if same direction ignore further code
        return;
    switch(stepper.dirstate){
        case DIR_SET:
            if(stepper.dir_now)
                PORTF.OUTSET = PIN3_bm; //forward               
            else
                PORTF.OUTCLR = PIN3_bm; //backward         
            stepper.dirstate = DIR_WAIT;
        break;
        case DIR_WAIT:
            stepper.dirstate = DIR_DONE;
        break;
        case DIR_DONE:
            stepper.dir_last = stepper.dir_now;
            stepper.dirstate = DIR_SET;
        break;
    }
}

void Stepper_Run() {
    if(!stepper.enabled) //if disabled do not allow to run
        return;
    if(stepper.run) //if already run skip further code
        return;
    if(TCD0.STATUS & TCD_ENRDY_bm){ //if tcd is ready
        TCD0.CTRLA |= TCD_ENABLE_bm; //enable signal generations
        stepper.run = true; //set flag
    }
}

void Stepper_Stop() {
    if(!stepper.enabled) //if disabled do not allow to stop
        return;
    if(!stepper.run) //if already stopped skip further code
        return;
    if(TCD0.STATUS & TCD_ENRDY_bm){ //if tcd is ready
        TCD0.CTRLA &= ~TCD_ENABLE_bm; //disable signal generations
        stepper.run = false; //set flag
    }
}

bool Read_Stepper_ALM(){ // true if driver has a error (overheat, stepepr stuck and so on)
	return !(PORTF.IN & PIN4_bm);
}

bool Read_Stepper_PEND(){ // true if position reached
	return !(PORTF.IN & PIN5_bm);
}

void Linear_init(){
    if(linear.init)
        return;
	TCA0_init_linear_PWM(20000, 95);
	PORTD.OUTSET = PIN2_bm; //set output disabled for TLE9201SG
    linear.init = true;
};

void Linear_Enable(){
    if(!linear.init) //if not initialized not allow further code
        return;
    if(stepper.enstate <= EN_WAIT) //if stepper is in set or wait state (use rtc)
        return;    
    if(linear.enabled) //if already enabled skip
        return;
    switch(linear.enstate){
        case EN_SET:
            PORTD.OUTCLR = PIN2_bm;
            RTC_ON(linear.wait_time);
            linear.enstate = EN_WAIT;
        break;
        case EN_WAIT:
            if (!RTC_Checker()) {
                RTC_OFF();
                linear.enstate = EN_DONE;
            }
        break;
        case EN_DONE:
            linear.enabled = true;
            linear.enstate = EN_SET;
        break;
    }    
}

void Linear_Disable(){
    if(!linear.enabled)
        return;
    if(stepper.enstate <= EN_WAIT) //if stepper is in set or wait state (use rtc)
        return;
    switch(linear.enstate){
        case EN_SET:
            PORTD.OUTSET = PIN2_bm;
            RTC_ON(linear.wait_time);
            linear.enstate = EN_WAIT;
        break;
        case EN_WAIT:
            if (!RTC_Checker()) {
                RTC_OFF();
                linear.enstate = EN_DONE;
            }
        break;
        case EN_DONE:
            linear.enabled = false;
            linear.enstate = EN_SET;
        break;
    }    
}

void Linear_Set_Dir(){
    if(!linear.enabled)// if disabled ignore further code
        return;
    if(linear.run) //if running do not allow to change direction!
        return;
    if(linear.dir_now == linear.dir_last) //if same direction ignore further code
        return;
    switch(linear.dirstate){
        case DIR_SET:
            if(linear.dir_now)
                PORTD.OUTSET = PIN5_bm; //forward               
            else
                PORTD.OUTCLR = PIN5_bm; //backward         
            linear.dirstate = DIR_WAIT;
        break;
        case DIR_WAIT:
            linear.dirstate = DIR_DONE;
        break;
        case DIR_DONE:
            linear.dir_last = linear.dir_now;
            linear.dirstate = DIR_SET;
        break;
    }
}

void Linear_Run() {
    if(!linear.enabled)
        return;
    if(linear.run)
        return;
    TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
    linear.run = true;
    
}

void Linear_Stop() {
    if(!linear.enabled)
        return;
    if(!linear.run) //skip if stoped
        return;
    TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm;
    linear.run = false;
}

bool Read_LinearMotor_EF(){ // false if driver has a error (overheat, linear motor stuck and so on)
	return PORTD.IN & PIN4_bm;
}

