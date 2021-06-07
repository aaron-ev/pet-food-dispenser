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

#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define GPIO_SERVO_A0 GPIO_PIN_0

I2C_HandleTypeDef hi2c1;

void GPIO_Init(void);
void Error_Handler(void);

typedef enum
{
	FALSE,
	TRUE
}bool;

uint8_t button_enter;
uint8_t button_up;
uint8_t button_down;

bool screen_main = TRUE;
bool screen_settings = FALSE;

#ifdef __cplusplus
}
#endif

#endif
