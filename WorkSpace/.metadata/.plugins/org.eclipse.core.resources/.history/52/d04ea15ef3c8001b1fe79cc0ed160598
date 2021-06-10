/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
 */

#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO_SERVO_A0 GPIO_PIN_0

// Pin settings

#define BUTTON_DISPENSE_PIN 	GPIO_PIN_1
#define BUTTON_ENTER_PIN 		GPIO_PIN_10
#define BUTTON_UP_PIN 		    GPIO_PIN_3
#define BUTTON_DOWN_PIN 		GPIO_PIN_2


I2C_HandleTypeDef hi2c1;

void GPIO_Init(void);
void Error_Handler(void);


typedef enum
{
	FALSE,
	TRUE
}bool;

bool button_enter = FALSE;
bool button_dispense = 0;
int8_t button_up = 0;
int8_t button_down = 0;
int8_t sum_up_down = 0;

bool screen_main = TRUE;
bool flag_GPIO_it = FALSE;
bool flag_button = FALSE;
void SystemClock_Config(void);
static void MX_I2C1_Init(void);

#ifdef __cplusplus
}
#endif

#endif
