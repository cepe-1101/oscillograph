#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h" 
#include "delay.h"
#include <stdio.h>

#define PI 3.1415926
/************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM2_CH4，该通道为板子PA3引脚输出PWM

#define            GENERAL_TIM2                   TIM2
#define            GENERAL_TIM2_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM2_CLK               RCC_APB1Periph_TIM2
#define            GENERAL_TIM2_Period            9
#define            GENERAL_TIM2_Prescaler         71

// TIM2 输出比较通道3
#define            GENERAL_TIM2_CH3_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM2_CH3_PORT          GPIOA
#define            GENERAL_TIM2_CH3_PIN           GPIO_Pin_2




void Init_PWM_Output(uint32_t period,uint32_t pulse);
void Set_Output_PWMComparex(uint16_t value);
void Set_Output_Freq(uint32_t value);
static void GENERAL_TIM_NVIC_Config(void);
#endif
