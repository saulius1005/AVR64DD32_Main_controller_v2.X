#include "settings.h"
#include "WeatherStationHandlerVar.h"

void SendAnswerToWeatherStation() {   
        
}

void WS_RequestParser(char* buf){
    
    uint8_t i = 2; //start after id symbols exmpl message: "0152fd0437015cc71a" skipping first 01
    uint8_t crc_data[7];// 14b/ 2 = 7B    

    for (uint8_t j = 0; j < 7; j++) {
        crc_data[j] = (uint8_t)fast_atoi_hex(&buf[j * 2], 2);//'52'->0x52, 'fd'->0xfd, '04'->0x04, '37'->0x37, '01'->0x01, '5c'->0x5c, 'c7'->0xc7
    }    
    uint8_t CC = (uint8_t)fast_atoi_hex(&buf[16], 2); //16,17 symbols are crc '1a'->0x1a
    bool crc_ok = verify_crc8_cdma2000(crc_data, 7, CC);//crc buf, total bytes, received crc
    
    if(!crc_ok){//skip further code if crc is not correct
        ws_data.WSStatus = WS_CRC_ERROR;
        return;  
    }

    ws_data.AZ  = fast_atoi_hex(&buf[i], 4); i += 4; //2-5
    ws_data.EL  = fast_atoi_hex(&buf[i], 4); i += 4; //6-9
    ws_data.WS  = fast_atoi_hex(&buf[i], 2); i += 2; //10-11
    ws_data.WS  = fast_atoi_hex(&buf[i], 1); i += 1; //12
    ws_data.SLS = fast_atoi_hex(&buf[i], 3);         //13-15

    ws_data.WSStatus = WS_BUSY; //set flag to busy and do other steps, such as reading sensor data from top controller
    
};


void ReadWeatherStation(){
    
    static uint8_t index = 0;
	static char cmd[RS485_CMD_LENGTH] = {0}; // Empty command array
	static uint8_t start = 0;
    ws_data.WSStatus = WS_READING; //set as busy and start reading
    RTC_ON(timer.weather_station_respond_time);    
    while(ws_data.WSStatus == WS_READING){
        char c = USART0_readChar(); // Reading a character from USART    
        if (start) {
            if (c == '}') { // If received cmd end symbol
                RX_LED_OFF();
                start = 0;
                cmd[index] = 0;
                index = 0;
                if (strncmp(cmd, ws_data.Device_ID, 2) == 0) { 
                    WS_RequestParser(cmd);		
                }
            } 
            else if (index < RS485_CMD_LENGTH) {// fill aray
                    cmd[index++] = c; // Store received character in command array
            }
            else{ //if array full start from beginning
                index = 0;
                start = 0;
            }
        }
        if (c == '{') { // If received cmd start symbol
            start = 1;
            index = 0;
            RX_LED_ON();
        }      
    }
    RTC_OFF();
    
}