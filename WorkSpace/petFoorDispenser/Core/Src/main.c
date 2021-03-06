#include "main.h"
#include "math.h"

TIM_HandleTypeDef tim3Buzzer_handle = {0};
TIM_HandleTypeDef tim4Debounce = {0};

uint32_t pulse294Hz = 170;
uint8_t numAlarmsSet;


/*
void swapAlarms(alarmTime **A,uint8_t i,uint8_t j)
{
	alarmTime *temp;
	temp = *(&A[i]);
	A[i] = A[j];
	A[j] = temp;
}
void gnomeSort(alarmTime **A,uint32_t sizeA)
{
    int index = 0;
    while(index < sizeA)
    {
        if (index == 0)
            index++;
        if (A[index]->acumValue >= A[index - 1]->acumValue)
            index++;
        else
        {
            swapAlarms(A,index,index-1);
            index--;
        }
    }
}
*/
alarmNumber nearestAlarm(alarmTime *alarms,uint8_t alarmsSet)
{
    uint8_t i;
    alarmTime *minAlarm = &alarms[0];
    for(i = 1; i < alarmsSet;i = i + 1)
    {
        if(alarms[i].acumValue < minAlarm->acumValue)
            minAlarm = &alarms[i];
    }
    return (&(*minAlarm))->number;
}
alarmNumber alarmReorder(void)
{
	RTC_TimeTypeDef time = {0};
	uint8_t i,j,count;

	HAL_RTC_GetTime(&rtc,&time,RTC_FORMAT_BIN);

	for(i = 0; i < 3; i = i + 1)
	{
		count = alarms[i].hour;
		for(j = 0; j < 24; j = j + 1)
		{
			if(count == 23)
				count = 0;
			if(count == time.Hours)
			{
				alarms[i].acumValue = j;
				break;
			}
			count++;
		}
	}

	for(i = 0; i < 3; i = i + 1)
	{
		count = alarms[i].minutes;
		for(j = 0; j < 60; j = j + 1)
		{
			if(count == 59)
				count = 0;
			if(count == time.Minutes)
			{
				alarms[i].acumValue = alarms[i].acumValue + j;
				break;
			}
			count++;
		}
	};
	return nearestAlarm(alarms,3);
}

uint8_t getAlarmsActive(void)
{
	uint8_t count = 0;
	uint8_t i;

	for(i = 0; i < NUM_ALARMS_USED; i = i + 1)
	{
		if(alarms[i].active == TRUE)
			count++;
	}
	return count;
}

uint8_t getAlarmPending(void)
{
	uint8_t alarmPending = NO_ALARM_PENDING;
	uint8_t i;

	for(i = 0; i < NUM_ALARMS_USED; i = i + 1)
	{
		if(alarms[i].pending == TRUE)
			alarmPending = i;
	}

	return alarmPending;
}
void buzzerSound(void)
{
	HAL_TIM_OC_Start_IT(&tim3Buzzer_handle,TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&tim6); // a short time for buzzer on
}
void tim3Debounce_init(void)
{
	__HAL_RCC_TIM4_CLK_ENABLE();

	tim4Debounce.Instance = TIM4;
	tim4Debounce.Init.Prescaler = 80;//clk = 100 kHz
	tim4Debounce.Init.Period = 1000-1;// interrup every 1ms

	if(HAL_TIM_Base_Init(&tim4Debounce) != HAL_OK)
	{
		Error_Handler();
	}

	// Interrupt settings
	HAL_NVIC_SetPriority(TIM4_IRQn,PRIORITY_BUTTON_DEBAUNCE,0);
	HAL_NVIC_EnableIRQ(TIM4_IRQn);
}

