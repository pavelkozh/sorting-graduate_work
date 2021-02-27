#include "servolib.h"

void servoInit(){
	
	DDRB|=(1<<1)|(1<<2);
	TCCR1A|=(1<<1)|(1<<7)|(1<<5);      //fast PWM mode; clear on compare match
	TCCR1B|=(1<<4)|(1<<3)|(1<<1)|(1<<0);//fast PWM mode; 64 prescale
	ICR1H=0x09;// period of PWM is 20 ms
	ICR1L=0xC4;
	
}

void timer2Init(void){
	
	TCCR2A|=(1<<1);      //CTC mode
	TCCR2B|=(1<<2)|(1<<1)|(1<<0); // prescale 1024
	OCR2A=0x9C;//   8000000/1024 * 20e-3 = 156 (9C in hex) - 20 ms
	
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

void servoPush(uint16_t direction){
	
	OCR1B=direction;// 80 - 90, 210 - 0
	
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