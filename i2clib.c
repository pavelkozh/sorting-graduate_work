#include "i2clib.h"

void i2cStart(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));

}

void i2cWrite(uint8_t data){
	
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
}

uint8_t i2cRead(void){

	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
	
}

void i2cStop(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);

}
