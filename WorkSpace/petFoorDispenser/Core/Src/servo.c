
#include "servo.h"

TIM_HandleTypeDef tim2PWM = {0};

int servo_delay = 200; // ms

void servo_Init(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{

	GPIO_InitTypeDef gpioServo = {0};
	TIM_OC_InitTypeDef tim2PWMConfig = {0};

	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	//GPIO Init
	gpioServo.Pin = GPIO_Pin;
	gpioServo.Mode = GPIO_MODE_AF_PP;
	gpioServo.Alternate = GPIO_AF1_TIM2;

	HAL_GPIO_Init(GPIOA,&gpioServo);
	float a = HAL_RCC_GetPCLK1Freq();
	// TIM Init
	tim2PWM.Instance = TIM2;
	tim2PWM.Init.Prescaler = 80 - 1; // newClk = 100 kHz, P2CLK = 8 Mhz
	tim2PWM.Init.Period = 2000; // freq = 50 Hz

	if(HAL_TIM_PWM_Init(&tim2PWM) != HAL_OK)
	{
		servo_Error();
	}
	tim2PWMConfig.OCMode = TIM_OCMODE_PWM1;
	tim2PWMConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim2PWMConfig.Pulse = SERVO_DEGREE_0;

	if(HAL_TIM_PWM_ConfigChannel(&tim2PWM,&tim2PWMConfig,TIM_CHANNEL_1) != HAL_OK)
	{
		servo_Error();
	}
	HAL_TIM_PWM_Start(&tim2PWM,TIM_CHANNEL_1);
}



void servo_Write(uint8_t degrees)
{

	switch (degrees)
	{
		case SERVO_DEGREE_0: __HAL_TIM_SET_COMPARE(&tim2PWM,TIM_CHANNEL_1,SERVO_SIGNAL_2ms);
				 	 	 	  HAL_Delay(servo_delay);
				 	 	 	  break;
		case SERVO_DEGREE_180: __HAL_TIM_SET_COMPARE(&tim2PWM,TIM_CHANNEL_1,SERVO_SIGNAL_0_5ms);
							 HAL_Delay(servo_delay);
							 break;
		default : servo_Error(); break;
	}

}

void servo_Error(void)
{
	  __disable_irq();
	  while (1)
	  {
	  }
}
