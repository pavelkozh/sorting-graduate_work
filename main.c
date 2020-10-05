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
	
}

void check_flag(void){
	
	while (!(TWCR & (1<<TWINT)));
	
}

void i2c_data(unsigned char data){
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
}


void i2c_stop(void){
	
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
	
}

void LCD_cmd(unsigned char cmd) {
	//[]--------------------------------------------------[]
	//|  Íàçíà÷åíèå: çàïèñü êîäîâ â ðåãèñòð êîìàíä ÆÊÈ     |
	//|  Âõîäíûå ïàðàìåòðû: cmd - êîä êîìàíäû              |
	//[]--------------------------------------------------[]
	unsigned char cmd_h=cmd & 0xF0;
	unsigned char cmd_l=(cmd & 0x0F)<<4;

	cmd_h |= (1<<E);
	i2c_data(cmd_h);
	check_flag();
	
	cmd_h &= ~(1<<E);
	i2c_data(cmd_h);
	check_flag();
	
	cmd_l |= (1<<E);
	i2c_data(cmd_l);
	check_flag();
	
	cmd_l &= ~(1<<E);
	i2c_data(cmd_l);
	check_flag();
	
}

void LCD_init(void){
	
	LCD_cmd(0x33);
	LCD_cmd(0x32);
	LCD_cmd(0x28);//Âûáîð 4-áèòíîãî ðåæèìà
	LCD_cmd(0x0D);//Âêëþ÷èòü ýêðàí, ìèãàþùèé êóðñîð
	LCD_cmd(0x01);//Î÷èñòèòü ýêðàí
	LCD_cmd(0x06);//Àâòîñäâèã êóðñîðà âïðàâî
}

void LCD_data(unsigned char lcd_data){
	
	unsigned char lcd_data_h=(lcd_data & 0xF0)|(1<<RS);
	unsigned char lcd_data_l=(((lcd_data & 0x0F)<<4))|(1<<RS);

	lcd_data_h |= (1<<E);
	i2c_data(lcd_data_h);
	check_flag();
	
	lcd_data_h &= ~(1<<E);
	i2c_data(lcd_data_h);
	check_flag();
	
	lcd_data_l |= (1<<E);
	i2c_data(lcd_data_l);
	check_flag();
	
	lcd_data_l &= ~(1<<E);
	i2c_data(lcd_data_l);
	check_flag();
	
}

void string(unsigned char *array){
	unsigned char i=0;
	while(array[i]!='\0'){
		LCD_data(array[i]);
		i++;
	}
}

int main(void)
{
	unsigned char addr=(lcd_address<<1); //7 address bits + W bit (0)
    TWBR|=(1<<TWBR5);// TWBR=32 (for 100 kHz i2c frequency)
	i2c_start();
	check_flag();
	i2c_data(addr);
	check_flag();
	LCD_init();
	LCD_cmd((1<<7)|0x42);// ïåðåíîñ êóðñîðà íà 2 äåëåíèÿ âïðàâî
	LCD_data(0x33);// âûâîä ñèìâîëà '3'
    i2c_stop();
	while (1)
	{
	}
}

