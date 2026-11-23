#include "settings.h"
#include "ADCVar.h"

void ADC0_init() {
    ADC0.CTRLB = ADC_SAMPNUM_ACC16_gc; // 16 measurements //32,64,128 is truncated
    ADC0.CTRLC = ADC_PRESC_DIV256_gc; // 24Mhz / 256 ADC speed = 93.75kHz (160kHz is top)
    ADC0.CTRLD = ADC_SAMPDLY_DLY15_gc | ADC_INITDLY_DLY256_gc ;//15cpu cycles betwean each sample, intialization is 256 cpu cycles after vref selection
    ADC0.SAMPCTRL = 255;//one sampling duration is extended by 255 cpu cycles
    ADC0.CTRLA = ADC_ENABLE_bm | ADC_RESSEL_12BIT_gc; // 12-bit resolution
}

void ADC0_read(ADC_CH_name_t channel) {
    
    ADC0.MUXPOS = ADC0_sensors[channel]->channel;    
    VREF.ADC0REF = ADC0_sensors[channel]->reference;
    
    ADC0.COMMAND = ADC_STCONV_bm; // Start conversion
    while (!(ADC0.INTFLAGS & ADC_RESRDY_bm)); // Wait until result is ready
    uint16_t answer = ADC0.RES>>4;
    if (channel <= JY_ch) //JX and JY or 0 and 1
        ADC0_sensors[channel]->result = (uint16_t)(((uint32_t)answer * ADC0_sensors[MCU_ch]->result / 4096)); //ref 5.0V +- something
    else if (channel <= LU_ch) //SU and LU or 2 and 3
        ADC0_sensors[channel]->result = answer >> 1; //divide by 2 (ref 2.048V)
    else if (channel <= LI_ch) //SI and LI or 4 and 5
        ADC0_sensors[channel]->result = answer; //as it is (ref 4.096V)
    else //MCU or 6
        ADC0_sensors[channel]->result = (uint16_t)((uint32_t)answer * 1024 / 409.6);// to get proper voltage around 5V + - 
    
    ADC0.COMMAND = ADC_SPCONV_bm; // Stop conversion
}

void ADC_read_all(){ // all measurements should take: 97.75kHz / ((7*16) + (0.625us * 16) + 10.67us + 10.65us)  = ~1.17ms + computing and calculations
    for(uint8_t i = 0; i < 7; i++){
        ADC0_read(6-i); //start from MCU voltage
    }
    
    //Joystick calculations
    ADC0_sensors[JX_ch]->out = (int16_t)(ADC0_sensors[JX_ch]->result - 2048) * Joystick_steps;//12bit ad middle is 2048. How many of one direction swing had different options for actions 4096/819 = 5 of full range 2 for right, one neutral, and 2 for left
    ADC0_sensors[JY_ch]->out = (int16_t)(ADC0_sensors[JY_ch]->result - 2048) * Joystick_steps;//same for y axis 2 top, 1 in middle neutral, and 2 for bottom 
    
    //Motors voltage calculations
    ADC0_sensors[SU_ch]->out = ((double)ADC0_sensors[SU_ch]->result * 0.1775) + 0.5; //AMC1131 full range is 2.0V, so 4000ADC (2V at ref 2.048V) meaning 71V(710k) (stepper) and if we want use one digit after point, interpretate it as 4000 == 710, then 710/4000 = 0.1775. EXMP adc reads 3000 * 0.1775 = 532,5 + 0.5 = 533 or 53.3V
    ADC0_sensors[LU_ch]->out = ((double)ADC0_sensors[LU_ch]->result * 0.075) + 0.5; //For linear difference is AMC1131 resistors and voltage 30V (300k) so koef is 300/4000 = 0.075. Exmpl 3000ADC = 225 + 0.5 = 225.5 or 22.5V 
    
    //Current calculation
    ADC0_sensors[SI_ch]->out = ((double)ADC0_sensors[SI_ch]->result * 0.25) + 0.5; //TMCS1100A4 is 400mV/A, ADC ref is 4.096V, 4096 total adc steps. PERFECT!!! 400 adc steps = 1A, so if i want to see 1.00 use it as 100/400 = 0.25. Exmp. ADC = 158*0.25 = 39.5 + 0.5 = 40 or 0.40A
    ADC0_sensors[LI_ch]->out = ((double)ADC0_sensors[LI_ch]->result * 0.25) + 0.5; //identical current measurement
    
    
    
}