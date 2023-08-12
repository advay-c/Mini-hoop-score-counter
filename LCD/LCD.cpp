#include "Arduino.h"
#include "LCD.h"
#include<avr/io.h>

	#define BIT(x)  (1 << (x))
	#define CHECKBIT(x,b) (x&b)   //Checks bit status
	#define SETBIT(x,b) x|=b;   //Sets the particular bit
	#define CLEARBIT(x,b) x&=~b;    //Sets the particular bit
	#define TOGGLEBIT(x,b) x^=b;    //Toggles the particular bit

	#define LINE1	0x80
	#define LINE2	0xC0
	 
	char save=0;




void LCD::init(void)	//init the lcd
{	
	save=PORTC;
	DDRC=0xFF;
	//DDRB=0xFF;
	WaitMs(20);
	CLEARBIT(PORTC,BIT(0)+BIT(1)+BIT(2));
	PORTC=0x00;
	cmd(0x01);
	cmd(0x0F);
	cmd(0x02);
	cmd(LINE1);
	
}

void LCD::toggleE(void)
{
SETBIT(PORTC,BIT(2));
_delay_ms(1);
CLEARBIT(PORTC,BIT(2));
_delay_ms(1);
}

void LCD::dely_ns(char tim)
{
	for(unsigned char j=0;j<tim;j++)
	{
		for(unsigned char k=0;k<10;k++)
		{};
	}
}

void LCD::cmd(char cmd)
{
	CLEARBIT(PORTC,BIT(0)+BIT(1)+BIT(2));
	PORTC=((cmd&0xF0));			
	toggleE();
	PORTC=(((cmd&(0x0F))*16));	
	toggleE();
}


void LCD::dispchar(unsigned char single)
{
	CLEARBIT(PORTC,BIT(0)+BIT(2));
	
	PORTC=(single&0xF0)|0x01;						
	toggleE();
	PORTC=((single&(0x0F))*16)|0x01;
	toggleE();
}

void LCD::string(char str[32])
{
	unsigned char k=0;
	
	CLEARBIT(PORTC,BIT(0)+BIT(2));
	while(str[k]!='\0')	//Till null character is encountered
		{
			if(k==16)
				cmd(LINE2);
			PORTC=((str[k])&0xF0)|0x01;
			toggleE();
			PORTC=((str[k]&(0x0F))*16)|0x01;
			toggleE();
			k++;
		}
}


void LCD::showvalue(int num)	//prints the decimal 3digit value of num
{
	int R=0,H=0,T=0,O=0;
	R = num/1000;
	H= (num - (R*1000))/100;
	T=((num - (R*1000))- (H*100))/10;
	O=(((num - (R*1000))- (H*100)) - (T*10));
	dispchar(R+48);
	dispchar(H+48);
	dispchar(T+48);
	dispchar(O+48);
}

void LCD::line1(char pos)
{
	cmd(LINE1+pos);
}

void LCD::line2(char pos)
{
	cmd(LINE2+pos);
}
		

void LCD::exit(void)
{
	PORTC=save;
}

/* waits (pauses) for ms milliseconds (assumes clock at 16MHz) */
void LCD::WaitMs(int ms)
{
	int i;

	while (ms-- > 0)
	{
		/* 16380 (16k) clock cycles for 1ms; each time through loop
		   is 5 cycles (for loop control + nop) */
		for (i = 0; i < 3276; ++i)
			asm("nop");
	}
}
