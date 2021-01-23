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
#include "i2clib.h"
#include "uartlib.h"
#include "sensorlib.h"
#define BAUD_RATE 19200
#define UBRR_VALUE 8000000/16/BAUD_RATE-1
#define SENS_ADDR 0x29 // RGB sensor's i2c address is 41
#define CDATAL_ADDR 0x14 //clear data h address
#define CDATAH_ADDR 0x15 //clear data l address
#define RDATAL_ADDR 0x16 //red data h address
#define RDATAH_ADDR 0x17 //red data l address
#define GDATAL_ADDR 0x18 //green data h address
#define GDATAH_ADDR 0x19 //green data l address
#define BDATAL_ADDR 0x1A //blue data h address
#define BDATAH_ADDR 0x1B //blue data l address

unsigned char init_sensor_array[10]={4,SENS_ADDR,0x03,0x01,0x00,0x00,0xFF,0xFF,0x01,0x03};
unsigned char *pointer_init=init_sensor_array;
uint16_t ind=0;//index of sample array's element
uint8_t colour_code=0;
uint8_t flag=0;


uint8_t getSingleMeasurement(uint16_t* rgb_array_pointer,float* hsv_array_pointer){
		
	rgb_array_pointer[0]=readColour(RDATAL_ADDR,RDATAH_ADDR);//red
	rgb_array_pointer[1]=readColour(GDATAL_ADDR,GDATAH_ADDR);//green
	rgb_array_pointer[2]=readColour(BDATAL_ADDR,BDATAH_ADDR);//blue
	if (rgb2hsv(rgb_array_pointer,hsv_array_pointer)) {
		colour_code=getColourCode(hsv_array_pointer[0]);
		return(1);//colour is defined
	}
	return(0); //colour is not defined
	
}

void getSampleArray(uint8_t is_colour_defined,uint8_t* output_sample_array){
PORTC=1;
	if(is_colour_defined){
		
		flag=1;
		output_sample_array[ind]=colour_code;
		ind+=1;
		if(ind>199) ind=199;
	}
	else {
		PORTC=0;
		if(flag==1){

			usartTransmitTwoBytes(ind); //transmit number of elements
			for(uint16_t i=0;i<ind;i++){	
				usartTransmit(output_sample_array[i]);
			}
			ind=0;
			flag=0;
		}
	}
	
}

int main(void)
{
	
	uint16_t rgb_array[3]={0,0,0};
	float hsv_array[3]={0,0,0};
	uint8_t sample_array[200]={0};//array for sample measurement
	float *pointer_hsv=hsv_array;
	uint16_t *pointer_rgb=rgb_array;
	uint8_t *pointer_sample_array=sample_array;
	DDRC=0x07;
	TWBR|=(1<<TWBR5);// TWBR=32 (for 100 kHz i2c frequency)
	usartInit(UBRR_VALUE);
	sensorInit(pointer_init);
	flag=0;
	while (1){
		

		uint8_t is_colour_defined=getSingleMeasurement(pointer_rgb,pointer_hsv);
		getSampleArray(is_colour_defined,pointer_sample_array);
		

		
			
			/*
			
             		rgb_array[0]=readColour(RDATAL_ADDR,RDATAH_ADDR);//red
             		rgb_array[1]=readColour(GDATAL_ADDR,GDATAH_ADDR);//green
             		rgb_array[2]=readColour(BDATAL_ADDR,BDATAH_ADDR);//blue
             		
             		if (rgb2hsv(pointer_rgb,pointer_hsv)) {
	             		usartTransmit(1);//colour is defined
	             		colour_code=getColourCode(hsv_array[0]);
	             		usartTransmit(colour_code);
             		}
             		else usartTransmit(0);//colour is not defined
             		*/
             		
             	    
	

	}
}
