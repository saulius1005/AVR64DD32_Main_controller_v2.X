#include "settings.h"
#include "CommunicationVar.h"

uint16_t fast_atoi_hex(const char *p, uint8_t digits){// char to uint16_t
    
    uint16_t value = 0;
    for (uint8_t i = 0; i < digits; i++){
        char c = p[i];
        value <<= 4;
        if (c >= '0' && c <= '9')
            value |= c - '0';
        else if (c >= 'A' && c <= 'F')
            value |= c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            value |= c - 'a' + 10;
    }
    return value;
    
}

bool UART_VerifyCRC(char *buf, uint8_t data_length) {
    
    uint8_t crc_data[UART_CRC_MAX_BYTES];
    uint8_t bytes = (data_length + 1) / 2; //how much full Bytes

    if(bytes > UART_CRC_MAX_BYTES)
        return false;
    for(uint8_t i = 0, j = 0; i < data_length; i += 2, j++) {
        uint8_t digits = (data_length - i >= 2) ? 2 : 1; //how much digits? last one is half Byte then 1 digit if full 2 digits
        crc_data[j] = (uint8_t)fast_atoi_hex(&buf[i], digits); //convert char to hex
    }
    uint8_t CC = (uint8_t)fast_atoi_hex(&buf[data_length], 2); 
    return verify_crc8_cdma2000(crc_data, bytes, CC);
    
}

void TopDataParser(char *buf){
    
    if(!UART_VerifyCRC(buf, TOP_MESSAGE_LENGTH)) { //check data crc without start and end symbols (17-2 = 15)
        top_controller.status = CRC_ERROR;
        return;
    } //skip further code if crc error

    uint8_t i = 0;
    uint8_t endSwitches = 0;
    
    sensor_data.HR_EL = fast_atoi_hex(&buf[i], 4); i += 4; //0-3
    sensor_data.HR_AZ   = fast_atoi_hex(&buf[i], 4); i += 4; //4-7
    sensor_data.PVU         = fast_atoi_hex(&buf[i], 3); i += 3; //8-10
    sensor_data.PVI         = fast_atoi_hex(&buf[i], 3); i += 3; //11-13
    endSwitches             = fast_atoi_hex(&buf[i], 1);         //14
    
    sensor_data.ElMin = endSwitches & 0x01;// extracting each end swith value
	sensor_data.ElMax = endSwitches & 0x02;
	sensor_data.AzMin = endSwitches & 0x04;
	sensor_data.AzMax = endSwitches & 0x08;
    
    top_controller.status = READY; //all ok
  
}

void SendAnswerToWeatherStation() {   
        
}

void WS_RequestParser(char* buf){
    
    if(!UART_VerifyCRC(buf, RS485_CMD_LENGTH)) { //check data crc without start and end symbols (18-2 = 16) or macro already should be 16+1?. Tests answer this.
        weather_station.status = CRC_ERROR;
        return;
    } //skip further code if crc error
    
    uint8_t i = 2; //start after id symbols exmpl message: "0152fd0437015cc71a" skipping first 01 

    ws_data.AZ  = fast_atoi_hex(&buf[i], 4); i += 4; //2-5
    ws_data.EL  = fast_atoi_hex(&buf[i], 4); i += 4; //6-9
    ws_data.WS  = fast_atoi_hex(&buf[i], 2); i += 2; //10-11
    ws_data.WD  = fast_atoi_hex(&buf[i], 1); i += 1; //12
    ws_data.SLS = fast_atoi_hex(&buf[i], 3);         //13-15

    weather_station.status = READY; //set flag to ready (meaning all ok data readed )
    
};

bool Top_Filter(char *cmd) {
    return true;
}

bool WS_Filter(char *cmd) {
    return strncmp(cmd, ws_data.Device_ID, 2) == 0;
}

void WS_OnStart() { //turn on rs485 rx led
    RX_LED_ON();
}

void WS_OnEnd() { //turn off
    RX_LED_OFF();
}

void UART_Receive(uart_receiver_t *uart) {

    uart->index = 0;
    uart->start = 0;
    uart->status = BUSY; //set flag to busy
    RTC_ON(uart->timeout); //start timer
    
    while(uart->status) { //do until timeout or...
        char c = uart->callbacks.read_char();
        if(uart->start) { //if start condition
            if(c == uart->end_symbol) { //if received message end symbol
                uart->start = 0;
                uart->buffer[uart->index] = '\0';
                uart->index = 0;
                if(uart->callbacks.filter(uart->buffer)) {// for top controller does nothing, but data from weather station need to check if ID match 
                    uart->callbacks.parser(uart->buffer); //data parsing
                }
                if(uart->callbacks.on_end) //action after end (only weather station (rs485) turn off rx led)
                    uart->callbacks.on_end();
            }
            else if(uart->index < uart->buffer_length - 1) { //if bufer overfill start from 0
                uart->buffer[uart->index++] = c;
            }
            else {
                uart->index = 0;
                uart->start = 0;
            }
        }
        if(c == uart->start_symbol) { //if received start symbol
            uart->start = 1;
            uart->index = 0;
            if(uart->callbacks.on_start) //action after start (only weather station (rs485) turn on rx led)
                uart->callbacks.on_start();
        }
    }
    RTC_OFF();
}


