#include "sensorlib.h"

uint8_t flag=1;
static float max_value,min_value,a;

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

	if(flag==1){
		
		     max_value = 65535;
		     min_value = 0;
		     a = 255 / (max_value - min_value);
			flag=0;
	}


	float norm_max_value = 0;
	float norm_min_value = 0;

    float r = ( in_rgb_array[0] - min_value ) * a;
    float g = ( in_rgb_array[1] - min_value ) * a;
    float b = ( in_rgb_array[2] - min_value ) * a;
    
    if( (r >= g) && (r >= b) ) norm_max_value = r;
    else if ( (g >= r) && (g >= b) ) norm_max_value = g;
    else if( (b >= r) && (b >= g) ) norm_max_value = b;
    
    if( (r <= g) && (r <= b) ) norm_min_value = r;
    else if ( (g <= r) && (g <= b) ) norm_min_value = g;
    else if( (b <= r) && (b <= g) ) norm_min_value = b;
	
    if( norm_max_value == norm_min_value ) return 0; //colour not defined
	
	//V
    out_hsv_array[2] = norm_max_value;
    
    //S
    if( norm_max_value == 0 ) out_hsv_array[1] = 0;
    else out_hsv_array[1] = 1 - norm_min_value / norm_max_value;
    
    //H
	float range = 60 / ( norm_max_value - norm_min_value );
	
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

uint16_t cutArray(uint8_t *input_array,uint8_t *output_array, uint16_t num_of_elements){
	
	uint16_t low_edge = num_of_elements*0.1;
	uint16_t high_edge = num_of_elements*0.9;
	uint16_t out_count_elements = high_edge-low_edge;
	for(uint16_t j = 0; j < out_count_elements; j++){
		output_array[j] = input_array[j+low_edge];
	}
	return out_count_elements;
}

uint8_t getSingleMeasurement(uint16_t* rgb_array_pointer, float* hsv_array_pointer){
	
	rgb_array_pointer[0] = readColour(RDATAL_ADDR, RDATAH_ADDR);//red
	rgb_array_pointer[1] = readColour(GDATAL_ADDR, GDATAH_ADDR);//green
	rgb_array_pointer[2] = readColour(BDATAL_ADDR, BDATAH_ADDR);//blue
	if ( rgb2hsv(rgb_array_pointer, hsv_array_pointer) ) {
		uint8_t colour_code = getColourCode(hsv_array_pointer[0]);
		return(colour_code);//return colour code
	}
	return(0); //colour is not defined (black)
	
}

uint8_t getMostCommonElement(uint8_t *array, uint16_t size_of_array){
	
	uint8_t colour_array[7]={0};//array of elements containing the frequency of each colour
	uint8_t var=0;//secondary variable for sorting colour_array
	uint8_t sec_array[7]={0}; // copy of filled colour_array
	
	for (uint16_t i = 0; i < size_of_array; i++){// filling the colour_array
		/*
		if(array[i] == 1) colour_array[0]++;
		else if(array[i] == 2) colour_array[1]++;
		else if(array[i] == 3) colour_array[2]++;
		else if(array[i] == 4) colour_array[3]++;
		else if(array[i] == 5) colour_array[4]++;
		else if(array[i] == 6) colour_array[5]++;
		else if(array[i] == 7) colour_array[6]++;
		*/
		switch (array[i])
		{
		case 1:
			colour_array[0]++;
			break;
		case 2:
			colour_array[1]++;
			break;
		case 3:
			colour_array[2]++;
			break;
		case 4:
			colour_array[3]++;
			break;
		case 5:
			colour_array[4]++;
			break;
		case 6:
			colour_array[5]++;
			break;
		case 7:
			colour_array[6]++;
			break;
		}
	}
	
	for (uint8_t i = 0; i < 7; i++){//copying colour_array
		
		sec_array[i] = colour_array[i];
	}
	
	for(uint8_t j = 0; j < 6; j++){// max element will move to the end of the sec_array
		
		if(sec_array[j] > sec_array[j+1]){
			
			var = sec_array[j+1];
			sec_array[j+1] = sec_array[j];
			sec_array[j] = var;
		}
	}
	
	for (uint8_t i = 0; i < 7;i++){
		
		if(colour_array[i] == sec_array[6]) return (i+1);
	}
	
}

uint16_t getSampleArray(uint8_t defined_colour, uint8_t* sample_array, uint16_t elem_index){

		sample_array[elem_index] = defined_colour;
		elem_index+=1;
		if(elem_index > 199) elem_index = 199;
		return elem_index;	
}