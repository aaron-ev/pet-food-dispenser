

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"

extern int8_t button_down;
void SysTick_Handler(void)
{
  HAL_IncTick();
}

void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI3_IRQHandler(void)
{
	button_down++;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
