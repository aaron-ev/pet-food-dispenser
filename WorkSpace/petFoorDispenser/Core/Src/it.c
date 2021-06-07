

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}


