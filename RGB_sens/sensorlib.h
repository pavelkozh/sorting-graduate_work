#ifndef sensorlib_H_INCLUDED
#define sensorlib_H_INCLUDED
#define F_CPU 8000000UL
#include <avr/io.h>
#include "i2clib.h"
#include <util/delay.h>
#define BAUD_RATE 19200
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

/// TCS34725 RGB Sensor's initializing
/// Parameters: init_array - array of array containing
/// the number of registers to write, i2c sensor's address,
/// registers for writing and values for writing
void sensorInit(uint8_t *init_array);

/// The function for reading 16 bits result of ADC conversion
/// Parameters: low_addr, high_addr - addresses of red, green or blue low and high data
uint16_t readColour(uint8_t low_addr, uint8_t high_addr);

/// The function for converting RGB data to HSV data
/// Parameters: in_rgb_array - pointer to input RGB array; out_hsv_array - pointer to output HSV array;
uint8_t rgb2hsv(uint16_t* in_rgb_array, float* out_hsv_array);

/// The function returns code of defined colour:
/// 0 - red; 1 - orange; 2 - yellow; 3 - green; 4 - light blue;
/// 5 - blue; 6 - pink;
/// Parameters: hue - Value of parameter Hue in HSV array
uint8_t getColourCode(float hue);

#endif