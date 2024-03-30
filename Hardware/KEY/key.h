#ifndef __KEY_H
#define __KEY_H


#include "stm32f10x.h"   
#include <stdio.h>
#include "main.h"

enum
{
	KEY1 = 1,
	KEY2 = 2,
	KEY3 = 3,
	KEYD = 4,
	KEYA = 5,
	KEYB = 6,
};

//按键引脚定义
#define KEY1_INT_GPIO_PORT         GPIOB
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_13
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource13
#define KEY1_INT_EXTI_LINE         EXTI_Line13
#define KEY1_INT_EXTI_IRQ          EXTI15_10_IRQn
#define KEY1_IRQHandler            EXTI15_10_IRQHandler


#define KEY2_INT_GPIO_PORT         GPIOB
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_15
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource15
#define KEY2_INT_EXTI_LINE         EXTI_Line15
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn
#define KEY2_IRQHandler            EXTI15_10_IRQHandler


#define KEY3_INT_GPIO_PORT         GPIOB
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_14
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource14
#define KEY3_INT_EXTI_LINE         EXTI_Line14
#define KEY3_INT_EXTI_IRQ          EXTI15_10_IRQn
#define KEY3_IRQHandler            EXTI15_10_IRQHandler


//编码器
#define KEYA_INT_GPIO_PORT         GPIOB
#define KEYA_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEYA_INT_GPIO_PIN          GPIO_Pin_4
#define KEYA_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEYA_INT_EXTI_PINSOURCE    GPIO_PinSource4
#define KEYA_INT_EXTI_LINE         EXTI_Line4
#define KEYA_INT_EXTI_IRQ          EXTI4_IRQn
#define KEYA_IRQHandler            EXTI4_IRQHandler

#define KEYB_INT_GPIO_PORT         GPIOB
#define KEYB_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEYB_INT_GPIO_PIN          GPIO_Pin_3

#define KEYD_INT_GPIO_PORT         GPIOB
#define KEYD_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)
#define KEYD_INT_GPIO_PIN          GPIO_Pin_9
#define KEYD_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define KEYD_INT_EXTI_PINSOURCE    GPIO_PinSource9
#define KEYD_INT_EXTI_LINE         EXTI_Line9
#define KEYD_INT_EXTI_IRQ          EXTI9_5_IRQn
#define KEYD_IRQHandler            EXTI9_5_IRQHandler

void Init_Key_GPIO(void);
void Key_Handle(volatile struct Oscilloscope *value);
void Init_EC11_GPIO(void);

#endif
