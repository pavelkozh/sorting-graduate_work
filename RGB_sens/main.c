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

uint8_t init_sensor_array[10] = {4, SENS_ADDR,0x0F, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x03};
uint16_t rgb_array[3] = {0, 0, 0};
float hsv_array[3] = {0, 0, 0};
uint8_t defined_colour=0;
uint8_t shot=0;

ISR (TIMER0_COMPA_vect)
{

	defined_colour=getSingleMeasurement(rgb_array,hsv_array);
	if (defined_colour) shot=1;
	
}
	
int main(void)
{

	uint8_t sample_array[200] = {0};//array for sample measurement
	uint8_t cut_sample_array[200] = {0};//array for sample measurement
	uint8_t ind=0;	
	DDRC = 0x07;
	usartInit(UBRR_VALUE);
	sensorInit(init_sensor_array);
	
	TCCR0A|=(1<<1);      //CTC mode
	TCCR0B|=(1<<2); // prescale 256
	TIMSK0|=(1<<1);//enable interrupt OCRA1	
	OCR0A=0x4E;//   8000000/256 * 2,5e-3 = 78 (4E in hex) - 2.5 ms
	
	sei();
	TCNT0=0x4D;// generate interrupt for reading colour
	
	while (1){
				

		ind=0;
		PORTC = 0;
		if(defined_colour) {
			PORTC = 1;
			do
			{
				if(shot){
					

					ind=getSampleArray(defined_colour,sample_array,ind);
					//usartTransmitFloat(hsv_array[0]);
					shot=0;
				}
				
			} while (defined_colour);
			
			uint16_t cut_array_size=cutArray(sample_array,cut_sample_array,ind);
			uint8_t most_element=getMostCommonElement(cut_sample_array,cut_array_size);
			usartTransmit(1);
			usartTransmit(most_element);
			
		}
		else //usartTransmitFloat(-1.0);
		usartTransmit(0);//black

	}
}