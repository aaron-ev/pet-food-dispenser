/*
* Description : Header file to servo.c
* Author: Aar?n Escoboza Villegas
*/


#ifndef SERVO_H
#define SERVO_H

#include "stm32f4xx_hal.h"


//#define SIGNAL_FREQ 		50	// 50Hz

#define SERVO_DEGREE_0  	0
#define SERVO_DEGREE_180	180

#define SERVO_DELAY			300 // In ms

#define SERVO_SIGNAL_2ms	200 //  when P2CLK = 8 MHz
#define SERVO_SIGNAL_1_5ms	150 //  when P2CLK = 8 MHz
#define SERVO_SIGNAL_1ms	100 //  when P2CLK = 8 MHz
#define SERVO_SIGNAL_0_5ms	50 //  when P2CLK = 8 MHz

#define PRIORITY_SERVO		1

void servo_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
void servo_Write(uint8_t degrees);
void servo_Error(void);

#endif
