/*
 * File:   main.c
 * Author: Saulius
 *
 * Created on Antradienis, 2026, lapkritis 17, 22.47
 */


#include "settings.h"
#include "MCU_Config.h"

int main(void) {
    CLOCK_INHF_clock_init();
    GPIO_init();
    I2C_init();  
    
    stepper.callbacks.init(); //MCU config for stepper
    linear.callbacks.init(); //MCU config for linear
    
    TCB0_init(); //initiate us timer module (I2c timeout)
    TCB1_init();
    
    screen_init();   
    screen_clear();
    
    TCB1_ON(5000);

    bool on = false;
    bool testflag = false;
    
    uint16_t half_second = 0;
    show_window();    

    while (1) {
        
        if (!stepper.motor_init.init_status) { //if stepper not init
            stepper.motor_init.action = ENABLE;
        }
        else if(!linear.motor_init.init_status){ //only after, do the same with linear
            linear.motor_init.action = ENABLE;           
        }
        
        
        stepper.direction = FORWARD;
        linear.direction = FORWARD;

        Motor_Actions(&stepper);
        Motor_Actions(&linear);    
        
         if(!TCB1_Checker()){ //run stop every 5ms?
            PORTC.OUTTGL = PIN0_bm;   
            if(testflag){
                testflag = false;
                stepper.motor_init.action = RUN;
                linear.motor_init.action = RUN;
            }
            else{
                testflag = true;
                stepper.motor_init.action = STOP;
                linear.motor_init.action = STOP;
            }
         }
    }
}
