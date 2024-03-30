#ifndef __FREQ_H
#define __FREQ_H

#include "stm32f10x.h"
#include "systick.h"
#include <stdio.h>
/************通用定时器TIM参数定义，只限TIM2、3、4************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 我们这里默认使用TIM3

#define            GENERAL_TIM3                   TIM3
#define            GENERAL_TIM3_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM3_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM3_PERIOD            0XFFFF
#define            GENERAL_TIM3_PSC              (72-1)

// TIM 输入捕获通道GPIO相关宏定义
#define            GENERAL_TIM3_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM3_CH1_PORT          GPIOA
#define            GENERAL_TIM3_CH1_PIN           GPIO_Pin_6
#define            GENERAL_TIM3_CHANNEL_1         TIM_Channel_1

// 中断相关宏定义
#define            GENERAL_TIM3_IT_CCx            TIM_IT_CC1
#define            GENERAL_TIM3_IRQ               TIM3_IRQn
#define            GENERAL_TIM3_INT_FUN           TIM3_IRQHandler

// 获取捕获寄存器值函数宏定义
#define            GENERAL_TIM3_GetCapturex_FUN                 TIM_GetCapture1
// 捕获信号极性函数宏定义
#define            GENERAL_TIM3_OCxPolarityConfig_FUN           TIM_OC1PolarityConfig

// 测量的起始边沿
#define            GENERAL_TIM3_STRAT_ICPolarity                TIM_ICPolarity_Rising
// 测量的结束边沿
#define            GENERAL_TIM3_END_ICPolarity                  TIM_ICPolarity_Falling

// 定时器输入捕获用户自定义变量结构体声明
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // 捕获结束标志位
	uint8_t   Capture_StartFlag;    // 捕获开始标志位
	uint16_t  Capture_CcrValue;     // 捕获寄存器的值
	uint16_t  Capture_Period;       // 自动重装载寄存器更新标志 
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure;
// 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void);
//初始化输入捕获GPIO
static void GENERAL_TIM_GPIO_Config(void) ;
//初始化输入捕获定时器
void Init_FreqTimer(void);
#endif
