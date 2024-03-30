#include "timer.h"
#include "adc.h"
#include "main.h"

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // 输出比较通道3-PA2 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM2_CH3_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM2_CH3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GENERAL_TIM2_CH3_PORT, &GPIO_InitStructure);
}
/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)
/*
*		函数内容：初始化定时器，输出PWM信号
*		函数参数：无
*		返回值：	无
*/
void Init_PWM_Output(uint32_t period,uint32_t pulse)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  //初始化PWM_GPIO输出
  GENERAL_TIM_GPIO_Config();
// 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK,ENABLE);

/*--------------------时基结构体初始化-------------------------*/
	// 配置周期，这里配置为100K
	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=period;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM2_Prescaler;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL_TIM2,&TIM_TimeBaseStructure);

	/*--------------------输出比较结构体初始化-------------------*/	
	// 配置为PWM模式2
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// 输出通道电平极性配置,输出极性为低	
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  
  // 输出比较通道 3占空比
	TIM_OCInitStructure.TIM_Pulse = pulse;
  
  //使能TIM2在CCR2上的预装载寄存器
	TIM_OC3Init(GENERAL_TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(GENERAL_TIM2, TIM_OCPreload_Enable);
}

/*
*		函数内容：设置PWM占空比
*		函数参数：无
*		返回值：  无
*/
void Set_Output_PWMComparex(uint16_t value)
{
  TIM_SetCompare3(GENERAL_TIM2,value);
}

/*
*		函数内容：设置周期，对应设置频率
*		函数参数：无
*		返回值：	无
*/
void Set_Output_Freq(uint32_t value)
{
    TIM_SetAutoreload(GENERAL_TIM2,value);
}

