#include "settings.h"
#include "crc8Var.h"


uint8_t crc8_cdma2000(uint8_t* buf, uint8_t i){
	uint8_t crc = 0xFF;
	for (uint8_t j = 0; j < i; j++)
	crc = crc8_table[crc ^ buf[j]];

	return crc;
}

uint8_t crc_packer(){
    uint8_t buf_for_crc8[8]; //temprary buffer for crc calculation
	uint8_t i = 0;
        
    i = crc8_cdma2000(buf_for_crc8, i);    
	return i;
}

bool verify_crc8_cdma2000(uint8_t *data, uint8_t length, uint8_t crc) {
	uint8_t calculatedcrc = 0xFF;
	for (size_t i = 0; i < length; i++) { // length = 8 baitai
		calculatedcrc = crc8_table[calculatedcrc ^ data[i]];
	}   
	return calculatedcrc == crc;
}