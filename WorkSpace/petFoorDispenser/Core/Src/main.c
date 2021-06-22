#include "main.h"
#include "lcd1602_i2c.h"

TIM_HandleTypeDef tim3Buzzer_handle = {0};
uint32_t pulse294Hz = 170;
void buzzer_init(void)
{

	GPIO_InitTypeDef gpioBuzzer= {0};
	TIM_OC_InitTypeDef tim3Buzzer_channel = {0};
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	// TIM Init
	tim3Buzzer_handle.Instance = TIM3;
	tim3Buzzer_handle.Init.Period = 0xFFFFFFFF;
	tim3Buzzer_handle.Init.Prescaler = 80; // newClk = 1 kHz, P1CLK = 8 Mhz

	float a = HAL_RCC_GetPCLK1Freq();
	float b = HAL_RCC_GetHCLKFreq();
	float C = HAL_RCC_GetSysClockFreq();
	float d = HAL_RCC_GetPCLK2Freq();

	//GPIO Init
	gpioBuzzer.Pin = GPIO_PIN_6;
	gpioBuzzer.Mode = GPIO_MODE_AF_PP;
	gpioBuzzer.Pull = GPIO_NOPULL;
	gpioBuzzer.Speed = GPIO_SPEED_FREQ_LOW;
	gpioBuzzer.Alternate = GPIO_AF2_TIM3;

	HAL_GPIO_Init(GPIOA,&gpioBuzzer);

	if(HAL_TIM_OC_Init(&tim3Buzzer_handle) != HAL_OK)
	{
		Error_Handler();
	}

	tim3Buzzer_channel.OCMode = TIM_OCMODE_TOGGLE;
	tim3Buzzer_channel.OCPolarity = TIM_OCNPOLARITY_HIGH;
	tim3Buzzer_channel.Pulse = pulse294Hz;

	if(HAL_TIM_OC_ConfigChannel(&tim3Buzzer_handle,&tim3Buzzer_channel,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

	// Interrupt settings
	HAL_NVIC_SetPriority(TIM3_IRQn,0,15);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
/*
	if(HAL_TIM_OC_Start_IT(&tim3Buzzer_handle,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	*/
}

void update_alarmTime(alarmTime *alarmTimex)
{
	char hour[3];
	char minutes[3];

	if(alarmTimex->number != ALARM_NONE)
	{
		switch(alarmTimex->number)
		{
		case ALARM1:alarm1.hour = alarmTimex->hour;
					alarm1.minutes = alarmTimex->minutes;
					break;
		case ALARM2:alarm2.hour = alarmTimex->hour;
					alarm2.minutes = alarmTimex->minutes;
					break;
		case ALARM3:alarm3.hour = alarmTimex->hour;
					alarm3.minutes = alarmTimex->minutes;
					break;
		default:break;
		}

	}

	itoa(alarmTimex->hour,hour,10);
	itoa(alarmTimex->minutes,minutes,10);

	if(alarmTimex->hour > 9)
		screen_send_line(hour,timexy.hour_two_digits);
	else
	{
		screen_send_line("  ",timexy.hour_one_digit);
		screen_send_line(hour,timexy.hour_one_digit);
	}
	screen_send_line(":",timexy.colon);

	if(alarmTimex->minutes > 9)
		screen_send_line(minutes,timexy.minutes_zero);
	else
	{
		screen_send_line("0",timexy.minutes_zero);
		screen_send_line(minutes,timexy.minutes);
	}
}

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	servo_Init(GPIOA,GPIO_SERVO_A0);
	MX_I2C1_Init();
	screen_init();
	screen_send_line("Food dispener",menu.feed.screenxy);
	HAL_Delay(500);
	buzzer_init();
	tim6Buzzer_Init();
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

 	time.Hours = 23;
 	time.Minutes = 59;
 	//time.TimeFormat = RTC_HOURFORMAT_24;

 	if(HAL_RTC_SetTime(&rtc,&time,RTC_FORMAT_BIN) != HAL_OK)
 	{
 		Error_Handler();
 	}
 }

void tim6Buzzer_Init(void)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	tim6.Instance = TIM6;
	tim6.Init.Prescaler = 8000;// clk_timer = 1 kHz
	tim6.Init.Period = 100-1;//  period = 700ms
	if(HAL_TIM_Base_Init(&tim6) != HAL_OK)
		Error_Handler();

	// Interrupt settings
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,0,14);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

