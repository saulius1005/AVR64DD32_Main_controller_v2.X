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
    
    TCB0_init(); //initiate us timer module (I2c timeout)
    TCB1_init();
    
    screen_init();   
    screen_clear();
    
    TCB2_ON(5000);
    
    stepper.callbacks.init();
    linear.callbacks.init();

    bool testflag = false;
    show_window();    

    while (1) {
        
        
        stepper.callbacks.enable();
        linear.callbacks.enable();
                
        stepper.callbacks.dir_forward();
        linear.callbacks.dir_forward();
         
         if(!TCB2_Checker()){ //run stop every 5ms?
            PORTC.OUTTGL = PIN0_bm;   
            if(testflag){
                testflag = false;
                stepper.callbacks.run();
                linear.callbacks.run();
            }
            else{
                stepper.callbacks.stop();
                linear.callbacks.stop();
                testflag = true;                 
            }
         }
    }
}
