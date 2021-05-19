#include "servolib.h"

extern uint8_t timeout_rotate;
extern uint8_t timeout_forward_push;
//extern uint8_t timeout_backward_push;

void servoInit(void){
	
	DDRB|=(1<<1)|(1<<2);
	TCCR1A|=(1<<1)|(1<<7)|(1<<5);      //fast PWM mode; clear on compare match
	TCCR1B|=(1<<4)|(1<<3)|(1<<1)|(1<<0);//fast PWM mode; 64 prescale
	ICR1H=0x09;// period of PWM is 20 ms
	ICR1L=0xC4;
	OCR1A=75;//red
	OCR1B=250;//backward
}

void timer2Init(void){
	
	TCCR2A|=(1<<1);      //CTC mode
	TCCR2B|=(1<<2)|(1<<1)|(1<<0); // prescale 1024
	OCR2A=0x9C;//   8000000/1024 * 20e-3 = 156 (9C in hex) - 20 ms
	
}

void startTimer2(void){
	
	TCNT2=0;
	TIFR2|=(1<<1);//interrupt flag OCF2A is cleared
	TIMSK2|=(1<<1);//enable interrupt
	
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
		
		case 255:
		angle=OCR1A;
		EIFR|=(1<<1);
		EIMSK|=(1<<1);
		break;		
	}
	
	return angle;
	
}

void servoPush(uint16_t direction){
	
	OCR1B=direction;// 50 - 90, 250 - 0
	
}

void servoRotate(uint16_t angle){

	OCR1A=angle;
}

uint8_t getServoState(void){
	
	if( (timeout_rotate == 0) && (timeout_forward_push == 0) ) return 0;
	else if ( (timeout_rotate == 1) && (timeout_forward_push == 0) ) return 1;
	//else if ( (timeout_rotate == 1) && (timeout_forward_push == 1) && (timeout_backward_push == 0) ) return 2;

}

void updateServoState(void){
	
	switch (getServoState())
	{
		
		case 0:
		
			timeout_rotate=1;
			break;
		
		case 1:
		
			timeout_rotate=0;
			timeout_forward_push=0;
			TIMSK2&=~(1<<1);
			break;
		/*
		case 2:
		
			timeout_rotate=0;
			timeout_forward_push=0;
			timeout_backward_push=0;
			TIMSK2&=~(1<<1);
			break;
		*/
	}
}

void shiftArray(uint8_t *array, uint16_t size){

	for(uint8_t i=0;i<size-1;i++){
		
		array[i]=array[i+1];
		
	}
	
	array[size-1]=0;
	
}