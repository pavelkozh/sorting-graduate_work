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
#define RGB_addr 0x29 // RGB sensor's i2c address is 41
#define repeated 0
#define Auto_inc 1 

void i2c_start(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));

}

void i2c_write(unsigned char data){
	
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
}

unsigned char i2c_read(void){

	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));				
	return(TWDR);
	
}



void i2c_stop(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));

}

void select_reg(unsigned char reg_addr,unsigned char mode){// mode 00 - Repeated byte, 01 - Auto-increment
	
	i2c_write((1<<7)|(mode<<5)|reg_addr);
	
}

void RGB_init(void){
	
	i2c_start();
	i2c_write(RGB_addr<<1);// 7 bit RGB sensor's address + W (0)
	select_reg(0x03,repeated);//Wait Time Register
	i2c_write(0xFF);// Wait time 2.4 ms

	i2c_start();
	i2c_write(RGB_addr<<1);// 7 bit RGB sensor's address + W (0)
	select_reg(0x01,repeated);//RGBC Timing Register
	i2c_write(0xFF);//integration time is 2.4 ms
	
	i2c_start();
	i2c_write(RGB_addr<<1);// 7 bit RGB sensor's address + W (0)
	select_reg(0x00,repeated);//Enable Register
	i2c_write(0x09);//Power on, Wait enable
	
	_delay_ms(3);//delay for RGBC initialization
	
	i2c_start();
	i2c_write(RGB_addr<<1);// 7 bit RGB sensor's address + W (0)
	select_reg(0x00,repeated);//Enable Register
	i2c_write(0x0B);//RGBC enable
	
}

unsigned int read_clear(){
	
	i2c_start();
	i2c_write(RGB_addr<<1);// 7 bit RGB sensor's address + W (0)
	select_reg(0x14,repeated);//CDATAL byte
	i2c_start();
	i2c_write((RGB_addr<<1)|(1<<0));// 7 bit RGB sensor's address + R (1)
	unsigned int clear_data_l=i2c_read();
	

	
	i2c_start();
	i2c_write(RGB_addr<<1);// 7 bit RGB sensor's address + W (0)
	select_reg(0x15,repeated);//CDATAH byte
	i2c_start();
	i2c_write((RGB_addr<<1)|(1<<0));// 7 bit RGB sensor's address + R (1)
	unsigned int clear_data_h=i2c_read();


	return((clear_data_h<<8)|clear_data_l);
	
}

int main(void)
{
	unsigned int red=0,green=0,blue=0,clear=0;
	DDRC=0x01;
	TWBR|=(1<<TWBR5);// TWBR=32 (for 100 kHz i2c frequency)
	RGB_init();
	clear=read_clear();
	if(clear<20)
	PORTC=1;
	else PORTC=0;
	i2c_stop();

	while (1){
		
	}
}