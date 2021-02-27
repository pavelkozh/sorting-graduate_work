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
#include "servolib.h"

uint8_t init_sensor_array[10] = {4, SENS_ADDR,0x0F, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x01, 0x03};
uint16_t rgb_array[3] = {0, 0, 0};
float hsv_array[3] = {0, 0, 0};
uint8_t defined_colour=0;
uint8_t shot=0;
/*uint16_t counter=0;
uint8_t timeout_rotate=0;
uint8_t timeout_forward_push=0;
uint8_t timeout_backward_push=0;*/
uint16_t timer2_counter=0;
uint8_t change_state=0;
ISR (TIMER0_COMPA_vect)
{
	
	defined_colour=getSingleMeasurement(rgb_array,hsv_array);

	if (defined_colour) shot=1;
	
}

ISR (TIMER2_COMPA_vect)
{
	
	timer2_counter++;
	if (timer2_counter==50){
		
		timer2_counter=0;
		updateServoState();
		change_state=1;
	}
	
}
	
int main(void)
{
	uint16_t count=0;
	uint8_t line_array[15]={0};
	uint8_t sample_array[200] = {0};//array for sample measurement
	uint8_t cut_sample_array[200] = {0};//array for sample measurement
	uint8_t ind=0;	
	
	DDRC = 0x07;
	usartInit(UBRR_VALUE);
	sensorInit(init_sensor_array);
	servoInit();
	timer0Init();
	timer2Init();
	sei();

	while (1){
		
		ind=0;
		PORTC = 0;
		if(defined_colour) {
			PORTC = 1;
			do
			{
				if(shot){

					ind=getSampleArray(defined_colour,sample_array,ind);
					shot=0;
				}
				
			} while (defined_colour);
			
			uint16_t cut_array_size=cutArray(sample_array,cut_sample_array,ind);
			uint8_t most_element=getMostCommonElement(cut_sample_array,cut_array_size);
			usartTransmit(1);
			usartTransmit(most_element);
			
			uint16_t ang=chooseAngle(most_element);
			servoRotate(ang);
			startTimer2();

			
			//line_array[count]=most_element;
			//if(count<49) count++;
		}
		else //usartTransmitFloat(-1.0);
		usartTransmit(0);//black
		
		if(change_state){
			if(getServoState()==1){
			
				servoPush(FORWARD);
		
			}
			else if (getServoState()==2){
				
				servoPush(BACKWARD);
				TIMSK2&=~(1<<1);
			
			}
			
			change_state=0;
			
		}
		
	}
}


