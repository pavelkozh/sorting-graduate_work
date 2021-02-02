#include "i2clib.h"

/*Sending START-condition*/
void i2cStart(void){
	
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while ( !(TWCR & (1 << TWINT)) );

}

/*Writing 8 bits of data*/
void i2cWrite(uint8_t data){
	
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while ( !(TWCR & (1 << TWINT)) );
	
}

/*Reading 8 bits of data*/
uint8_t i2cRead(void){

	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while ( !(TWCR & (1 << TWINT)) );
	return TWDR;
	
}

/*Sending STOP-condition*/
void i2cStop(void){
	
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

}