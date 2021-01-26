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

int main(void)
{
	uint16_t rgb_array[3] = {0, 0, 0};
	float hsv_array[3] = {0, 0, 0};
	uint8_t sample_array[200] = {0};//array for sample measurement
	uint8_t cut_sample_array[200] = {0};//array for sample measurement
	uint8_t ind=0;	
	DDRC = 0x07;
	usartInit(UBRR_VALUE);
	sensorInit(init_sensor_array);

	while (1){
			
			ind=0;
			PORTC = 0;
			uint8_t defined_colour = getSingleMeasurement(rgb_array, hsv_array);
			
			if(defined_colour) {
					PORTC = 1;
				do 
				{
					ind=getSampleArray(defined_colour,sample_array,ind);
					defined_colour = getSingleMeasurement(rgb_array, hsv_array);
				} while (defined_colour);
				uint16_t cut_array_size=cutArray(sample_array,cut_sample_array,ind);
				uint8_t most_element=getMostCommonElement(cut_sample_array,cut_array_size);
				usartTransmit(1);
				usartTransmit(most_element);
			}
			usartTransmit(0);//black
             	
	}
}
