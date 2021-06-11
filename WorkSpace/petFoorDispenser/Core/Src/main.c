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

#define TRUE 1
#define FALSE !TRUE
#define SOURCE_BUTTON_ENTER   0
#define SOURCE_BUTTON_DOWN    1
#define SOURCE_BUTTON_UP	  2
#define SOURCE_NOTHING		  4

#define ROW_CYCLES 2
#define ROW_SPEED  1
#define ROW_BACK   0

int arrow[3][2] = {{0,9},{1,0},{0,0}}; // back,speed,cycles
int row = ROW_CYCLES;
int itSource = SOURCE_NOTHING;
int times_to_serve = 1;
extern int servo_delay;

void display_screen_main()
{
	lcd_clear();
	lcd_send_line("Dispenser",0,0);
	lcd_send_line("->settings",1,0);
}
void display_screen_settings(void)
{
	lcd_clear();
	lcd_send_line("cycles",0,2);
	lcd_send_line("speed",1,2);
	lcd_send_line("back",0,11);
}
void display_screen_cycles(void)
{
	lcd_clear();
	lcd_send_line("cycles < >",0,0);
}

void display_screen_speed(void)
{
	lcd_clear();
	lcd_send_line("speed <    >",0,0);
}

void func_screen_cycles(void)
{
	char cycles[2];
	display_screen_cycles();
	itoa(times_to_serve,cycles,10);
	lcd_send_line(cycles,0,8);
	flag_GPIO_it = FALSE;
	screen_main = FALSE;
	while(1)
	{
		while(!flag_GPIO_it);
		switch(itSource)
		{
			case SOURCE_BUTTON_UP: if(times_to_serve < 9)
								   {
									   times_to_serve++;
									   itoa(times_to_serve,cycles,10);
									   lcd_send_line(cycles,0,8);
								   }break;

			case SOURCE_BUTTON_DOWN: if(times_to_serve > 1)
									 {
										times_to_serve--;
										itoa(times_to_serve,cycles,10);
										lcd_send_line(cycles,0,8);
									}break;

			case SOURCE_BUTTON_ENTER: button_enter = FALSE;
									  row = ROW_CYCLES;
									  func_screen_main();
									  break;
			default: break;
		}
		flag_GPIO_it = FALSE;
	}

}

void func_screen_speed(void)
{
	char speed[4];
	display_screen_speed();
	itoa(servo_delay,speed,10);
	lcd_send_line(speed,0,7);
	flag_GPIO_it = FALSE;
	screen_main = FALSE;
	while(1)
	{
		while(!flag_GPIO_it);
		switch(itSource)
		{
			case SOURCE_BUTTON_UP: if(servo_delay < 2000)
									{
									   servo_delay = servo_delay + 100;
									   itoa(servo_delay,speed,10);
									   display_screen_speed();
									   lcd_send_line(speed,0,7);break;
									}break;


			case SOURCE_BUTTON_DOWN: if(servo_delay > 200)
									 {
									   servo_delay = servo_delay - 100;
									   itoa(servo_delay,speed,10);
									   display_screen_speed();
									   lcd_send_line(speed,0,7);break;
									 }break;

			case SOURCE_BUTTON_ENTER: button_enter = FALSE;
									  row = ROW_CYCLES;
									  func_screen_main();
									  break;
			default: break;
		}
		flag_GPIO_it = FALSE;
	}

}

void func_screen_settings(void)
{
	display_screen_settings();
	lcd_send_line("->",0,0);
	flag_GPIO_it = FALSE;
	screen_main = FALSE;

	while(1)
    {

		while(!flag_GPIO_it);

		switch(itSource)
		{
			case SOURCE_BUTTON_ENTER : button_enter = FALSE;
									   switch(row)
										{
											case ROW_CYCLES: func_screen_cycles();break;
											case ROW_SPEED: func_screen_speed();break;
											case ROW_BACK: button_enter = FALSE;
														   row = ROW_CYCLES;
														   func_screen_main();
														   break;
											default : break;
										}break;

			case SOURCE_BUTTON_DOWN:	if(row > 0)
										{
											row--;
											switch(row)
											{
												case ROW_BACK: display_screen_settings();lcd_send_line("->",arrow[row][0],arrow[row][1]);break;
												case ROW_SPEED: display_screen_settings();lcd_send_line("->",arrow[row][0],arrow[row][1]);break;
												case ROW_CYCLES: display_screen_settings();lcd_send_line("->",arrow[row][0],arrow[row][1]);break;
											default :break;
											}

										}break;
			case SOURCE_BUTTON_UP:	if(row < 3)
										{
											row++;
											switch(row)
											{
												case ROW_BACK: display_screen_settings();lcd_send_line("->",arrow[row][0],arrow[row][1]);break;
												case ROW_SPEED: display_screen_settings();lcd_send_line("->",arrow[row][0],arrow[row][1]);break;
												case ROW_CYCLES: display_screen_settings();lcd_send_line("->",arrow[row][0],arrow[row][1]);break;
											default :break;
											}

										}break;
			default:  break;

		}
		flag_GPIO_it = FALSE;
	}
}

void func_screen_main(void)
{
	display_screen_main();
	screen_main = TRUE;
	int i;
	while (screen_main)
	{
				if(button_dispense)
				{

					lcd_send_line_clr("serving...",1,0);
					for(i = 0; i < times_to_serve; i = i + 1)
					{
						servo_Write(SERVO_DEGREE_180);
						servo_Write(SERVO_DEGREE_0);
					}
					button_dispense = FALSE;
					display_screen_main();
				}
				else if(button_enter)
				{
					func_screen_settings();
				}

	}
}

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	HAL_Delay(250);
	GPIO_Init();
	MX_I2C1_Init();
	lcd_init ();
	servo_Init(GPIOA,GPIO_SERVO_A0);
	lcd_send_line_clr("Food dispenser",0,0);
	HAL_Delay(1000);

	while (1)
	{
		func_screen_main();

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
  __HAL_RCC_GPIOB_CLK_ENABLE();

  //Configure GPIO pins
  GPIO_InitStruct.Pin = BUTTON_DISPENSE_PIN | BUTTON_ENTER_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BUTTON_UP_PIN | BUTTON_DOWN_PIN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Interrupt settings
  HAL_NVIC_SetPriority(EXTI1_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

  HAL_NVIC_SetPriority(EXTI3_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
