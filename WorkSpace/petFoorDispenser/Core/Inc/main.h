
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "lcd1602_i2c.h"

//#include "lcd_i2c.h"
#include "screenDriver.h"
#include "servo.h"
#include "fonts.h"
#include "ssd1306.h"
// custom data types
typedef enum
{
	FALSE,
	TRUE
}bool;
// macros
#define LCD_2X16 1
//#define OLED 0
#define SOURCE_BUTTON_ENTER   GPIO_PIN_0
#define SOURCE_BUTTON_DOWN    GPIO_PIN_2
#define SOURCE_BUTTON_UP	  GPIO_PIN_1
#define SOURCE_NOTHING		  50


#define ROW_SPEED  	 3
#define ROW_TIME	 2
#define ROW_ALARMS 	 1
#define ROW_BACK   	 0

#define ROW_ALARM1 	 3
#define ROW_ALARM2 	 2
#define ROW_ALARM3 	 1

//#define ROW_CYCLES 	 4
#define ROW_FEED 	 4
#define ROW_SETTING  5
#define ROW_SERVING  6
#define ROW_CENTER	 7


#define BUTTON_ENTER_PIN 		GPIO_PIN_0
#define BUTTON_UP_PIN 		    GPIO_PIN_1
#define BUTTON_DOWN_PIN 		GPIO_PIN_2

#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO_SERVO_A0 GPIO_PIN_0

#define TRUE 1
#define FALSE 0

//p
// global variables
TIM_HandleTypeDef tim6;
TIM_HandleTypeDef tim7;
I2C_HandleTypeDef hi2c1;
RTC_HandleTypeDef rtc;


#ifdef OLED
	#define MENU_FEED  	  	{33,21}//{33,21}
	#define MENU_SETTINGS  	{33,41}
	#define MENU_SPEED     	{33,0}
	#define MENU_TIME      	{33}
	#define MENU_ALARMS    	{33,40}
	#define MENU_BACK      	{33,20}
	#define MENU_CENTER      {20,20}
	#define MENU_SERVING     {10,40}


	#define ARROW_FEED  	  	{11,21}//{33,21}
	#define ARROW_SETTINGS  	{11,41}
	#define ARROW_SPEED     	{11,0}
	#define ARROW_TIME      	{11}
	#define ARROW_ALARMS    	{11,40}
	#define ARROW_BACK      	{11,20}
	#define ARROW_CENTER      {20,20}
	#define ARROW_SERVING     {10,40}

	//#define ARROW_ALARM1      {33,0}
	//#define ARROW_ALARM2      {33,20}
	//#define ARROW_ALARM3      {33,40}

	#define XY_HOUR 			{20,0}
	#define XY_MINUTES 			{55,0}
	#define XY_SECONDS 			{55,0}
#endif

#ifdef LCD_2X16
	//Screen main
	#define MENU_FEED  	  	1,2,1,0
	#define MENU_SETTINGS  	1,8,1,6
	//screen settings
	#define MENU_SPEED     	0,2,0,0
	#define MENU_TIME      	1,2,1,0
	#define MENU_ALARMS    	0,10,0,8
	#define MENU_BACK      	1,10,1,8
	#define MENU_SERVING    {0,3}

	//screen speed
	#define MENU_SPEED_CENTER {0,3}
    #define MENU_SPEED_VALUE  {0,10}
	#define MENU_CENTER       {0,4}
	//screen alarms
	#define MENU_ALARM1		  0,2,0,0
	#define MENU_ALARM2		  1,2,1,0
	#define MENU_ALARM3		  0,10,0,8
	//time
	#define XY_HOUR_ONE_DIGIT   {0,6}
	#define XY_HOUR_TWO_DIGITS  {0,5}
	#define XY_COLON 			{0,7}
	#define XY_MINUTES 			{0,9}
    #define XY_MINUTES_ZERO	    {0,8}

#endif
//interrupt priorities

#define PRIORITY_BUZZER				2
#define PRIORIT_DISPLAY_UPDATE		3
#define PRIORITY_BUZZER_SOUND		2
#define PRIORITY_BUTTON		     	2
#define PRIORITY_BUTTON_DEBAUNCE	2
#define PRIORITY_ALARM				2

#define NUM_ALARMS_USED 			3
#define NO_ALARM_PENDING 			NUM_ALARMS_USED + 1

typedef struct
{
	uint16_t screenxy[2];
	uint16_t arrowxy[2];
}menu_a; // menu with arrow

typedef struct
{
	uint16_t screenxy[2];
}menu_e; // menu element

typedef struct
{
	menu_a feed;
	menu_a settings;
	menu_a speed;
	menu_a time;
	menu_a alarms;
	menu_a back;
	menu_a alarm1;
	menu_a alarm2;
	menu_a alarm3;
	menu_e serving;
	menu_e center;
	menu_e speed_center;
	menu_e speed_value;
}menu_s;

menu_s menu = { MENU_FEED,MENU_SETTINGS,
				MENU_SPEED,MENU_TIME,
				MENU_ALARMS,MENU_BACK,
				MENU_ALARM1,MENU_ALARM2,
				MENU_ALARM3,MENU_SERVING,
				MENU_CENTER,MENU_SPEED_CENTER,
				MENU_SPEED_VALUE
			  };

typedef struct
{
	uint16_t hour_one_digit[2];
	uint16_t hour_two_digits[2];
	uint16_t colon[2];
	uint16_t minutes[2];
	uint16_t minutes_zero[2];
}time_s;

typedef enum
{
	ALARM1,
	ALARM2,
	ALARM3,
	ALARM_NONE,
}alarmNumber;
typedef struct
{
	uint8_t 	hour;
	uint8_t 	minutes;
	bool		active;
	bool		alarmA;
	uint8_t     acumValue;
	bool 		pending;
	alarmNumber number;
}alarmTime;


alarmTime alarms[NUM_ALARMS_USED];

time_s timexy = {XY_HOUR_ONE_DIGIT,XY_HOUR_TWO_DIGITS,XY_COLON,XY_MINUTES,XY_MINUTES_ZERO};

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
void display_screen_alarms(void);
void display_screen_time(void);
void display_hour(void);
void display_screen_alarmSelected(alarmNumber alarmx);

void screen_main(void);
void screen_cycles(void);
void screen_speed(void);
void screen_settings(void);
void screen_alarms(void);
void screen_time(void);
void screen_alarmSelected(alarmNumber alarmNumber);

// dispense functions
void dispense(void);

// init functions
void GPIO_init(void);
void SystemClock_Config(void);
static void MX_I2C1_Init(void);
void RTC_Init(void);
void tim3buzzer_init(void);
void tim6BuzzerSound_init(void);
void tim7DisplayUpdate_init(void);

// error handler functions
void Error_Handler(void);

// RTC functions
void set_time(RTC_TimeTypeDef *time);
void update_time(uint8_t t,uint8_t m);

// enable and disable buttons
void enable_it_buttons(void);
void disable_it_buttons(void);
void clear_arrowSettings(void);
void clear_arrowAlarms(void);

//buzzer functions
void buzzer_init(void);
#endif
