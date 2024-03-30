#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"
#include <stdio.h>

// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
/********************ADC1输入通道（引脚）配置**************************/
#define    ADCx                          ADC1
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_CLK                       RCC_APB2Periph_ADC1

#define    ADC_GPIO_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_APB2Periph_GPIOA  
#define    ADC_PORT                      GPIOA

// PA3-通道3 独立IO
#define    ADC_PIN                       GPIO_Pin_3
#define    ADC_CHANNEL                   ADC_Channel_3
#define    ADC_DMA_CHANNEL               DMA1_Channel1

void ADC_DMA_Start(void);
uint16_t Get_ADC_Value(uint16_t value);
static void ADCx_GPIO_Config(void);
static void ADCx_Mode_Config(void);
static void ADC_DMA_Init(void);
static void ADC_DMA_NVIC_Config(void);


#endif
