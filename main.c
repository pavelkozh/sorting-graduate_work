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
#define BAUD_RATE 9600
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


int main(void)
{
	uint16_t rgb_array[3]={0,0,0};
	uint8_t colour_code=0;
	float hsv_array[3]={0,0,0};
	float *pointer_hsv=hsv_array;
	uint16_t *pointer_rgb=rgb_array;
	DDRC=0x07;
	TWBR|=(1<<TWBR5);// TWBR=32 (for 100 kHz i2c frequency)
	usartInit(UBRR_VALUE);
	sensorInit(pointer_init);

	while (1){
		
		rgb_array[0]=readColour(RDATAL_ADDR,RDATAH_ADDR);//red
		rgb_array[1]=readColour(GDATAL_ADDR,GDATAH_ADDR);//green		
		rgb_array[2]=readColour(BDATAL_ADDR,BDATAH_ADDR);//blue
			
			if (rgb2hsv(pointer_rgb,pointer_hsv)) {
				usartTransmit(1);//colour is defined
				colour_code=getColourCode(hsv_array[0]);
				usartTransmit(colour_code);
			}
			else usartTransmit(0);//colour is not defined

	}
}
