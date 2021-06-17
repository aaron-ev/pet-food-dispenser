
#ifndef IT_H
#define IT_H

typedef enum
{
	FALSE,
	TRUE
}bool;
#define TRUE	1
#define FALSE !TRUE

#define SOURCE_BUTTON_ENTER   GPIO_PIN_0
#define SOURCE_BUTTON_DOWN    GPIO_PIN_2
#define SOURCE_BUTTON_UP	  GPIO_PIN_1
#define SOURCE_NOTHING		  50

void SysTick_Handler(void);

#endif
