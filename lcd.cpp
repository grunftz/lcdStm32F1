#include "lcd.h"                  // Device header

//-------------------------VARS-------------------------------------------
int ticks,i;								// smart count
//-------------------------END VARS---------------------------------------

//------------------------PROTOTYPES && other sutff-----------------------
static void write4bit(uint8_t val);
extern "C" {void TIM3_IRQHandler();} 

//---------------------END PROTPTYPs--------------------------------------

//-------------------------TIMER STUFF-------------------------------------------
void timer_init(){
	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;  //timer 4 enable GP Timer
	TIM3->PSC = 0; 						          //prescaller
	TIM3->ARR =72; 					          	//1MHz freq  period 1uS
	TIM3->CR1|=TIM_CR1_URS;				      //interrupt on overflow
	TIM3->DIER |=TIM_DIER_UIE;			    //Update interupt enabled
	TIM3->EGR |=TIM_EGR_UG;				      //update generation
	NVIC_EnableIRQ(TIM3_IRQn);		   		//enable TIM3 interupt number 30
}	
		
//----------------------------------END TIMERS----------------------------------


//-------------------------------------------
//LCD initialization
//-------------------------------------------


//******************************************
// @fn   	->lcd_init
//
// @expl	->lcd init 4  bit mode
//  D4->A0
//  D5->A1
//  D6->A2
//  D7->A3
//  RS->A4
//  EN->A5  
//
//@return	->void
//
//********************************************


void lcd_init()
{
	//timer init
		timer_init();
	//lcd init procedure
	//after power on wait 40ms
		delay_ms(40);
 	//GPIO configuration for D7-D4, RS and EN pins all outputs FAST mode
		RCC->APB2ENR |=RCC_APB2ENR_IOPAEN;
		for(int i=0;i<6;i++)
		{
			GPIOA->CRL |=(1<<(i*4));
			GPIOA->CRL |=(1<<((i*4)+1));
			GPIOA->CRL &=~(1<<((i*4)+2));
			GPIOA->CRL &=~(1<<((i*4)+3));
		}

	//send 00 then 0011
			GPIOA->ODR &=~(1<<RS);
			GPIOA->ODR &=~(1<<EN);
	//send 0011
			write4bit(0x03);
	//wait 5ms
			delay_ms(5);
 //send 0011
			write4bit(0x03);
	//wait 150us
			delay_ms(5);
	//send 0011
			write4bit(0x03);
	//send 0010
			write4bit(0x02);
	//send initialisation command
			sendLcd_cmd(LCD_2x16);
			delay_us(50);
 //lcd on cursor ON 0x0e or curssor off 0x0C
			sendLcd_cmd(LCD_ON_WITH_CURRSOR);
			delay_us(50);
	//clear LCD	
			lcd_clear();
	//entry mode set where we start the text on LCd
    sendLcd_cmd(LCD_INCAD);
		delay_us(50);
}


//-------------------------------------------
//LCD clear
//-------------------------------------------


//******************************************
// @fn   	->lcd_clear
//
// @expl	->clear LCD display
//
//@return	->void
//
//********************************************
void lcd_clear()
{
	sendLcd_cmd(LCD_CLEAR);
	delay_ms(3);
 }

 
//-------------------------------------------
//LCD send char
//-------------------------------------------
 
 
 
//******************************************
// @fn   	->sendLcd_char
//
// @expl	->clear LCD display
//parm[0] ->character
//@return	->void
//
//********************************************
void sendLcd_char(char data)
{

	//RW is connected to GND
	//first nible
		int data1=data>>4;
	//second nible
		int data2=data & 0x0F;
	//for data 	mode RS must be 1 and RW must be 0 for write operation
		GPIOA->ODR |=(1<<RS);
		write4bit(data1);
		write4bit(data2);

}



 //-------------------------------------------
//LCD set cursor
//-------------------------------------------
//******************************************
// @fn   	->setCursor
//
// @expl	->set dcursor diplay location 
//parm[0] ->row  1 or 2
//parm[1] ->column 1 to 16
//@return	->void
//
//********************************************


void lcd_setCursor(uint8_t row, uint8_t col)
{

	col--;
	switch(row)
	{
		case 1:
			sendLcd_cmd((col|=0x08));
		break;
		case 2:
			sendLcd_cmd((col|=0xC0));
		 break;
	}
}


void lcd_print(char *message)
{
	do
	{
		sendLcd_char((uint8_t)*message++);
	}
	while (*message !='\0');
}
 
//timer3 interrupt
void TIM3_IRQHandler(void)
{
	TIM3->SR &=~ TIM_SR_UIF;     //clear UIF
	ticks++;
}
//us delay
void delay_us(int us)
{
	TIM3->CR1 |=TIM_CR1_CEN;      //enable counter
	ticks=0;
	while(ticks<us);
	TIM3->CR1 &=~TIM_CR1_CEN;    //disable counter	
}
//ms delay
void delay_ms(int ms)
{
	delay_us(ms*1000);
}





//write data on D4, D5, D6, D7 data bits
void write4bit(uint8_t val)
{
	//dor DATA mode EN in on
		GPIOA->ODR|=(1<<EN);
		for(int i=0;i<=3;i++)
		{
				if((val>>i)&0x01)
					{
						GPIOA->ODR |=(1<<i);
					}
					else
					{
						GPIOA->ODR &=~(1<<i);
					}
		}
	//
	//enable puls
		GPIOA->ODR&=~(1<<EN);	
		if(GPIOA->ODR&GPIO_ODR_ODR3){;}
			delay_us(40);
			
}


//send cmd to LCD in 4 bit mode
void sendLcd_cmd(uint8_t cmd)
{
	//for cnd mode RS must be 0 and RW must be 0 for write operation
	GPIOA->ODR &=~(1<<RS);
	//RW is connected to GND
	//1 nible
	int cmd1=cmd>>4;
	write4bit(cmd1);
	//2 nibble
	int cmd2=cmd & 0x0F;
	write4bit(cmd2);

}


