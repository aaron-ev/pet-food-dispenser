#include "main.h"


int main(void)
{
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	servo_Init(GPIOA,GPIO_SERVO_A0);
	MX_I2C1_Init();
	HAL_Delay(100);
	screen_init();
	screen_send_line("Hello World",menu.feed.screenxy);
	//if(SSD1306_Init() != HAL_OK)
	//{
	//	Error_Handler();
	//}
	//oled_send_line_test("Food",menu.center);
	//oled_send_line("Food",20,10,&Font_11x18);
	HAL_Delay(200);
	//tim6_Init();
	tim7_Init();
	RTC_Init();

	while (1)
	{
		HAL_TIM_Base_Start_IT(&tim7);
		screen_main();
	}
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure the main internal regulator output voltage
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  // Initializes the RCC Oscillators according to the specified parameters
   //in the RCC_OscInitTypeDef structure.

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  //Initializes the CPU, AHB and APB buses clocks

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
  hi2c1.Init.ClockSpeed = 400000;
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
  GPIO_InitStruct.Pin = BUTTON_ENTER_PIN | BUTTON_UP_PIN | BUTTON_DOWN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // down
  HAL_NVIC_SetPriority(EXTI2_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  // up
  HAL_NVIC_SetPriority(EXTI1_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
  // enter
  HAL_NVIC_SetPriority(EXTI0_IRQn,0,15);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
void RTC_Init(void)
 {
 	rtc.Instance = RTC;
 	rtc.Init.HourFormat = RTC_HOURFORMAT_24;
 	rtc.Init.AsynchPrediv = 127;
 	rtc.Init.SynchPrediv = 255;
 	rtc.Init.OutPut = RTC_OUTPUT_DISABLE;
 	rtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_LOW;
 	rtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

 	if(HAL_RTC_Init(&rtc) != HAL_OK)
 	{
 		Error_Handler();
 	}

 	RTC_TimeTypeDef time;

 	time.Hours = 0;
 	time.Minutes = 0;
 	time.Seconds = 0;
 	time.TimeFormat = RTC_HOURFORMAT_24;

 	if(HAL_RTC_SetTime(&rtc,&time,RTC_FORMAT_BIN) != HAL_OK)
 	{
 		Error_Handler();
 	}
 }

void tim6_Init(void)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	tim6.Instance = TIM6;
	tim6.Init.Prescaler = 80;// clk_timeer = 100 kHz
	tim6.Init.Period = 7000-1;//  period = 700ms
	if(HAL_TIM_Base_Init(&tim6) != HAL_OK)
		Error_Handler();

	// Interrupt settings
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,0,15);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void tim7_Init(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();
	tim7.Instance = TIM7;
	float a = HAL_RCC_GetPCLK2Freq();
	tim7.Init.Prescaler =8000 ;// clk_timeer = 1 kHz
	tim7.Init.Period = 1000-1;//  period = 1
	if(HAL_TIM_Base_Init(&tim7) != HAL_OK)
	{
		Error_Handler();
	}

	// Interrupt settings
	HAL_NVIC_SetPriority(TIM7_IRQn,0,15);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
}
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

// Functions to display screens
void display_screen_main()
{
	screen_clear();
	display_hour();
	screen_send_line("feed",menu.feed.screenxy);
	screen_send_line("->",menu.feed.arrowxy);
	screen_send_line("settings",menu.settings.screenxy);
	arrow_row = ROW_FEED;
	HAL_NVIC_EnableIRQ(TIM7_IRQn); // Enable hour update
}
void display_screen_settings(void)
{
	screen_clear();
	screen_send_line("speed",menu.speed.screenxy);
	screen_send_line("time",menu.time.screenxy);
	screen_send_line("alarms",menu.alarms.screenxy);
#ifdef LCD_2X16
	screen_send_line("back",menu.back.screenxy);
#endif
}
/*
void display_screen_cycles(void)
{
	oled_clear();
	oled_send_line("cycles",arrow[ROW_CENTER][0],arrow[ROW_CENTER][1],&Font_11x18);
	oled_send_line("<  >",arrow[ROW_CENTER][0],arrow[ROW_CENTER][1]+20,&Font_11x18);
}
*/
void display_screen_speed(void)
{
	screen_clear();
	screen_send_line("speed:",menu.center.screenxy);
}

void display_screen_time(void)
{
	screen_clear();
	display_hour();
}
void display_screen_alarms(void)
{
	screen_clear();
	screen_send_line("alarm1",menu.alarm1.screenxy);
	screen_send_line("alarm2",menu.alarm2.screenxy);
	screen_send_line("alarm3",menu.alarm3.screenxy);
	screen_send_line("back",menu.back.screenxy);
}
void display_hour(void)
{
	RTC_TimeTypeDef time;
	char hours[3];
	char minutes[3];
	char seconds[3];

	HAL_RTC_GetTime(&rtc,&time,RTC_FORMAT_BIN);

	itoa(time.Hours,hours,10);
	itoa(time.Minutes,minutes,10);
	itoa(time.Seconds,seconds,10);

	if(time.Hours > 9)
		screen_send_line(hours,timexy.hour);
	else
	{
		screen_send_line("  ",timexy.hour);
		screen_send_line(hours,timexy.hour);
	}
	screen_send_line(":",timexy.colon1);

	if(time.Minutes > 9)
		screen_send_line(minutes,timexy.minutes);
	else
	{
		screen_send_line("  ",timexy.minutes);
		screen_send_line(minutes,timexy.minutes);
	}
	screen_send_line(":",timexy.colon2);

	if(time.Seconds > 9)
		screen_send_line(seconds,timexy.seconds);
	else
	{
		screen_send_line("  ",timexy.seconds);
		screen_send_line(seconds,timexy.seconds);
	}
}
/*
// Functions to manage a screen
void screen_cycles(void)
{
	char cycles[2];
	display_screen_cycles();
	itoa(times_to_serve,cycles,10);
	oled_send_line(cycles,arrow[ROW_CENTER][0]+12,arrow[ROW_CENTER][1]+20,&Font_11x18);
	flag_GPIO_it = FALSE;
	while(1)
	{
		while(!flag_GPIO_it);
		switch(itSource)
		{
			case SOURCE_BUTTON_UP: if(times_to_serve < 9)
								   {
									   times_to_serve++;
									   itoa(times_to_serve,cycles,10);
									   oled_send_line(cycles,arrow[ROW_CENTER][0]+ 12,arrow[ROW_CENTER][1]+20,&Font_11x18);
								   }break;

			case SOURCE_BUTTON_DOWN: if(times_to_serve > 1)
									 {
										times_to_serve--;
										itoa(times_to_serve,cycles,10);
										oled_send_line(cycles,arrow[ROW_CENTER][0]+ 12,arrow[ROW_CENTER][1]+20,&Font_11x18);
									}break;

			case SOURCE_BUTTON_ENTER:screen_main();break;
			default: break;
		}
		itSource = SOURCE_NOTHING;
		flag_GPIO_it = FALSE;
	}

}
*/
void screen_main(void)
{
	display_screen_main();
	flag_GPIO_it = FALSE;
	while(1)
    {
		while(!flag_GPIO_it);

		switch(itSource)
		{
			case SOURCE_BUTTON_ENTER : if(arrow_row == ROW_FEED)
									   	   dispense();
									   else
										   screen_settings();
										break;
			case SOURCE_BUTTON_DOWN: arrow_row = ROW_SETTING;
									 screen_send_line("  ",menu.feed.arrowxy);
									 screen_send_line("->",menu.settings.arrowxy);
									 break;
			case SOURCE_BUTTON_UP:	arrow_row = ROW_FEED;
									screen_send_line("  ",menu.settings.arrowxy);
									screen_send_line("->",menu.feed.arrowxy);
									break;
			default: break;

		}
		itSource = SOURCE_NOTHING;
 		flag_GPIO_it = FALSE;
	}
}
void screen_settings(void)
{
	HAL_NVIC_DisableIRQ(TIM7_IRQn);
	display_screen_settings();
	arrow_row = ROW_SPEED;
	screen_send_line("->",menu.speed.arrowxy);
	flag_GPIO_it = FALSE;

	while(1)
    {
		while(!flag_GPIO_it);
		switch(itSource)
		{
			case SOURCE_BUTTON_ENTER : switch(arrow_row)
										{
											//case ROW_CYCLES: screen_cycles();break;
											case ROW_SPEED:  screen_speed();break;
											case ROW_TIME:   screen_time();break;
											case ROW_ALARMS: screen_alarms();break;
											case ROW_BACK: screen_main();break;
											default : break;
										}break;

			case SOURCE_BUTTON_DOWN: if(arrow_row  == ROW_BACK)
									 {
										break;
									 }
											if(arrow_row > ROW_BACK)
												{
													arrow_row--;
													clear_arrowSettings();
													switch(arrow_row)
													{
														case ROW_SPEED:screen_send_line("->",menu.speed.arrowxy);break;
														case ROW_TIME: screen_send_line("->",menu.time.arrowxy);break;
														case ROW_ALARMS: screen_send_line("->",menu.alarms.arrowxy);break;
														case ROW_BACK: screen_send_line("->",menu.back.arrowxy);break;
														default:break;
													}
												}
									 break;
			case SOURCE_BUTTON_UP:	if(arrow_row  == ROW_SPEED)
									{
										break;
									}
									if(arrow_row < ROW_SPEED)
										{
											arrow_row++;
											clear_arrowSettings();
											switch(arrow_row)
											{
												case ROW_SPEED:screen_send_line("->",menu.speed.arrowxy);break;
												case ROW_TIME: screen_send_line("->",menu.time.arrowxy);break;
												case ROW_ALARMS: screen_send_line("->",menu.alarms.arrowxy);break;
												case ROW_BACK: screen_send_line("->",menu.back.arrowxy);break;
												default:break;
											}
										}
								break;
			default:  break;
    	}
		itSource = SOURCE_NOTHING;
		flag_GPIO_it = FALSE;
	}
}
void screen_speed(void)
{
	char speed[4];
	display_screen_speed();
	itoa(servo_delay,speed,10);
	screen_send_line(speed,menu.speed_value.screenxy);
	flag_GPIO_it = FALSE;

	while(1)
	{
		while(!flag_GPIO_it);
		switch(itSource)
		{
			case SOURCE_BUTTON_UP: if(servo_delay < 2000)
									{
									   servo_delay = servo_delay + 100;
									   itoa(servo_delay,speed,10);
									   screen_send_line("    ",menu.speed_value.screenxy);
									   screen_send_line(speed,menu.speed_value.screenxy);
									   break;
									}break;


			case SOURCE_BUTTON_DOWN: if(servo_delay > 200)
									 {
									   servo_delay = servo_delay - 100;
									   itoa(servo_delay,speed,10);
									   screen_send_line("    ",menu.speed_value.screenxy);
									   screen_send_line(speed,menu.speed_value.screenxy);
									   break;
									 }break;

			case SOURCE_BUTTON_ENTER: screen_main();break;
			default: break;
		}
		itSource = SOURCE_NOTHING;
		flag_GPIO_it = FALSE;
	}

}
void screen_time(void)
{
	RTC_TimeTypeDef time = {0};
	//uint8_t finish = 0;
	flag_GPIO_it = FALSE;
	display_screen_time();
	HAL_RTC_GetTime(&rtc,&time,RTC_FORMAT_BIN);
	itSource = SOURCE_NOTHING;

	while(1)
	{
		while(1)
			{
			    while(!flag_GPIO_it);
				switch(itSource)
				{
					case SOURCE_BUTTON_DOWN: if(time.Hours > 1) time.Hours = time.Hours - 1;break;
					case SOURCE_BUTTON_UP:	if(time.Hours < 23) time.Hours = time.Hours + 1;break;
					default: break;
				}
				update_time(time.Hours,1);
				flag_GPIO_it = FALSE;
				if (itSource == SOURCE_BUTTON_ENTER)
					break;
				else
					itSource = SOURCE_NOTHING;
			}
		while(1)
			{
			    while(!flag_GPIO_it);
				switch(itSource)
				{
					case SOURCE_BUTTON_DOWN: if(time.Minutes > 1) time.Minutes = time.Minutes - 1;break;
					case SOURCE_BUTTON_UP:	if(time.Minutes < 59) time.Minutes = time.Minutes + 1;break;
					default: break;
				}
				update_time(time.Minutes,2);
				flag_GPIO_it = FALSE;
				if (itSource == SOURCE_BUTTON_ENTER)
					break;
				else
					itSource = SOURCE_NOTHING;
			}
		while(1)
			{
			    while(!flag_GPIO_it);
				switch(itSource)
				{
					case SOURCE_BUTTON_DOWN: if(time.Seconds > 1) time.Seconds = time.Seconds - 1;break;
					case SOURCE_BUTTON_UP:	if(time.Seconds < 59) time.Seconds = time.Seconds + 1;break;
					default: break;
				}
				update_time(time.Seconds,3);
				flag_GPIO_it = FALSE;
				if (itSource == SOURCE_BUTTON_ENTER)
					break;
				else
					itSource = SOURCE_NOTHING;
			}
		set_time(&time);
		screen_main();
	}
}
void screen_alarms(void)
{
	//HAL_NVIC_DisableIRQ(TIM7_IRQn);
	display_screen_alarms();
	arrow_row = ROW_ALARM1;
	screen_send_line("->",menu.alarm1.arrowxy);
	flag_GPIO_it = FALSE;

	while(1)
    {
		while(!flag_GPIO_it);
		switch(itSource)
		{
			case SOURCE_BUTTON_ENTER : switch(arrow_row)
										{
											case ROW_ALARM1: screen_alarmSelected(ALARM1);break;
											case ROW_ALARM2: screen_alarmSelected(ALARM2);break;
											case ROW_ALARM3: screen_alarmSelected(ALARM3);break;
											case   ROW_BACK: screen_main();break;
											default : break;
										}break;

			case SOURCE_BUTTON_DOWN: if(arrow_row  == ROW_BACK)
									 {
										break;
									 }
											if(arrow_row > ROW_BACK)
												{
													arrow_row--;
													clear_arrowAlarms();
													switch(arrow_row)
													{
														case ROW_ALARM1: screen_send_line("->",menu.alarm1.arrowxy);break;
														case ROW_ALARM2: screen_send_line("->",menu.alarm2.arrowxy);break;
														case ROW_ALARM3:screen_send_line("->",menu.alarm3.arrowxy);break;
														case ROW_BACK:screen_send_line("->",menu.back.arrowxy);break;
														default:break;
													}
												}
									 break;
			case SOURCE_BUTTON_UP:	if(arrow_row  == ROW_ALARM1)
									{
										break;
									}
									if(arrow_row < ROW_ALARM1)
										{
											arrow_row++;
											clear_arrowAlarms();
											switch(arrow_row)
											{
												case ROW_ALARM1: screen_send_line("->",menu.alarm1.arrowxy);break;
												case ROW_ALARM2: screen_send_line("->",menu.alarm2.arrowxy);break;
												case ROW_ALARM3:screen_send_line("->",menu.alarm3.arrowxy);break;
												default:break;
											}
										}
								break;
			default:  break;
    	}
		itSource = SOURCE_NOTHING;
		flag_GPIO_it = FALSE;
	}
}
void screen_alarmSelected(alarms alarm)
{

}
// Function to dispense food
void dispense(void)
{
	int i;
	HAL_NVIC_DisableIRQ(TIM7_IRQn); // to not update hour
	screen_clear();
	screen_send_line("serving...",menu.serving.screenxy);
	for(i = 0; i < times_to_serve; i = i + 1)
	{
		servo_Write(SERVO_DEGREE_180);
		servo_Write(SERVO_DEGREE_0);
	}
	screen_main();
}

void disable_it_buttons(void)
{
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
	HAL_NVIC_DisableIRQ(EXTI3_IRQn);
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
}
void enable_it_buttons(void)
{
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
	HAL_NVIC_EnableIRQ(EXTI3_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}
// Call backs
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	flag_GPIO_it = TRUE;
	button_pressed = GPIO_Pin;
	switch (GPIO_Pin)
		{
			case BUTTON_ENTER_PIN : itSource = SOURCE_BUTTON_ENTER;break;
			case BUTTON_UP_PIN : itSource = SOURCE_BUTTON_UP;break;
			case BUTTON_DOWN_PIN : itSource = SOURCE_BUTTON_DOWN;break;
			default : break;
		}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM7)
	{
		display_hour();
	}
}

void update_time(uint8_t t,uint8_t m)
{
	char time_chr[3];
	itoa(t,time_chr,10);
	if(m == 1)
	{
		screen_send_line("  ",timexy.hour);
		screen_send_line(time_chr,timexy.hour);
	}
	else if(m == 2)
	{
		screen_send_line("  ",timexy.minutes);
		screen_send_line(time_chr,timexy.minutes);
	}
    else
    {
		screen_send_line("  ",timexy.seconds);
		screen_send_line(time_chr,timexy.seconds);
    }
	/*
    }
	screen_send_line("  ",time.Hours);
	if(t > 9)
		oled_send_line(time,hourxy[m][0],hourxy[m][1],&Font_11x18);
	else
		oled_send_line(time,hourxy[m][0]+11,hourxy[m][1],&Font_11x18);
		*/
}

void set_time(RTC_TimeTypeDef *time)
{
	if(HAL_RTC_SetTime(&rtc,time,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}

void clear_arrowSettings(void)
{
	screen_send_line("  ",menu.speed.arrowxy);
	screen_send_line("  ",menu.time.arrowxy);
	screen_send_line("  ",menu.alarms.arrowxy);
#ifdef LCD_2X16
	screen_send_line("  ",menu.back.arrowxy);
#endif
}
void clear_arrowAlarms(void)
{
	screen_send_line("  ",menu.alarm1.arrowxy);
	screen_send_line("  ",menu.alarm2.arrowxy);
	screen_send_line("  ",menu.alarm3.arrowxy);
#ifdef LCD_2X16
	screen_send_line("  ",menu.back.arrowxy);
#endif
}

