/* 
 * File:   ADC.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugs?jis 18, 16.20
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus
extern "C" {
#endif
    
typedef enum {
    JX_ch,
    JY_ch,
    SU_ch,            
    LU_ch,
    SI_ch,
    LI_ch,
    MCU_ch,
}ADC_CH_name_t;
    

typedef struct {
    ADC_MUXPOS_t channel;
    VREF_REFSEL_t reference;
    uint16_t result;
    uint16_t out;
} analog_t;

typedef struct{
    analog_t JX; //Joystick X
    analog_t JY; //Joystick Y
    analog_t SU; //Stepper Voltage
    analog_t SI; //Stepper Current
    analog_t LU; //Linear Voltage
    analog_t LI; //Linear Current
    analog_t MCU;
}analog_sensors_t;

extern analog_sensors_t AnalogSensors;
extern analog_t *ADC0_sensors[7]; //same but different

void ADC0_init(); //adc settings
void ADC_read_all(); //read all analog voltage channels

#ifdef	__cplusplus
}
#endif

#endif	/* ADC_H */

