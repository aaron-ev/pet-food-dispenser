/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
 */

#include "stm32f4xx_hal.h"
#include "main.h"
#include "lcd_i2c.h"
#include "servo.h"

void SystemClock_Config(void);
static void MX_I2C1_Init(void);

uint8_t servo_flag = 0;

int main(void)
{
	int i;
	HAL_Init();
	HAL_Delay(250);
	SystemClock_Config();
	GPIO_Init();
	MX_I2C1_Init();
	lcd_init ();

	lcd_clear();
	lcd_put_cur(1,0);

	lcd_send_string ("Hello World");
	HAL_Delay(1000);
	servo_Init(GPIOA,GPIO_SERVO_A0);
	while (1)
	{
		lcd_clear();
		lcd_put_cur(1,0);
		lcd_send_string ("-> Settings");
		while (screen_main)
		 {
			if(servo_flag)
			{
				//Message
				lcd_clear();
				lcd_put_cur(1,0);
				lcd_send_string ("Serving...");
				for(i = 0; i < SERVO_TIMES_TO_SERVE; i = i + 1)
				{
					servo_Write(SERVO_DEGREE_180);
					servo_Write(SERVO_DEGREE_0);
					servo_flag = 0;
				}

			}
			else if(button_enter)
			{
				screen_main = FALSE;
				screen_settings = TRUE;
			    //Message
			}
			else

			while(screen_settings)
		    {
				if(button_enter)
				{
					//Cycles
				}
			}

		}
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_I2C1_Init(void)
{
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
}

 void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  //GPIO Ports Clock Enable
  __HAL_RCC_GPIOA_CLK_ENABLE();
  //configure GPIO pin Output Level
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);

  //Configure GPIO pin : PA5
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Interrupt settings
  HAL_NVIC_SetPriority(EXTI1_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
}

 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
 {
	 servo_flag = 1;
 }

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
