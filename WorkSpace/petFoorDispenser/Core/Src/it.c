

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"

#define TRUE 1

extern int8_t button_down;
extern bool flag_GPIO_it;
extern int itSource;
extern bool button_enter;
extern bool screen_main;

#define SOURCE_BUTTON_ENTER   0
#define SOURCE_BUTTON_DOWN    1
#define SOURCE_BUTTON_UP	  2

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void EXTI1_IRQHandler(void)
{
	itSource = 0;
	button_enter = TRUE;
	 if(button_down < 3)
	 {
		 if(!screen_main)
			 flag_GPIO_it = TRUE;
		 button_down++;
	 }
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
//down
void EXTI2_IRQHandler(void)
{
	flag_GPIO_it = TRUE;
	itSource = SOURCE_BUTTON_DOWN;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
//up
void EXTI3_IRQHandler(void)
{
	flag_GPIO_it = TRUE;
	itSource = SOURCE_BUTTON_UP;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
//enter
void EXTI15_10_IRQHandler(void)
{
	button_enter = TRUE;
	 if(!screen_main)
	 {
		 flag_GPIO_it = TRUE;
		 itSource = SOURCE_BUTTON_ENTER;
	 }
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
}
