#ifndef sensorlib_H_INCLUDED
#define sensorlib_H_INCLUDED
#define F_CPU 8000000UL
#include <avr/io.h>
#include "i2clib.h"
#include <util/delay.h>

/// Function for timer0 initialization
void timer0Init(void);

/// TCS34725 RGB Sensor's initializing
/// Parameters: init_array - array of array containing
/// the number of registers to write, i2c sensor's address,
/// registers for writing and values for writing
void sensorInit(uint8_t *init_array);

/// The function for reading 16 bits result of ADC conversion
/// Parameters: rgb_array - pointer to rgg array
void readColour(uint16_t *rgb_array);

/// The function for starting new ADC conversion
void startConvertion(void);

/// The function for converting RGB data to HSV data
/// Parameters: in_rgb_array - pointer to input RGB array;
/// out_hsv_array - pointer to output HSV array;
uint8_t rgb2hsv(uint16_t* in_rgb_array, float* out_hsv_array);

/// The function returns code of defined colour:
/// 1 - red; 2 - orange; 3 - yellow; 4 - green; 5 - light blue;
/// 6 - blue; 7 - pink; 8 - brown
/// Parameters: hsv_array - Pointer to HSV array
uint8_t getColourCode(float *hsv_array);

/// The function returns code (1-7) of most common element in array:
/// Parameters: *array - array for finding most common element;
/// size_of_array - size of input array
uint8_t getMostCommonElement(uint8_t *array, uint16_t size_of_array);

/// The function cuts array from the edges (10% of array size) 
/// Parameters: *input_array - pointer at array to cut;
/// *output_array - pointer at cut array;
/// num_of_elements - size of input_array
uint16_t cutArray(uint8_t *input_array,uint8_t *output_array, uint16_t num_of_elements);

/// The function returns colour code (from 1 to 7) or 0 if colour is not defined
/// Parameters: *rgb_array_pointer - pointer at rgb array;
/// *hsv_array_pointer - pointer at hsv array;
uint8_t getSingleMeasurement(uint16_t* rgb_array_pointer, float* hsv_array_pointer);

///The function fills sample_array and returns total count of read elements
/// Parameters: defined_colour - code of defined colour (1-7)
/// or 0 if colour is not defined;
///* sample_array - pointer at array with read colour codes;
///*cut_array - pointer at cut sample array;
uint16_t getSampleArray(uint8_t defined_colour, uint8_t* sample_array, uint16_t elem_index);

#endif