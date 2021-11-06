#include "stm32f10x.h"                  // Device header
#include "lcd.h"                  // Device header
#include <stdio.h>
uint16_t j;
char buffer[10];
int main(void)
{
	lcd_init();
	lcd_print("LCD 2X16 DRIVER");
	while(1)
	{
	
		lcd_setCursor(2,6);
		j++;
		snprintf(buffer, 10, "%d", j);
		lcd_print(buffer);
		delay_ms(1000);	
			
		
	}
}
