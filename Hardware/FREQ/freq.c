#include "freq.h"
#include "main.h"
// 定时器输入捕获用户自定义变量结构体定义
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};

// 中断优先级配置
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // 输入捕获通道 GPIO 初始化
	RCC_APB2PeriphClockCmd(GENERAL_TIM3_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM3_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GENERAL_TIM3_CH1_PORT, &GPIO_InitStructure);	
  GPIO_ResetBits(GENERAL_TIM3_CH1_PORT,GENERAL_TIM3_CH1_PIN);
}

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)
void Init_FreqTimer(void)
{
  /*--------------------时基结构体初始化-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  /*--------------------输入捕获结构体初始化-------------------*/	
	TIM_ICInitTypeDef TIM_ICInitStructure;
  //初始化GPIO
  GENERAL_TIM_GPIO_Config();
  
// 开启定时器时钟,即内部时钟CK_INT=72M
	GENERAL_TIM3_APBxClock_FUN(GENERAL_TIM3_CLK,ENABLE);

	// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM3_PERIOD;	
	// 驱动CNT计数器的时钟 = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM3_PSC;	
	// 时钟分频因子 ，配置死区时间时需要用到
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// 计数器计数模式，设置为向上计数
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// 重复计数器的值，没用到不用管
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// 初始化定时器
	TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);

	// 配置输入捕获的通道，需要根据具体的GPIO来配置
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM3_CHANNEL_1;
	// 输入捕获信号的极性配置
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM3_STRAT_ICPolarity;
	// 输入通道和捕获通道的映射关系，有直连和非直连两种
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// 输入的需要被捕获的信号的分频系数
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// 输入的需要被捕获的信号的滤波系数
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// 定时器输入捕获初始化
	TIM_ICInit(GENERAL_TIM3, &TIM_ICInitStructure);
    //初始化NVIC
  GENERAL_TIM_NVIC_Config();
  
	
	// 清除更新和捕获中断标志位
  TIM_ClearFlag(GENERAL_TIM3, TIM_FLAG_Update|GENERAL_TIM3_IT_CCx);	
  // 开启更新和捕获中断  
	TIM_ITConfig (GENERAL_TIM3, TIM_IT_Update | GENERAL_TIM3_IT_CCx, ENABLE );
	
	// 使能计数器
	TIM_Cmd(GENERAL_TIM3, ENABLE);
}

uint16_t readvalue1 = 0, readvalue2 = 0;
__IO uint16_t ccnumber = 0;
__IO uint32_t count = 0;
__IO float freq=0;
extern volatile struct Oscilloscope oscilloscope;

void TIM3_IRQHandler(void)
{
	// 当要被捕获的信号的周期大于定时器的最长定时时，定时器就会溢出，产生更新中断
	// 这个时候我们需要把这个最长的定时周期加到捕获信号的时间里面去
  //TIM_GetITStatus():查询是否发生超时中断
	if ( TIM_GetITStatus ( GENERAL_TIM3, TIM_IT_Update) != RESET )               
	{	
    //超时自增
		TIM_ICUserValueStructure.Capture_Period ++;		
    //清除超时标志位
		TIM_ClearITPendingBit ( GENERAL_TIM3, TIM_FLAG_Update ); 		
	}

	// 上升沿捕获中断
	if ( TIM_GetITStatus (GENERAL_TIM3, GENERAL_TIM3_IT_CCx ) != RESET)
	{
		// 第一次捕获
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// 计数器清0
			TIM_SetCounter ( GENERAL_TIM3, 0 );
			// 自动重装载寄存器更新标志清0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // 存捕获比较寄存器的值的变量的值清0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

//			// 当第一次捕获到上升沿之后，就把捕获边沿配置为下降沿
//			GENERAL_TIM3_OCxPolarityConfig_FUN(GENERAL_TIM3, TIM_ICPolarity_Falling);
      // 开始捕获标准置1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// 下降沿捕获中断
		else if(TIM_ICUserValueStructure.Capture_StartFlag == 1 ) // 第二次捕获
		{
			// 获取捕获比较寄存器的值，这个值就是捕获到信号周期时间
			TIM_ICUserValueStructure.Capture_CcrValue = GENERAL_TIM3_GetCapturex_FUN(GENERAL_TIM3);
//			// 当第二次捕获到下降沿之后，就把捕获边沿配置为上升沿，捕获上升沿，完成周期测量
//			GENERAL_TIM3_OCxPolarityConfig_FUN(GENERAL_TIM3, TIM_ICPolarity_Rising);    
       //自动重装载寄存器更新标志*定时器溢出时间+定时器捕获时间，定时器给的是1M的时钟,时间单位需要进行换算才能得到秒/次
      oscilloscope.gatherFreq=(1.0f/(TIM_ICUserValueStructure.Capture_Period * (GENERAL_TIM3_PERIOD+1) + (TIM_ICUserValueStructure.Capture_CcrValue+1)))*1000000;
      // 开始捕获标志清0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // 捕获完成标志置1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;  
    }
		TIM_ClearITPendingBit (GENERAL_TIM3,GENERAL_TIM3_IT_CCx);	    
	}		
}
