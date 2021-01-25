/*
 * RGB_sens.c
 *
 * Created: 30.10.2020 19:31:41
 * Author : User
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <math.h>
#include "i2clib.h"
#include "uartlib.h"
#include "sensorlib.h"

uint8_t init_sensor_array[10] = {4, SENS_ADDR, 0x03, 0x01, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0x03};
uint16_t ind = 0;//index of sample array's element
uint8_t flag = 0;


int main(void)
{
	uint16_t rgb_array[3] = {0, 0, 0};
	float hsv_array[3] = {0, 0, 0};
	uint8_t sample_array[200] = {0};//array for sample measurement
	uint8_t cut_sample_array[200] = {0};//array for sample measurement
		
	DDRC = 0x07;
	usartInit(UBRR_VALUE);
	sensorInit(init_sensor_array);

	while (1){
			
			uint8_t is_colour_defined = getSingleMeasurement(rgb_array, hsv_array);
			getCutSampleArray(is_colour_defined,sample_array, cut_sample_array);


			/*
        rgb_array[0]=readColour(RDATAL_ADDR,RDATAH_ADDR);//red
        rgb_array[1]=readColour(GDATAL_ADDR,GDATAH_ADDR);//green
        rgb_array[2]=readColour(BDATAL_ADDR,BDATAH_ADDR);//blue
             		
        if (rgb2hsv(pointer_rgb,pointer_hsv)) {
	            usartTransmit(1);//colour is defined
	            colour_code=getColourCode(hsv_array[0]);
	            usartTransmit(colour_code);
        }
        else usartTransmit(0);//colour is not defined
             */		
             	
	}
}
