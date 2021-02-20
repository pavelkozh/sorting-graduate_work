#include "servolib.h"

void servoInit(){
	
	DDRB|=2;
	TCCR1A|=(1<<1)|(1<<7);      //fast PWM mode; clear on compare match
	TCCR1B|=(1<<4)|(1<<3)|(1<<1)|(1<<0);//fast PWM mode; 64 prescale
	//TIMSK1|=(1<<1);//enable interrupt OCRA1
	ICR1H=0x09;
	ICR1L=0xC4;
	
}

uint16_t chooseAngle(uint8_t colour_code){
	
	uint16_t angle=0;
	
	switch (colour_code)
	{
		case 1:
		angle=75;//0 degrees
		break;
		
		case 2:
		angle=137;//45 degrees
		break;

		case 3:
		angle=200;//90 degrees
		break;

		case 4:
		angle=260;//135 degrees
		break;

		case 5:
		angle=320;//180 degrees
		break;
		
	}
	
	return angle;
	
}

void servoRotate(uint16_t angle){

	OCR1A=angle;
}

void shiftArray(uint8_t *array, uint16_t size){

	for(uint8_t i=0;i<size-1;i++){
		
		array[i]=array[i+1];
		
	}
	
	array[size-1]=0;
	
}