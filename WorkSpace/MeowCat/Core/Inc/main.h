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

I2C_HandleTypeDef hi2c1;

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif
