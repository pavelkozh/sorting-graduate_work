#include "uartlib.h"

void usartInit(uint8_t ubrr){
	
	/*Set baud rate */
	UBRR0H = ubrr>>8;
	UBRR0L = ubrr;
	/*Enable transmitter and receiver */
	UCSR0B = (1<<TXEN0)|(1<<RXEN0);
	/* Set frame format: 8 data bits, 1 stop bit, parity mode disabled */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
	
}

uint8_t usartReceive(void)
{
	
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
	
}

void usartTransmit(uint8_t data){
	
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;
	
}

void usartTransmitTwoBytes(uint16_t data){
	
	usartTransmit(data>>8);
	usartTransmit(data);
	
}

void usartTransmitFloat(float in_value){
	
	unsigned char *pointer;
	pointer = (unsigned char *)&in_value;
	for (uint8_t i=0;i<4;i++)
	usartTransmit(*(pointer++));
	
}