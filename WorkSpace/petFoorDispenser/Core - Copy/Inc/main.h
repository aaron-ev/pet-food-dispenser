
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "fonts.h"
#include "ssd1306.h"
// custom data types
typedef enum
{
	FALSE,
	TRUE
}bool;
// macros
#define SOURCE_BUTTON_ENTER   GPIO_PIN_0
#define SOURCE_BUTTON_DOWN    GPIO_PIN_2//1
#define SOURCE_BUTTON_UP	  GPIO_PIN_1//2
#define SOURCE_NOTHING		  50

#define ROW_BACK   	 0
#define ROW_TIME	 1
#define ROW_SPEED  	 2
#define ROW_CYCLES 	 3
#define ROW_FEED 	 4
#define ROW_SETTING  5
#define ROW_SERVING  6
#define ROW_CENTER	 7

//#define BUTTON_DISPENSE_PIN 	GPIO_PIN_1
#define BUTTON_ENTER_PIN 		GPIO_PIN_0
#define BUTTON_UP_PIN 		    GPIO_PIN_1
#define BUTTON_DOWN_PIN 		GPIO_PIN_2

#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO_SERVO_A0 GPIO_PIN_0

#define TRUE 1
#define FALSE 0

// global variables
TIM_HandleTypeDef tim6;
TIM_HandleTypeDef tim7;
I2C_HandleTypeDef hi2c1;
RTC_HandleTypeDef rtc;
bool flag_screen_main = FALSE;

/*
typedef enum
{

	int cycles[2];
	int speed[2];
	int back[2];
	int feed[2];
	int settings[2];
}arrow_t;
int cycles[2] = {10,10};
int speed[2] = {10,30};
int back[2] = {40,10};
int feed[2] = {10,21};
int settings[2] = {10,41};
arrow_t arrow = {{10,10},{10,30},{0,10},{10,21},{10,41}};
*/

int arrow[8][2] = {{33,20},{33,40},{33,20},{33,0},{33,21},{33,41},{10,40},{20,20}}; // back,time,speed,cycles,feed,setting,serving,center
int hourxy[3][2] = {{20,0},{55,0},{90,0}};//hour:minutes:seconds
int hourDot[2][2] = {{43,0},{78,0}};
int	arrow_row = ROW_FEED;

int itSource = SOURCE_NOTHING;
int times_to_serve = 1;
bool flag_GPIO_it = FALSE;
uint16_t button_pressed = SOURCE_NOTHING;
// extern variables
extern int servo_delay;

// display funtions
void display_screen_main();
void display_screen_settings(void);
void display_screen_cycles(void);
void display_screen_speed(void);
void display_hour(void);
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
void RTC_Init(void);
void tim6_Init(void);
void tim7_Init(void);
// error handler functions
void Error_Handler(void);
// RTC functions
void set_time(RTC_TimeTypeDef *time);

// enable and disable buttons
void enable_it_buttons(void);
void disable_it_buttons(void);

/*
//  @modulations
#define QPSK      0
#define QAM_16    1
#define QAM_64    2

typedef enum
{
    busy,
    done,
}status;

/*	function:
    input: @modulations
/*
void detector_writeConfReg(uint32_t mod);
void detector_writeMemIn(char *str);
uint32_t detector_readMemOut(void);
status detector_readStatus(void);
*/
#endif
