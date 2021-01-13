#include "sensorlib.h"

void sensorInit(uint8_t *init_array){

	for (int i=0;i<*(init_array+0);i++){
		
		i2cStart();
		i2cWrite(*(init_array+1)<<1);// 7 bit RGB sensor's address + W (0)
		i2cWrite((1<<7)|(*(init_array+2+i)));//select register
		i2cWrite(*(init_array+6+i));//write init value
		if(i==2) _delay_ms(3);
		i2cStop();
	}
}

uint16_t readColour(uint8_t low_addr, uint8_t high_addr){
	
	i2cStart();
	i2cWrite(SENS_ADDR<<1);// 7 bit RGB sensor's address + W (0)
	i2cWrite((1<<7)|low_addr);// DATAL byte
	i2cStop();
	
	i2cStart();
	i2cWrite((SENS_ADDR<<1)|(1<<0));// 7 bit RGB sensor's address + R (1)
	unsigned int low_data_value=i2cRead();
	i2cStop();
	
	i2cStart();
	i2cWrite(SENS_ADDR<<1);// 7 bit RGB sensor's address + W (0)
	i2cWrite((1<<7)|high_addr);// DATAH byte
	i2cStop();
	
	i2cStart();
	i2cWrite((SENS_ADDR<<1)|(1<<0));// 7 bit RGB sensor's address + R (1)
	unsigned int high_data_value=i2cRead();
	i2cStop();
	
	// PON Enable (AEN is enabled after every ADC cycle)
	
	i2cStart();
	i2cWrite(SENS_ADDR<<1);// 7 bit RGB sensor's address + W (0)
	i2cWrite((1<<7)|0x00);
	i2cWrite(0x01);
	i2cStop();
	
	_delay_ms(3);
	
	return((high_data_value<<8)|low_data_value);
	
}
