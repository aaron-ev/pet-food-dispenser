/*
*       Author: Aarón Escoboza Villegas
*       Github: aaron-ev
*
**/


#ifndef LCD1602_I2C_H
#define LCD1602_I2C_H
#include "stm32f4xx_hal.h"
// change here your handler and lcd address
extern I2C_HandleTypeDef hi2c1; // change I2C handler based on the I2C peripheral used
#define LCD_ADDRESS 0x4e  // Changed based on your settings

// macro definition

#define MASK_UPPER_NIBBLE 0xf0
#define LCD_DELAY_100ms   100
// @commands

#define  CMD_4BITMODE 		0x28
#define  CMD_DISPLAY_OFF 	0x08
#define  CMD_CLEAR			0x01
// function prototypes

/*
*   function name: lcd_writeCMD(char cmd);
*     description: send a command to lcd
*           input: command to send refers <this command can be @ref commands >
*/
void lcd_writeCMD(uint8_t cmd);

/*
*   function name: lcd_init(void);
*     description: initialize lcd screen
*
*/
void lcd_init(void);
#endif
