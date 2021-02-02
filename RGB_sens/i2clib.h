#ifndef i2clib_H_INCLUDED
#define i2clib_H_INCLUDED
#include <avr/io.h>
#define SENS_ADDR 0x29 // RGB sensor's i2c address is 41
#define RDATAL_ADDR 0x16 //red data h address
#define RDATAH_ADDR 0x17 //red data l address
#define GDATAL_ADDR 0x18 //green data h address
#define GDATAH_ADDR 0x19 //green data l address
#define BDATAL_ADDR 0x1A //blue data h address
#define BDATAH_ADDR 0x1B //blue data l address

/// The function sends START condition
void i2cStart(void);

/// The function writes 8 bits data
/// Parameters: data - 8 bits of data to transmit
void i2cWrite(uint8_t data);

/// The function reads 8 bits data
uint8_t i2cRead(void);

/// The function sends STOP condition
void i2cStop(void);

#endif