/* 
 * File:   CommunicationVar.h
 * Author: Saulius
 *
 * Created on Ketvirtadienis, 2026, lapkritis 26, 21.24
 */

#ifndef COMMUNICATIONVAR_H
#define	COMMUNICATIONVAR_H

#ifdef	__cplusplus
extern "C" {
#endif

    
char top_buffer[TOP_MESSAGE_LENGTH + 1] = {0};
char ws_buffer[RS485_CMD_LENGTH + 1] = {0};    

top_cont_t sensor_data = {
  .AZ =  157,
  .EL = 34
};

ws_t ws_data = {
  .AZ = 17190,
  .EL = 3590,
  .Device_ID = "00\0"  
};

uart_receiver_t top_controller = {
    .index = 0,
    .start = 0,
    .status = READY,
    .timeout = 500,
    .buffer_length = sizeof(top_buffer),
    .buffer = top_buffer,

    .start_symbol = '<',
    .end_symbol = '>',

    .callbacks = {
        .read_char = USART1_readChar,
        .parser = TopDataParser,
        .filter = Top_Filter,
        .on_start = NULL,
        .on_end = NULL
    }
};

uart_receiver_t weather_station = {
    .index = 0,
    .start = 0,
    .status = READY,
    .timeout = 100,
    .buffer_length = sizeof(ws_buffer),
    .buffer = ws_buffer,

    .start_symbol = '{',
    .end_symbol = '}',

    .callbacks = {
        .read_char = USART0_readChar,
        .parser = WS_RequestParser,
        .filter = WS_Filter,
        .on_start = WS_OnStart,
        .on_end = WS_OnEnd
    }
};

#ifdef	__cplusplus
}
#endif

#endif	/* COMMUNICATIONVAR_H */

