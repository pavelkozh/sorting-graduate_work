/*
 * lcd_1.c
 *
 * Created: 04.10.2020 23:49:14
 * Author : User
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define lcd_address 0x27
#define RS 0
#define E 2


void i2c_start(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
}

void i2c_data(unsigned char data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}


void i2c_stop(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	
}

void LCD_cmd(unsigned char cmd) {

	unsigned char cmd_h=(cmd & 0xF0)|(1<<3);
	unsigned char cmd_l=((cmd & 0x0F)<<4)|(1<<3);

	cmd_h |= (1<<E);
	i2c_data(cmd_h);
	_delay_us(5);
	
	cmd_h &= ~(1<<E);
	i2c_data(cmd_h);
	_delay_ms(100);
	
	cmd_l |= (1<<E);
	i2c_data(cmd_l);
	_delay_us(5);
	
	cmd_l &= ~(1<<E);
	i2c_data(cmd_l);
	_delay_ms(100);
	
}

void LCD_init(void){
	
	LCD_cmd(0x33);
	LCD_cmd(0x32);
	LCD_cmd(0x28);//4-bit mode
	LCD_cmd(0x0D);//Enable display, blinking cursor
	LCD_cmd(0x06);//right shift of cursor
	LCD_cmd(0x01);//clear display
}
void LCD_data(unsigned char lcd_data){
	
	unsigned char lcd_data_h=(lcd_data & 0xF0)|(1<<RS)|(1<<3);
	unsigned char lcd_data_l=(((lcd_data & 0x0F)<<4))|(1<<RS)|(1<<3);

	lcd_data_h |= (1<<E);
	_delay_us(5);
	i2c_data(lcd_data_h);
	

	lcd_data_h &= ~(1<<E);
	_delay_ms(100);
	i2c_data(lcd_data_h);
	
	lcd_data_l |= (1<<E);_delay_ms(100);
	_delay_us(5);
	i2c_data(lcd_data_l);
	
	lcd_data_l &= ~(1<<E);
	_delay_ms(100);
	i2c_data(lcd_data_l);
	
}

void stroka(unsigned char *array){
	unsigned char i=0;
	while(array[i]!='\0'){
		LCD_data(array[i]);
		i++;
	}
}

void init_CGRAM_symb(void){
	
	LCD_cmd(0x40);// CGRAM address 0x00
	LCD_data(0x00);//       
	LCD_data(0x00);//      
	LCD_data(0x00);//      
	LCD_data(0x0A);//       * *
	LCD_data(0x1F);//      *****
	LCD_data(0x1F);//      *****
	LCD_data(0x0E);//       ***
	LCD_data(0x04);//        *
	
	
	LCD_data(0x1F);//     *****
	LCD_data(0x11);//     *   *
	LCD_data(0x11);//     *   *
	LCD_data(0x15);//     * * *
	LCD_data(0x15);//     * * *
	LCD_data(0x11);//     *   *
 	LCD_data(0x11);//     *   *
 	LCD_data(0x1F);//     *****
	
}

int main(void)
{
	unsigned char addr=(lcd_address<<1); //7 address bits + W bit (0)
    TWBR|=(1<<TWBR5);// TWBR=32 (for 100 kHz i2c frequency)
	i2c_start();
	i2c_data(addr);
	LCD_init();
	stroka("Symbol1:");// string 1 output
	LCD_data(0x00);// output CGRAM user's symbol 1
	LCD_cmd((1<<7)|0x40);// place cursor to the second line
	stroka("Symbol2:");// string 2 output
	LCD_data(0x01);// output CGRAM user's symbol 2
    i2c_stop();
	while (1)
	{
	}
}

