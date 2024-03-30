#include "timer.h"
#include "adc.h"
#include "main.h"

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // ����Ƚ�ͨ��3-PA2 GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM2_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM2_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM2_CH3_PORT, &GPIO_InitStructure);
}
/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)
/*
*		�������ݣ���ʼ����ʱ�������PWM�ź�
*		������������
*		����ֵ��	��
*/
void Init_PWM_Output(uint32_t period,uint32_t pulse)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  //��ʼ��PWM_GPIO���
  GENERAL_TIM_GPIO_Config();
// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK,ENABLE);

/*--------------------ʱ���ṹ���ʼ��-------------------------*/
	// �������ڣ���������Ϊ100K
	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=period;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM2_Prescaler;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(GENERAL_TIM2,&TIM_TimeBaseStructure);

	/*--------------------����ȽϽṹ���ʼ��-------------------*/	
	// ����ΪPWMģʽ2
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	// ���ʹ��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// ���ͨ����ƽ��������,�������Ϊ��	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  
  // ����Ƚ�ͨ�� 3ռ�ձ�
	TIM_OCInitStructure.TIM_Pulse = pulse;
  
  //ʹ��TIM2��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC3Init(GENERAL_TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM2, TIM_OCPreload_Enable);
}

/*
*		�������ݣ�����PWMռ�ձ�
*		������������
*		����ֵ��  ��
*/
void Set_Output_PWMComparex(uint16_t value)
{
  TIM_SetCompare3(GENERAL_TIM2,value);
}

/*
*		�������ݣ��������ڣ���Ӧ����Ƶ��
*		������������
*		����ֵ��	��
*/
void Set_Output_Freq(uint32_t value)
{
    TIM_SetAutoreload(GENERAL_TIM2,value);
}

