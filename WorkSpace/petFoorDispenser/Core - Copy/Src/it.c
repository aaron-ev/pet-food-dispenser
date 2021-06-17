#include <it.h>
#include "stm32f4xx_hal.h"

extern TIM_TypeDef tim6;
extern TIM_TypeDef tim7;

void SysTick_Handler(void)
{
  HAL_IncTick();
}

// down
void EXTI2_IRQHandler(void)
{
	//if(HAL_GPIO_ReadPin(GPIOB,SOURCE_BUTTON_DOWN) == GPIO_PIN_RESET)
		HAL_GPIO_EXTI_IRQHandler(SOURCE_BUTTON_DOWN);
	//__HAL_GPIO_EXTI_CLEAR_IT(SOURCE_BUTTON_DOWN);
}
// up
void EXTI1_IRQHandler(void)
{
	//if(HAL_GPIO_ReadPin(GPIOB,SOURCE_BUTTON_UP) == GPIO_PIN_RESET)
		HAL_GPIO_EXTI_IRQHandler(SOURCE_BUTTON_UP);
	//__HAL_GPIO_EXTI_CLEAR_IT(SOURCE_BUTTON_UP);
}
// enter
void EXTI0_IRQHandler(void)
{
	if(HAL_GPIO_ReadPin(GPIOB,SOURCE_BUTTON_ENTER) == GPIO_PIN_SET)
			HAL_GPIO_EXTI_IRQHandler(SOURCE_BUTTON_ENTER);
		__HAL_GPIO_EXTI_CLEAR_IT(SOURCE_BUTTON_ENTER);
}

// timer 6
void TIM6_DAC_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim6);
}
void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&tim7);
}
