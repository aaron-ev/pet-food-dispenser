
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

// custom data types
typedef enum
{
	FALSE,
	TRUE
}bool;
// macros
#define SOURCE_BUTTON_ENTER   GPIO_PIN_10
#define SOURCE_BUTTON_DOWN    GPIO_PIN_2//1
#define SOURCE_BUTTON_UP	  GPIO_PIN_3//2
#define SOURCE_NOTHING		  4

#define ROW_BACK   	 0
#define ROW_SPEED  	 1
#define ROW_CYCLES 	 2
#define ROW_FEED 	 3
#define ROW_SETTING  4

#define BUTTON_DISPENSE_PIN 	GPIO_PIN_1
#define BUTTON_ENTER_PIN 		GPIO_PIN_10
#define BUTTON_UP_PIN 		    GPIO_PIN_3
#define BUTTON_DOWN_PIN 		GPIO_PIN_2

#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO_SERVO_A0 GPIO_PIN_0

#define TRUE 1
#define FALSE 0

// global variables
TIM_HandleTypeDef tim6;
I2C_HandleTypeDef hi2c1;
int arrow[5][2] = {{0,9},{1,0},{0,0},{1,0},{1,7}}; // back,speed,cycles,feed,setting
int row = ROW_FEED;
int itSource = SOURCE_NOTHING;
int times_to_serve = 1;
bool flag_GPIO_it = FALSE;
uint16_t button_pressed;
// extern variables
extern int servo_delay;

// display funtions
void display_screen_main();
void display_screen_settings(void);
void display_screen_cycles(void);
void display_screen_speed(void);
// screen functions
void screen_main(void);
void screen_cycles(void);
void screen_speed(void);
void screen_settings(void);
// dispense functions
void dispense(void);
// init functions
void GPIO_Init(void);
void SystemClock_Config(void);
static void MX_I2C1_Init(void);
// error handler functions
void Error_Handler(void);

#endif
