/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  *
 */


#include "main.h"
#include "lcd_i2c.h"
#include "servo.h"


void display_screen_main()
{
	lcd_clear();
	lcd_send_line("Hour",1,3);
	lcd_send_line("-> settings",1,0);
}
void display_screen_settings(void)
{
	lcd_clear();
	lcd_send_line("cycles",0,3);
	lcd_send_line("speed",1,3);
	lcd_send_line("back",0,12);

}

void screen_settings(void)
{
	display_screen_settings();
	lcd_send_line("->",0,0);
	while(1)
    {
		while(!flag_GPIO_it);
		sum_up_down = button_up - button_down;
		switch(sum_up_down)
		{
			case 0: display_screen_settings();lcd_send_line("->",0,0); break;
			case -1:display_screen_settings();lcd_send_line("->",1,0);break;
			case -2:display_screen_settings();lcd_send_line("->",0,10);break;

		}
		flag_GPIO_it = FALSE;
	}
}
int main(void)
{
	int i;
	HAL_Init();
	HAL_Delay(250);
	SystemClock_Config();
	GPIO_Init();
	MX_I2C1_Init();
	lcd_init ();
	lcd_send_line_clr("Hello World",1,0);
	HAL_Delay(1000);
	servo_Init(GPIOA,GPIO_SERVO_A0);
	display_screen_main();

	while (1)
	{
		while (screen_main)
		 {
			if(button_dispense)
			{
				//Message
				lcd_send_line_clr("serving...",1,0);
				for(i = 0; i < SERVO_TIMES_TO_SERVE; i = i + 1)
				{
					servo_Write(SERVO_DEGREE_180);
					servo_Write(SERVO_DEGREE_0);
					button_dispense = 0;
				}
				display_screen_main();
			}
			else if(button_enter)
			{
				screen_main = FALSE;
				screen_settings();
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
  HAL_GPIO_WritePin(GPIOA, BUTTON_DISPENSE_PIN, GPIO_PIN_RESET);

  //Configure GPIO pins
  GPIO_InitStruct.Pin = BUTTON_DISPENSE_PIN ;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON_DOWN_PIN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Interrupt settings
  HAL_NVIC_SetPriority(EXTI1_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
  //HAL_NVIC_SetPriority(EXTI3_IRQn,0,15);
  //HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

 void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
 {
	 //if(HAL_GPIO_ReadPin(GPIOA,BUTTON_DISPENSE_PIN))
			 //button_dispense = TRUE;
	// else if(HAL_GPIO_ReadPin(GPIOA,BUTTON_DOWN_PIN))
		// BUTTON_DOWN_PIN++;
	 if(button_down < 3)
	 {
		 flag_GPIO_it = TRUE;
		 button_down++;
		__HAL_GPIO_EXTI_CLEAR_IT(BUTTON_DOWN_PIN);
	 }
 }

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
