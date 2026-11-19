#include "settings.h"

void USART0_init(){ //For RS485 communication with Weather station v.2
	USART0.BAUD = (uint16_t)USART_BAUD_RATE(115200);
    USART0.CTRLA = USART_RS485_ENABLE_gc;
	USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm | USART_RXMODE_CLK2X_gc;
	USART0.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;
}

void USART0_sendChar(char c){
	while (!(USART0.STATUS & USART_DREIF_bm));
	USART0.TXDATAL = c;
}

void USART0_sendString(char *str){
	for(size_t i = 0; i < strlen(str); i++){
		USART0_sendChar(str[i]);
	}
}

char USART0_readChar(){ //but still i keep it
	USART0.STATUS = USART_RXCIF_bm;// clear buffer before reading
	while (!(USART0.STATUS & USART_RXCIF_bm));
	return USART0.RXDATAL;
}

void USART1_init(){ //For Top Controller readings
	USART1.BAUD = (uint16_t)USART_BAUD_RATE(125000);
	USART1.CTRLB = USART_RXEN_bm | USART_RXMODE_CLK2X_gc;
	USART1.CTRLC = USART_CMODE_ASYNCHRONOUS_gc | USART_CHSIZE_8BIT_gc | USART_PMODE_DISABLED_gc | USART_SBMODE_1BIT_gc;
}

void USART1_sendChar(char c){
	while (!(USART1.STATUS & USART_DREIF_bm));
	USART1.TXDATAL = c;
}

void USART1_sendString(char *str){
	for(size_t i = 0; i < strlen(str); i++){
		USART1_sendChar(str[i]);
	}
}

char USART1_readChar(){ //but still i keep it
	USART1.STATUS = USART_RXCIF_bm;// clear buffer before reading
	while (!(USART1.STATUS & USART_RXCIF_bm));
	return USART1.RXDATAL;
}


void USART_printf(uint8_t usart_ch, const char *format, ...) {
	char buffer[96]; // Temporary buffer for formatted message
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, sizeof(buffer), format, args); // Format the message into the buffer
	va_end(args);
    if(usart_ch == 0)
        USART0_sendString(buffer); // Use USART0 for sending
    else
        USART1_sendString(buffer);
}