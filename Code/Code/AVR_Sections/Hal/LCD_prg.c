/*
 * LCD_prg.c
 *
 * Created: 12/2/2020 7:23:09 AM
 *  Author: bahaa
 */ 
#include "../LIB/STD_TYPES.h"
#include "util/delay.h"
#include "../Mcal/DIO/DIO_Prog.h"
#include "LCD_interface.h"
#include "LCD_Extra.h"

#define lcd_Clear 0x01 
#define lcd_Home 0x02
#define lcd_EntryMode 0x06
#define lcd_DisplayOff 0x08
#define lcd_DisplayOn 0x0C
#define lcd_FunctionReset 0x30
#define lcd_FunctionSet8bit 0x38
#define lcd_SetCursor 0x80



void LCD_Init(void){
	_delay_ms(30);
	LCD_SendCommand(lcd_Home);
	_delay_ms(15);
	LCD_SendCommand(lcd_FunctionSet8bit);
	_delay_ms(1);
	LCD_SendCommand(lcd_DisplayOn);
	_delay_ms(1);
	LCD_SendCommand(lcd_Clear);
	_delay_ms(1);
	LCD_SendCommand(lcd_EntryMode);
	_delay_ms(2);
}
void LCD_SendCommand(u8 CmdCpy){
	//RS -> C7, RW -> C6, E -> C5
	//write  rw -> low
	//command rs -> low
	DIO_SetPinValue(PORTD,PIN7,LOW);
	DIO_SetPinValue(PORTD,PIN6,LOW);
	DIO_SetPinValue(PORTD,PIN5,HIGH);// make E high to read command
	DIO_SetPortValue(PORTC,CmdCpy);
				_delay_ms(10);
	DIO_SetPinValue(PORTD,PIN5,LOW);// make E low to save command
	_delay_ms(5);
}
void LCD_WriteChar(u8 DataCpy){
		//RS -> C7, RW -> C6, E -> C5
			//write  rw -> low
			//data rs -> high
		DIO_SetPinValue(PORTD,PIN7,HIGH);
		DIO_SetPinValue(PORTD,PIN6,LOW);
		DIO_SetPortValue(PORTC,DataCpy);
		DIO_SetPinValue(PORTD,PIN5,HIGH);// make E high to read data
		_delay_ms(10);
				DIO_SetPinValue(PORTD,PIN5,LOW);// make E low to save data

}
void LCD_WriteString(u8 *StrCpy){
	u8 Index =0;
	while (StrCpy[Index]!='\0')
	{
		LCD_WriteChar(StrCpy[Index++]);
		_delay_ms(2);
	}
}

void Gotoyx(u8 Y,u8 X){
	if(X>=0 && X<16){
		
		switch (Y)
		{
			case 0:
			LCD_SendCommand(X+0x80);//goto 0x80 position col = 0 line 1
			break;
			case 1:
			LCD_SendCommand(X+0xC0);//goto 0xC0 position col= 0 line 2
			break;
			default:
			/* Your code here */
			break;
		}
	}
}
void LCD_CLR(){
	LCD_SendCommand(lcd_Clear);
}
void LCD_WriteExtraChar0(u8 Y,u8 X){
	u8 I1,I2;
	LCD_SendCommand(64);//write to cg ram
	for (I1=0;I1<64;I1++)
	{
		LCD_WriteChar(ExtraChar[I1]);
	}
	LCD_SendCommand(128);//access ddram
	Gotoyx(Y,X);
	
	for (I2=0;I2<1;I2++)
	{
		LCD_WriteChar(I2);//print asci code data from cgram 0..7 -> ddram
		_delay_ms(5);
	}
	
}


void LCD_WriteExtraChar1(u8 Y,u8 X){
	u8 I1,I2;
	LCD_SendCommand(64);//write to cg ram
	for (I1=0;I1<64;I1++)
	{
		LCD_WriteChar(ExtraChar[I1]);
	}
	LCD_SendCommand(128);//access ddram
	Gotoyx(Y,X);
	for (I2=1;I2<2;I2++)
	{LCD_WriteChar(I2);//print asci code data from cgram 0..7 -> ddram
		_delay_ms(5);
	}
	
}

