#ifndef uartlib_H_INCLUDED
#define uartlib_H_INCLUDED
#include <avr/io.h>
#define BAUD_RATE 19200
#define UBRR_VALUE 8000000/16/BAUD_RATE-1


/// UART initialization
void usartInit(uint8_t ubrr);

/// The function reads 8 bits data
uint8_t usartReceive(void);

/// The function writes 8 bits data
void usartTransmit(uint8_t data);

/// The function writes 16 bits (2 bytes) data
void usartTransmitTwoBytes(uint16_t DATA);

/// The function writes 32 bits (4 bytes) of float data
void usartTransmitFloat(float in_value);

/// The function writes elements of array
/// Parameters: array - pointer at the beginning of array;
/// count_of_elements - count of elements in array
/// type - type of array (1 - uint8_t; 2 - uint16_t; 4 - float)
void usartTransmitArray(uint8_t *array, uint16_t count_of_elements, uint8_t type);
	
#endif