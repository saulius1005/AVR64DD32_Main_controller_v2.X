/* 
 * File:   RS485.h
 * Author: Saulius
 *
 * Created on Penktadienis, 2026, rugs?jis 18, 21.40
 */

#ifndef RS485_H
#define	RS485_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define RS485_CMD_LENGTH 18 // usart command length received from weather station 
    
#define RX_LED_ON()    (PORTC.OUTSET = PIN0_bm)
#define RX_LED_OFF()    (PORTC.OUTCLR = PIN0_bm)
    
#define TX_LED_ON()    (PORTA.OUTSET = PIN6_bm)
#define TX_LED_OFF()    (PORTA.OUTCLR = PIN6_bm)    
    
typedef enum {
    WS_READY = 0,
    WS_READING,
    WS_BUSY,
    WS_CRC_ERROR,
    WS_TIMEOUT_ERROR
} ws_status_t;    
    
typedef struct {
    ws_status_t WSStatus;
    char Device_ID[3];
	uint16_t AZ;
	int16_t EL;
	uint8_t WS;
	uint8_t WD;
	uint16_t SLS;
} ws_t;

extern ws_t ws_data;

void SendAnswerToWeatherStation(); //data send to weather station function   
void ReadWeatherStation(); //infinite wait for command from weather station until  receive "{GET}"    

#ifdef	__cplusplus
}
#endif

#endif	/* RS485_H */

