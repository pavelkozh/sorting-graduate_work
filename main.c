/*
 * blinking.c
 *
 * Created: 27.09.2020 15:53:48
 * Author : User
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
unsigned char i=1;
ISR (TIMER1_COMPA_vect)
{
	if ((i>=1)&&(i<6))//green  2.5 sec
	{
		PORTC|=(1<<0);
	}
	
	if ((i>=6)&&(i<11))//green blinking  3 sec
	{
		PORTC^=(1<<0);
	}
	
    if ((i>=11)&&(i<16))//red  3 sec; off green
    {
	    PORTC&=~(1<<0);
	    PORTC|=(1<<2);
    }

    if ((i>=16)&&(i<21))//red+yellow  3 sec
    {
	    PORTC|=((1<<2)|(1<<1));
    }
    else if (i==21)  // off red+yellow, green on (0.5s)
    {
	    PORTC=0x01;
	    i=0;
    }	
	    i++;
		
}

int main(void)
{
    DDRC=0x07;
	PORTC=0;
	TCCR1A|=(0<<1)|(0<<0);      //CTC mode
	TCCR1B|=(0<<4)|(1<<3)|0x05;//CTC mode; prescale 1024
	TIMSK1|=(1<<1);//enable interrupt OCRA1
	
	sei();
    while (1) 
    {
		
		OCR1AH=0x0F;//   8000000 /(2*1024) = 3906 (0F42 in hex) - 0.5sec
		OCR1AL=0x42;     
    }
}

