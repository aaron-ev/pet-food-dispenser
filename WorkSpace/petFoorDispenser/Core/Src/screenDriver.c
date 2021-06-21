/*
 * screenDriver.c
 *
 *  Created on: Jun 21, 2021
 *      Author: aaron
 */


#include "lcd1602_i2c.h"

void screen_clear(void)
{
	lcd_clear();
}

void screen_send_line(char *str,uint16_t *xy)
{
	lcd_setCur(xy[0],xy[1]);
	lcd_writeString(str);
}
void screen_init(void)
{
	lcd_init();
}
