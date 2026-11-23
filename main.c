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

    TCA0_init_linear_PWM(20000, 95);
    TCD0_init_stepper_PWM(25600, 50);
    
    TCB0_init(); //initiate us timer module (I2c timeout)
    TCB1_init();
    
    RTC_ON(500);
    
    screen_init();   
    screen_clear();

    bool on = false;
    show_window();
    
    while (1) {

         if(!RTC_Checker()){ //led toggling every 500ms
            PORTC.OUTTGL = PIN0_bm;   
            if(on){
                on = false;
                TCA0.SPLIT.CTRLB &= ~TCA_SPLIT_HCMP0EN_bm;
                while (!(TCD0.STATUS & TCD_ENRDY_bm));
                    TCD0.CTRLA |= TCD_ENABLE_bm;
            }
            else{
                on = true;
                TCA0.SPLIT.CTRLB |= TCA_SPLIT_HCMP0EN_bm;
                while (!(TCD0.STATUS & TCD_ENRDY_bm));
                    TCD0.CTRLA &= ~TCD_ENABLE_bm;
            }
         }
    }
}
