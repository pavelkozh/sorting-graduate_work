#ifndef uartlib_H_INCLUDED
#define uartlib_H_INCLUDED
#include <avr/io.h>
#define BAUD_RATE 9600
#define UBRR_VALUE 8000000/16/BAUD_RATE-1
#define SENS_ADDR 0x29 // RGB sensor's i2c address is 41
#define CDATAL_ADDR 0x14 //clear data h address
#define CDATAH_ADDR 0x15 //clear data l address
#define RDATAL_ADDR 0x16 //red data h address
#define RDATAH_ADDR 0x17 //red data l address
#define GDATAL_ADDR 0x18 //green data h address
#define GDATAH_ADDR 0x19 //green data l address
#define BDATAL_ADDR 0x1A //blue data h address
#define BDATAH_ADDR 0x1B //blue data l address

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

#endif
