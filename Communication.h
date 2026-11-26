/* 
 * File:   Communication.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, lapkritis 26, 21.24
 */

#ifndef COMMUNICATION_H
#define	COMMUNICATION_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define TOP_MESSAGE_LENGTH 15  // 17-2 = 15/2 = 7.5 ~ 8B or it should be 15? reding function automatically not include start and end symbols... Future shows how it be and leave that for tests
#define RS485_CMD_LENGTH 16 // usart command length received from weather station 18-2 = 16/2 = 8B
#define UART_CRC_MAX_BYTES 8 //7+ one for \0 total 8
    
#define RX_LED_ON()    (PORTC.OUTSET = PIN0_bm)
#define RX_LED_OFF()    (PORTC.OUTCLR = PIN0_bm)
    
#define TX_LED_ON()    (PORTA.OUTSET = PIN6_bm)
#define TX_LED_OFF()    (PORTA.OUTCLR = PIN6_bm)
    
typedef enum {
    READY = 0,
    BUSY,
    CRC_ERROR,
    TIMEOUT_ERROR
} device_status_t; 

typedef struct {
	int8_t EL; //0-90
	int16_t HR_EL; // 000-900
	uint16_t AZ; //0-359
	uint16_t HR_AZ; // 000-35999
	uint16_t PVU; //30000max (300.00V)
	uint16_t PVI; //exmpl 150- 1.5A max is 1250 or 12.5A (need to check TopController firmware) tmcs1100A4 range is 0.125-12.5A
	bool ElMin;
	bool ElMax;
	bool AzMin;
	bool AzMax;
}top_cont_t;

    
typedef struct {
    char Device_ID[3];
	uint16_t AZ;
	int16_t EL;
	uint8_t WS;
	uint8_t WD;
	uint16_t SLS;
} ws_t;

typedef struct { //calback for both motors are the same
    char (*read_char)(void);
    void (*parser)(char *cmd);
    bool (*filter)(char *cmd);
    void (*on_start)(void);
    void (*on_end)(void);
} uart_callbacks_t;

typedef struct {
    uint8_t index;
    uint8_t start;
    device_status_t status;

    uint16_t timeout;
    uint16_t buffer_length;

    char *buffer;

    char start_symbol;
    char end_symbol;

    uart_callbacks_t callbacks;

} uart_receiver_t;


extern top_cont_t sensor_data;
extern ws_t ws_data;


extern char top_buffer[TOP_MESSAGE_LENGTH + 1];
extern char ws_buffer[RS485_CMD_LENGTH + 1];

extern uart_receiver_t top_controller;
extern uart_receiver_t weather_station;

void TopDataParser(char *buf);
void WS_RequestParser(char* buf);
bool Top_Filter(char *cmd);
bool WS_Filter(char *cmd);
void WS_OnStart(void);
void WS_OnEnd(void);


#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATION_H */

