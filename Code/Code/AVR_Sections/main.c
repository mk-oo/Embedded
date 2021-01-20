 #include "avr/io.h"
#include "Mcal/DIO/DIO_Prog.h"
#include "Mcal/DIO/DIO_reg.h"
#include "util/delay.h"
#include "Hal/LCD_interface.h"
#include "avr/interrupt.h"
#include "Mcal/DIO/DIO_Prog.h"
#include "Mcal/DIO/DIO_reg.h"
#include "LIB/BIT_MATH.h"
#include "LIB/STD_TYPES.h"


int addr[] = {0x3f, 0x30, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
int i;
int flag=3;
void Red(){
	LCD_WriteString("STOP");
	LCD_WriteExtraChar0(0,5);
		for(i=9;i>0;i--)
		{
			if (flag==1){
				DIO_SetPortValue(PORTA,addr[i]);
				DIO_SetPinValue(PORTB,PIN0,HIGH);
				
				_delay_ms(1000);
			}
			else
			break;	
		}
		flag=2;
		LCD_CLR();
		DIO_SetPinValue(PORTB,PIN0,LOW);	
}
void Yellow(){
	LCD_WriteString("Ready");
	LCD_WriteExtraChar0(0,5);
	for(i=3;i>0;i--)
	{
		if (flag==2){
			DIO_SetPortValue(PORTA,addr[i]);
			DIO_SetPinValue(PORTB,PIN1,HIGH);
			_delay_ms(1000);
		}
		else break;
		
	}
	flag=3;
	LCD_CLR();
	DIO_SetPinValue(PORTB,PIN1,LOW);
}
void Green(){
	LCD_WriteString("Go");
		for(i=9;i>0;i--)
		{
			if (flag==3){
				DIO_SetPortValue(PORTA,addr[i]);
				DIO_SetPinValue(PORTB,PIN2,HIGH);
				LCD_WriteExtraChar1(0,5);
				LCD_WriteExtraChar0(0,5);
				//_delay_ms(1000);
			}
			else break;
			
		}
		flag=1;
		LCD_CLR();
		DIO_SetPinValue(PORTB,PIN2,LOW);
}
int main(void)
{
	DIO_SetPortDirection(PORTA,0xFF);
	DIO_SetPortDirection(PORTB,0xFF);
	DIO_SetPinDirection(PORTC,PIN7,HIGH);//RS
	DIO_SetPinDirection(PORTC,PIN6,HIGH);//RW
	DIO_SetPinDirection(PORTC,PIN5,HIGH);//E
	//--------------------------------------------
	DIO_SetPortDirection(PORTD,0xFF);
	LCD_Init();
	
	SET_BIT(GICR,6);
	SET_BIT(MCUCR,1);
	sei();
   
   
   LCD_CLR();
   Gotoyx(0,0);
    while (1) 
    {
		Red();
		Yellow();
		Green();
    }
}

ISR(INT0_vect){	
	switch(flag){
		case 1:
		flag=2;
		break;
		
		case 2:
		flag=3;
		break;
		
		case 3:
		flag=1;
		break;
	}
}