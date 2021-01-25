#include "sensorlib.h"

void sensorInit(uint8_t *init_array){
	
	TWBR |= (1 << TWBR5);// TWBR=32 (for 100 kHz i2c frequency)

	for (int i = 0; i < init_array[0]; i++){
		
		i2cStart();
		i2cWrite(init_array[1] << 1);// 7 bit RGB sensor's address + W (0)
		i2cWrite( (1 << 7) | init_array[2+i] );//select register
		i2cWrite( init_array[6+i] );//write init value
		if(i == 2) _delay_ms(3);
		i2cStop();
	}
}

uint16_t readColour(uint8_t low_addr, uint8_t high_addr){
	
	i2cStart();
	i2cWrite(SENS_ADDR << 1);// 7 bit RGB sensor's address + W (0)
	i2cWrite( (1 << 7) | low_addr );// DATAL byte
	i2cStop();
	
	i2cStart();
	i2cWrite( (SENS_ADDR << 1) | (1 << 0) );// 7 bit RGB sensor's address + R (1)
	uint8_t low_data_value = i2cRead();
	i2cStop();
	
	i2cStart();
	i2cWrite(SENS_ADDR << 1);// 7 bit RGB sensor's address + W (0)
	i2cWrite( (1 << 7) | high_addr );// DATAH byte
	i2cStop();
	
	i2cStart();
	i2cWrite( (SENS_ADDR << 1) | (1 << 0) );// 7 bit RGB sensor's address + R (1)
	uint8_t high_data_value = i2cRead();
	i2cStop();
	
	// PON Enable (AEN is enabled after every ADC cycle)
	
	i2cStart();
	i2cWrite( SENS_ADDR << 1 );// 7 bit RGB sensor's address + W (0)
	i2cWrite( (1<<7) | 0x00 );
	i2cWrite(0x01);
	i2cStop();
	
	_delay_ms(3);// delay for initialize after setting PON
	
	return( ( high_data_value << 8 ) | low_data_value );
}

uint8_t rgb2hsv(uint16_t* in_rgb_array, float* out_hsv_array){
	
	float max_value = 65535;
	float min_value = 0;
	float norm_max_value = 0;
	float norm_min_value = 0;
	float a = 255 / ( max_value - min_value );

    float r = ( in_rgb_array[0] - min_value ) * a;
    float g = ( in_rgb_array[1] - min_value ) * a;
    float b = ( in_rgb_array[2] - min_value ) * a;
    
    if( (r >= g) && (r >= b) ) norm_max_value = r;
    else if ( (g >= r) && (g >= b) ) norm_max_value = g;
    else if( (b >= r) && (b >= g) ) norm_max_value = b;
    
    if( (r <= g) && (r <= b) ) norm_min_value = r;
    else if ( (g <= r) && (g <= b) ) norm_min_value = g;
    else if( (b <= r) && (b <= g) ) norm_min_value = b;
    
	float range = 60 / ( norm_max_value - norm_min_value );
	
    if( norm_max_value == norm_min_value ) return 0; //colour not defined
	
	//V
    out_hsv_array[2] = norm_max_value;
    
    //S
    if( norm_max_value == 0 ) out_hsv_array[1] = 0;
    else out_hsv_array[1] = 1 - norm_min_value / norm_max_value;
    
    //H
    if( (norm_max_value == r) && (g >= b) )
    out_hsv_array[0] = (g - b) * range;
	
    if ( (norm_max_value == r) && (g < b) )
    out_hsv_array[0] = (g - b) * range + 360;
	
    if (norm_max_value == g)
    out_hsv_array[0] = (b - r) * range + 120;
	
    if (norm_max_value == b)
    out_hsv_array[0] = (r - g) * range + 240;
    
    return 1;//colour defined
    }

uint8_t getColourCode(float hue){
	
	if ( (hue >= 345) || (hue < 15) ) return 1;//Code of red colour
	else if ( (hue >= 15) && (hue < 30) ) return 2;//Code of orange colour
	else if ( (hue >= 30) && (hue < 90) ) return 3;//Code of yellow colour
	else if ( (hue >= 90) && (hue < 150) ) return 4;//Code of green colour
	else if ( (hue >= 150) && (hue < 210) ) return 5;//Code of light blue colour
	else if ( (hue >= 210) && (hue < 270) ) return 6;//Code of blue colour
	else if ( (hue >= 270) && (hue < 345) ) return 7;//Code of pink colour
}