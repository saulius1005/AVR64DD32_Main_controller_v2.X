#include "settings.h"
#include "TopControllerVar.h"

uint16_t fast_atoi_hex(const char *p, uint8_t digits){// char to hex
    
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

void TopDataParser(char *buf){
    
    uint8_t i = 0;
    uint8_t crc_data[8];// 15b/ 2 = 7.5 ~= 8B    exmpl message: "046e54ae87200d0a6"
    uint8_t endSwitches = 0;

    for (uint8_t j = 0; j < 8; j++) {
        crc_data[j] = (uint8_t)fast_atoi_hex(&buf[j * 2], j == 7? 1:2);//'04'->0x04, '6e'->0x6e, '54'->0x54, 'ae'->0xae, '87'->0x87, '20'->0x20, '0d'->0x0d, '0'->0x00
    }    
    uint8_t CC = (uint8_t)fast_atoi_hex(&buf[15], 2); //15,16 symbols are crc 'a6'->0xa6
    bool crc_ok = verify_crc8_cdma2000(crc_data, 8, CC);//crc buf, total bytes, received crc
    
    if(!crc_ok)//skip further code if crc is not correct
        return;    

    sensor_data.HPElevation = fast_atoi_hex(&buf[i], 4); i += 4; //0-3
    sensor_data.HPAzimuth   = fast_atoi_hex(&buf[i], 4); i += 4; //4-7
    sensor_data.PVU         = fast_atoi_hex(&buf[i], 3); i += 3; //8-10
    sensor_data.PVI         = fast_atoi_hex(&buf[i], 3); i += 3; //11-13
    endSwitches             = fast_atoi_hex(&buf[i], 1); //14
    
    sensor_data.ElMin = endSwitches & 0x01;// extracting each end swith value
	sensor_data.ElMax = endSwitches & 0x02;
	sensor_data.AzMin = endSwitches & 0x04;
	sensor_data.AzMax = endSwitches & 0x08;
  
}

void TopReceiver() {
    
    static uint8_t index = 0;
	static char cmd[TOP_MESSAGE_LENGTH] = {0}; // Empty command array
	static bool start = false;

    char c = USART0_readChar(); // Reading a character from USART
     
    if (start) {
        if (c == '>') { // If received cmd end symbol
            start = false;
            cmd[index] = 0;
            index = 0;
			TopDataParser(cmd);
        } 
        else if (index < TOP_MESSAGE_LENGTH) {// fill aray
                cmd[index++] = c; // Store received character in command array
        }
        else{ //if array full start from the beginning
            index = 0;
            start = false;
        }
    }
    if (c == '<') { // If received cmd start symbol
        start = true;
        index = 0;
    }
    
}