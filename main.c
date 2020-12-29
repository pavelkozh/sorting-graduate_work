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
#include <stdio.h>
#define SENS_ADDR 0x29 // RGB sensor's i2c address is 41
#define CDATAL_ADDR 0x14 //clear data h address
#define CDATAH_ADDR 0x15 //clear data l address
#define RDATAL_ADDR 0x16 //red data h address
#define RDATAH_ADDR 0x17 //red data l address
#define GDATAL_ADDR 0x18 //green data h address
#define GDATAH_ADDR 0x19 //green data l address
#define BDATAL_ADDR 0x1A //blue data h address
#define BDATAH_ADDR 0x1B //blue data l address

unsigned char INIT_SENSOR_ARRAY[10]={4,SENS_ADDR,0x03,0x01,0x00,0x00,0xFF,0xFF,0x09,0x0B};
unsigned char *POINTER_INIT=INIT_SENSOR_ARRAY;

void i2cStart(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));

}

void i2cWrite(unsigned char data){
	
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
}

unsigned char i2cRead(void){

	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));				
	return TWDR;
	
}

void i2cStop(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);

}

void selectRegister(unsigned char reg_addr,unsigned char mode){// mode 00 - Repeated byte, 01 - Auto-increment
	
	i2cWrite((1<<7)|(mode<<5)|reg_addr);
	
}

void sensorInit(unsigned char *ARRAY){

	for (int i=0;i<*(ARRAY+0);i++){
		
		i2cStart();
		i2cWrite(*(ARRAY+1)<<1);// 7 bit RGB sensor's address + W (0)
		i2cWrite((1<<7)|(*(ARRAY+2+i)));//select register
		i2cWrite(*(ARRAY+6+i));//write init value
		if(i==2) _delay_ms(3);
		i2cStop();
	}
	}

unsigned int readColour(unsigned char LOW_ADDR, unsigned char HIGH_ADDR){
	
	i2cStart();
	i2cWrite(SENS_ADDR<<1);// 7 bit RGB sensor's address + W (0)
	i2cWrite((1<<7)|LOW_ADDR);// DATAL byte
	i2cStop();
	
	i2cStart();
	i2cWrite((SENS_ADDR<<1)|(1<<0));// 7 bit RGB sensor's address + R (1)
	unsigned int LOW_DATA_VALUE=i2cRead();
	i2cStop();
	
	i2cStart();
	i2cWrite(SENS_ADDR<<1);// 7 bit RGB sensor's address + W (0)
	i2cWrite((1<<7)|HIGH_ADDR);// DATAH byte
	i2cStop();
	
	i2cStart();
	i2cWrite((SENS_ADDR<<1)|(1<<0));// 7 bit RGB sensor's address + R (1)
	unsigned int HIGH_DATA_VALUE=i2cRead();
    i2cStop();
	
	return((HIGH_DATA_VALUE<<8)|LOW_DATA_VALUE);
	
}

int main(void)
{
	unsigned int RED=0;
	DDRC=0x07;
	TWBR|=(1<<TWBR5);// TWBR=32 (for 100 kHz i2c frequency)
	sensorInit(POINTER_INIT);
	
	while (1){
     RED=readColour(RDATAL_ADDR,RDATAH_ADDR);
     if (RED<500) PORTC|=0x01;
	 _delay_ms(200);

	}
}