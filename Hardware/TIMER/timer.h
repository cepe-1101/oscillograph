#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h" 
#include "delay.h"
#include <stdio.h>

#define PI 3.1415926
/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4��5************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM2_CH4����ͨ��Ϊ����PA3�������PWM

#define            GENERAL_TIM2                   TIM2
#define            GENERAL_TIM2_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM2_CLK               RCC_APB1Periph_TIM2
#define            GENERAL_TIM2_Period            9
#define            GENERAL_TIM2_Prescaler         71

// TIM2 ����Ƚ�ͨ��3
#define            GENERAL_TIM2_CH3_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM2_CH3_PORT          GPIOA
#define            GENERAL_TIM2_CH3_PIN           GPIO_Pin_2




void Init_PWM_Output(uint32_t period,uint32_t pulse);
void Set_Output_PWMComparex(uint16_t value);
void Set_Output_Freq(uint32_t value);
static void GENERAL_TIM_NVIC_Config(void);
#endif