void handleAlarm(alarmNumber alarmNumberx)
{
	RTC_AlarmTypeDef sAlarm = {0};
	numAlarmsSet = 3;
	alarms[0].hour = 12;
	alarms[0].minutes = 12;
	alarms[1].hour = 14;
	alarms[1].minutes = 1;
	alarms[2].hour = 15;
	alarms[2].minutes = 0;

	numAlarmsSet  = getaAlarmsActive();
	alarms[alarmNumberx].active = TRUE;
	if (numAlarmsSet == 0)
	{
		alarms[alarmNumberx].active = TRUE;
		setAlarm(alarmNumberx);
	}
	else
	{
		setAlarm(alarmReorder());
	}

}
void setAlarm(alarmNumber alarmNumberx)
{
	RTC_AlarmTypeDef sAlarm = {0};

	sAlarm.Alarm = RTC_ALARM_A;
	sAlarm.AlarmTime.Hours = alarms[alarmNumberx].hour;
	sAlarm.AlarmTime.Minutes = alarms[alarmNumberx].minutes;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY | RTC_ALARMMASK_SECONDS;

	if(HAL_RTC_SetAlarm_IT(&rtc,&sAlarm,RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
}


void display_alarm(alarmNumber alarmNumberx)
{
	char hour[3];
	char minutes[3];

	itoa(alarms[alarmNumberx].hour,hour,10);
	itoa(alarms[alarmNumberx].minutes,minutes,10);

	if(alarms[alarmNumberx].hour > 9)
		screen_send_line(hour,timexy.hour_two_digits);
	else
	{
		screen_send_line("  ",timexy.hour_one_digit);
		screen_send_line(hour,timexy.hour_one_digit);
	}
	screen_send_line(":",timexy.colon);

	if(alarms[alarmNumberx].minutes > 9)
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
	GPIO_init();
	servo_Init(GPIOA,GPIO_SERVO_A0);
	MX_I2C1_Init();
	screen_init();
	screen_send_line("Food dispenser",menu.feed.screenxy);
	HAL_Delay(500);
	RTC_Init();
	tim3buzzer_init();
	tim6BuzzerSound_init();
	tim3Debounce_init();
	tim7DisplayUpdate_init();
	while (1)
	{
		HAL_TIM_Base_Start_IT(&tim7);
		//screen_main();
		//test zone
		alarmTime time = {0};
		setAlarm(&time);

	}
}

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  //configure the main internal regulator output voltage
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  //initializes the RCC Oscillators according to the specified parameters
  //in the RCC_OscInitTypeDef structure.
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  //initializes the CPU, AHB and APB buses clocks

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

 void GPIO_init(void)
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
  HAL_NVIC_SetPriority(EXTI2_IRQn,PRIORITY_BUTTON,0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  // up
  HAL_NVIC_SetPriority(EXTI1_IRQn,PRIORITY_BUTTON,0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);
  // enter
  HAL_NVIC_SetPriority(EXTI0_IRQn,PRIORITY_BUTTON,0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}
void RTC_Init(void)
 {
	RTC_TimeTypeDef time;

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

 	time.Hours = 0;
 	time.Minutes = 0;

 	if(HAL_RTC_SetTime(&rtc,&time,RTC_FORMAT_BIN) != HAL_OK)
 	{
 		Error_Handler();
 	}
 	//NVIC settings
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn,PRIORITY_ALARM,0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
 }


void tim3buzzer_init(void)
{

	GPIO_InitTypeDef gpioBuzzer= {0};
	TIM_OC_InitTypeDef tim3Buzzer_channel = {0};
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	// TIM Init
	tim3Buzzer_handle.Instance = TIM3;
	tim3Buzzer_handle.Init.Period = 0xFFFFFFFF;
	tim3Buzzer_handle.Init.Prescaler = 80; // newClk = 1 kHz, P1CLK = 8 Mhz

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
	HAL_NVIC_SetPriority(TIM3_IRQn,PRIORITY_BUZZER,0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
/*
	if(HAL_TIM_OC_Start_IT(&tim3Buzzer_handle,TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	*/
}
void tim6BuzzerSound_init(void)
{
	__HAL_RCC_TIM6_CLK_ENABLE();
	tim6.Instance = TIM6;
	tim6.Init.Prescaler = 80;// clk_timer = 100 kHz
	tim6.Init.Period = 2000-1;//  period = 700ms
	if(HAL_TIM_Base_Init(&tim6) != HAL_OK)
		Error_Handler();

	// Interrupt settings
	HAL_NVIC_SetPriority(TIM6_DAC_IRQn,PRIORITY_BUZZER_SOUND,0);
	HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

//timer to count every minute
void tim7DisplayUpdate_init(void)
{
	__HAL_RCC_TIM7_CLK_ENABLE();

	tim7.Instance = TIM7;
	tim7.Init.Prescaler = 40000 ;// clk_timeer = 200 Hz
	tim7.Init.Period = 12000-1;//  period = 1

	if(HAL_TIM_Base_Init(&tim7) != HAL_OK)
	{
		Error_Handler();
	}

	// Interrupt settings
	HAL_NVIC_SetPriority(TIM7_IRQn,PRIORIT_DISPLAY_UPDATE,0);
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
					case SOURCE_BUTTON_DOWN: if(time.Hours > 0) time.Hours = time.Hours - 1;break;
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
	uint8_t hours,minutes;
	alarms[alarmNumberx].number = alarmNumberx;
	hours = alarms[alarmNumberx].hour;
	minutes = alarms[alarmNumberx].minutes;

	flag_GPIO_it = FALSE;
	screen_clear();
	display_alarm(alarmNumberx);
	itSource = SOURCE_NOTHING;
	while(1)
	{
		while(1)
			{
			    while(!flag_GPIO_it);
				switch(itSource)
				{
					case SOURCE_BUTTON_DOWN: if(hours > 0) hours = hours - 1;break;
					case SOURCE_BUTTON_UP:	if(hours < 23) hours = hours + 1;break;
					default: break;
			    }
				alarms[alarmNumberx].hour = hours;
				display_alarm(alarmNumberx);
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
					case SOURCE_BUTTON_DOWN: if(minutes > 0) minutes = minutes - 1;break;
					case SOURCE_BUTTON_UP:	if(minutes < 59) minutes = minutes + 1;break;
					default: break;
				}
				alarms[alarmNumberx].minutes = minutes;
				display_alarm(alarmNumberx);
				flag_GPIO_it = FALSE;
				if (itSource == SOURCE_BUTTON_ENTER)
					break;
				else
					itSource = SOURCE_NOTHING;
			}
		handleAlarm(alarmNumberx);
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
	buzzerSound();
	flag_GPIO_it = TRUE;
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
	//if it is TIM6 buzzer is off
	if(htim->Instance == TIM6)

	{
		HAL_TIM_Base_Stop_IT(&tim6);
		HAL_TIM_OC_Stop(&tim3Buzzer_handle,TIM_CHANNEL_1);
		//HAL_TIM_OC_Stop_IT(&tim3Buzzer_handle,TIM_CHANNEL_1);
	}

	/*
	//software to avoid button debouncing
	if(htim->Instance == TIM4)
	{
		switch(itSource)
		{
			case BUTTON_ENTER_PIN: if(HAL_GPIO_ReadPin(GPIOB,BUTTON_ENTER_PIN) == FALSE)
								   {
								   	   flag_GPIO_it = TRUE;
								   }
								   break;
			case BUTTON_UP_PIN : if(HAL_GPIO_ReadPin(GPIOB,BUTTON_UP_PIN) == FALSE)
								 {
								 	  flag_GPIO_it = TRUE;
								 }
			   	   	   	   	   	 break;
			case BUTTON_DOWN_PIN : if(HAL_GPIO_ReadPin(GPIOB,BUTTON_DOWN_PIN) == FALSE)
								   {
								   	   flag_GPIO_it = TRUE;
								   }
								   break;
			default : break;
		}

		if(flag_GPIO_it == TRUE)
		{
			HAL_TIM_OC_Start(&tim3Buzzer_handle,TIM_CHANNEL_1);
			//HAL_TIM_Base_Start_IT(&tim6);//to stop buzzer
			//HAL_TIM_Base_Stop_IT(&tim4Debounce);
		}*/
		//HAL_TIM_Base_Stop_IT(&tim4Debounce);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	 if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	 {
		 __HAL_TIM_SET_COMPARE(htim,TIM_CHANNEL_1,HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1)+pulse294Hz);
	 }
}

 void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	 uint8_t alarmPending = NO_ALARM_PENDING;
	 dispense();

	 //alarmPending = getAlarmPending();
	 //setAlarm(&alarms[alarmPending]);
}
/*
void HAL_RTCEx_AlarmBEventCallback(RTC_HandleTypeDef *hrtc)
{

}*/
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

