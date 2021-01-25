#ifndef sensorlib_H_INCLUDED
#define sensorlib_H_INCLUDED
#define F_CPU 8000000UL
#include <avr/io.h>
#include "i2clib.h"
#include <util/delay.h>


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