// Timer to interrupt every second
void tim7_Init(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();
	tim7.Instance = TIM7;
	float a = HAL_RCC_GetPCLK1Freq();
	tim7.Init.Prescaler = 8000 ;// clk_timeer = 1 kHz
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
	screen_send_line("1.A1",menu.alarm1.screenxy);
	screen_send_line("2.A2",menu.alarm2.screenxy);
	screen_send_line("3.A3",menu.alarm3.screenxy);
	screen_send_line("back",menu.back.screenxy);
}
void display_hour(void)
{
	RTC_TimeTypeDef time;
	char hours[3];
	char minutes[3];

	HAL_RTC_GetTime(&rtc,&time,RTC_FORMAT_BIN);

	itoa(time.Hours,hours,10);
	itoa(time.Minutes,minutes,10);

	if(time.Hours > 9)
		screen_send_line(hours,timexy.hour_two_digits);
	else
	{
		screen_send_line("  ",timexy.hour_two_digits);
		screen_send_line(hours,timexy.hour_one_digit);
	}
	screen_send_line(":",timexy.colon);

	if(time.Minutes > 9)
		screen_send_line(minutes,timexy.minutes_zero);
	else
	{
		screen_send_line("0",timexy.minutes_zero);
		screen_send_line(minutes,timexy.minutes);
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
/*
void display_screen_alarmSelected(alarmNumber alarmx)
{
	screen_clear();
	switch(alarmx)
	{
		case ALARM1: update_alarmTime(alarm1);break;
		case ALARM2: update_alarmTime(alarm2);break;
		case ALARM3: update_alarmTime(alarm3);break;
		default:     break;
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
					case SOURCE_BUTTON_DOWN: if(time.Minutes > 0) time.Minutes = time.Minutes - 1;break;
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
void screen_alarmSelected(alarmNumber alarmNumberx)
{
	alarmTime alarmTimex = {0};
	alarmTimex.number = ALARM_NONE;
	flag_GPIO_it = FALSE;
	update_alarmTime(&alarmTimex);
	itSource = SOURCE_NOTHING;

	while(1)
	{
		while(1)
			{
			    while(!flag_GPIO_it);
				switch(itSource)
				{
					case SOURCE_BUTTON_DOWN: if(alarmTimex.hour > 1) alarmTimex.hour = alarmTimex.hour - 1;break;
					case SOURCE_BUTTON_UP:	if(alarmTimex.hour < 23) alarmTimex.hour = alarmTimex.hour + 1;break;
					default: break;
			    }
				alarmTimex.number = alarmNumberx;
				update_alarmTime(&alarmTimex);
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
					case SOURCE_BUTTON_DOWN: if(alarmTimex.minutes > 1) alarmTimex.minutes = alarmTimex.minutes - 1;break;
					case SOURCE_BUTTON_UP:	if(alarmTimex.minutes < 59) alarmTimex.minutes = alarmTimex.minutes + 1;break;
					default: break;
				}
				update_alarmTime(&alarmTimex);
				flag_GPIO_it = FALSE;
				if (itSource == SOURCE_BUTTON_ENTER)
					break;
				else
					itSource = SOURCE_NOTHING;
			}
		//set_alarm(alarmTimex,alarmNumberx);
		//set_time(&time);
		screen_main();
	}
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
	HAL_TIM_OC_Start_IT(&tim3Buzzer_handle,TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&tim6);
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

	if(htim->Instance == TIM6)

	{
		HAL_TIM_OC_Stop(&tim3Buzzer_handle,TIM_CHANNEL_1);
		HAL_TIM_Base_Stop_IT(&tim6);
	}

}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	 {
		 __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1)+pulse294Hz);
	 }
}

void update_time(uint8_t t,uint8_t m)
{
	char time_chr[3];
	itoa(t,time_chr,10);
	if(m == 1)
	{
		screen_send_line("  ",timexy.hour_two_digits);
		if(t > 9)
			screen_send_line(time_chr,timexy.hour_two_digits);
		else
			screen_send_line(time_chr,timexy.hour_one_digit);

	}
	else if(m == 2)
	{
		screen_send_line("  ",timexy.minutes_zero);
		if(t > 9)
			screen_send_line(time_chr,timexy.minutes_zero);
		else
		{
			screen_send_line("0",timexy.minutes_zero);
			screen_send_line(time_chr,timexy.minutes);
		}
	}
    else
    {
    }
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

