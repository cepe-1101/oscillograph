#ifndef __FREQ_H
#define __FREQ_H

#include "stm32f10x.h"
#include "systick.h"
#include <stdio.h>
/************ͨ�ö�ʱ��TIM�������壬ֻ��TIM2��3��4************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������Ĭ��ʹ��TIM3

#define            GENERAL_TIM3                   TIM3
#define            GENERAL_TIM3_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM3_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM3_PERIOD            0XFFFF
#define            GENERAL_TIM3_PSC              (72-1)

// TIM ���벶��ͨ��GPIO��غ궨��
#define            GENERAL_TIM3_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            GENERAL_TIM3_CH1_PORT          GPIOA
#define            GENERAL_TIM3_CH1_PIN           GPIO_Pin_6
#define            GENERAL_TIM3_CHANNEL_1         TIM_Channel_1

// �ж���غ궨��
#define            GENERAL_TIM3_IT_CCx            TIM_IT_CC1
#define            GENERAL_TIM3_IRQ               TIM3_IRQn
#define            GENERAL_TIM3_INT_FUN           TIM3_IRQHandler

// ��ȡ����Ĵ���ֵ�����궨��
#define            GENERAL_TIM3_GetCapturex_FUN                 TIM_GetCapture1
// �����źż��Ժ����궨��
#define            GENERAL_TIM3_OCxPolarityConfig_FUN           TIM_OC1PolarityConfig

// ��������ʼ����
#define            GENERAL_TIM3_STRAT_ICPolarity                TIM_ICPolarity_Rising
// �����Ľ�������
#define            GENERAL_TIM3_END_ICPolarity                  TIM_ICPolarity_Falling

// ��ʱ�����벶���û��Զ�������ṹ������
typedef struct
{   
	uint8_t   Capture_FinishFlag;   // ���������־λ
	uint8_t   Capture_StartFlag;    // ����ʼ��־λ
	uint16_t  Capture_CcrValue;     // ����Ĵ�����ֵ
	uint16_t  Capture_Period;       // �Զ���װ�ؼĴ������±�־ 
}TIM_ICUserValueTypeDef;

extern TIM_ICUserValueTypeDef TIM_ICUserValueStructure;
// �ж����ȼ�����
static void GENERAL_TIM_NVIC_Config(void);
//��ʼ�����벶��GPIO
static void GENERAL_TIM_GPIO_Config(void) ;
//��ʼ�����벶��ʱ��
void Init_FreqTimer(void);
#endif
