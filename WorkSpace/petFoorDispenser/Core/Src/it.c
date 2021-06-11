#include <it.h>
#include "stm32f4xx_hal.h"

extern bool flag_GPIO_it;
extern int itSource;

void SysTick_Handler(void)
{
  HAL_IncTick();
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
	flag_GPIO_it = TRUE;
	itSource = SOURCE_BUTTON_ENTER;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_10);
}

