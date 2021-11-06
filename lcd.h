#ifndef lcd_h
#define lcd_h

#include "stm32f10x.h"                  // Device header
/***************************************************
*
*GENERIC MACROS
*
****************************************************/

//-------------4 BIT mode--------------------------
# define D4 0x00
# define D5 0x01
# define D6 0x02
# define D7 0x03

//--------------Cntrl Bits-----------------------

#define RS 0x04
#define EN 0x05   

//--------------LCD commands-----------------------

#define LCD_2x16 0x28
#define LCD_ON_WITH_CURRSOR 0x0E
#define LCD_ON_NO_CURRSOR 0x0E
#define LCD_INCAD 0x06

#define LCD_CLEAR 0x01
#define LCD_GOHOME 0x02



/***************************************************
*
*API's for LCD
*
****************************************************/

void lcd_init();
void lcd_clear();
void sendLcd_cmd(uint8_t cmd);
void lcd_setCursor(uint8_t row, uint8_t col);
void lcd_print(char *message);
void sendLcd_char(char data);

void delay_ms(int ms);
void delay_us(int us);

#endif
