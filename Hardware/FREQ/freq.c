#include "freq.h"
#include "main.h"
// ��ʱ�����벶���û��Զ�������ṹ�嶨��
TIM_ICUserValueTypeDef TIM_ICUserValueStructure = {0,0,0,0};

// �ж����ȼ�����
static void GENERAL_TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
		// �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	
		// ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	  // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void GENERAL_TIM_GPIO_Config(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // ���벶��ͨ�� GPIO ��ʼ��
	RCC_APB2PeriphClockCmd(GENERAL_TIM3_CH1_GPIO_CLK, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GENERAL_TIM3_CH1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
  GPIO_Init(GENERAL_TIM3_CH1_PORT, &GPIO_InitStructure);	
  GPIO_ResetBits(GENERAL_TIM3_CH1_PORT,GENERAL_TIM3_CH1_PIN);
}

/* ----------------   PWM�ź� ���ں�ռ�ձȵļ���--------------- */
// ARR ���Զ���װ�ؼĴ�����ֵ
// CLK_cnt����������ʱ�ӣ����� Fck_int / (psc+1) = 72M/(psc+1)
// PWM �źŵ����� T = ARR * (1/CLK_cnt) = ARR*(PSC+1) / 72M
// ռ�ձ�P=CCR/(ARR+1)
void Init_FreqTimer(void)
{
  /*--------------------ʱ���ṹ���ʼ��-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  /*--------------------���벶��ṹ���ʼ��-------------------*/	
	TIM_ICInitTypeDef TIM_ICInitStructure;
  //��ʼ��GPIO
  GENERAL_TIM_GPIO_Config();
  
// ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
	GENERAL_TIM3_APBxClock_FUN(GENERAL_TIM3_CLK,ENABLE);

	// �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM3_PERIOD;	
	// ����CNT��������ʱ�� = Fck_int/(psc+1)
	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM3_PSC;	
	// ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	// ����������ģʽ������Ϊ���ϼ���
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	// �ظ���������ֵ��û�õ����ù�
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	// ��ʼ����ʱ��
	TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);

	// �������벶���ͨ������Ҫ���ݾ����GPIO������
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM3_CHANNEL_1;
	// ���벶���źŵļ�������
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM3_STRAT_ICPolarity;
	// ����ͨ���Ͳ���ͨ����ӳ���ϵ����ֱ���ͷ�ֱ������
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// �������Ҫ��������źŵķ�Ƶϵ��
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	// �������Ҫ��������źŵ��˲�ϵ��
	TIM_ICInitStructure.TIM_ICFilter = 0;
	// ��ʱ�����벶���ʼ��
	TIM_ICInit(GENERAL_TIM3, &TIM_ICInitStructure);
    //��ʼ��NVIC
  GENERAL_TIM_NVIC_Config();
  
	
	// ������ºͲ����жϱ�־λ
  TIM_ClearFlag(GENERAL_TIM3, TIM_FLAG_Update|GENERAL_TIM3_IT_CCx);	
  // �������ºͲ����ж�  
	TIM_ITConfig (GENERAL_TIM3, TIM_IT_Update | GENERAL_TIM3_IT_CCx, ENABLE );
	
	// ʹ�ܼ�����
	TIM_Cmd(GENERAL_TIM3, ENABLE);
}

uint16_t readvalue1 = 0, readvalue2 = 0;
__IO uint16_t ccnumber = 0;
__IO uint32_t count = 0;
__IO float freq=0;
extern volatile struct Oscilloscope oscilloscope;

void TIM3_IRQHandler(void)
{
	// ��Ҫ��������źŵ����ڴ��ڶ�ʱ�������ʱʱ����ʱ���ͻ���������������ж�
	// ���ʱ��������Ҫ�������Ķ�ʱ���ڼӵ������źŵ�ʱ������ȥ
  //TIM_GetITStatus():��ѯ�Ƿ�����ʱ�ж�
	if ( TIM_GetITStatus ( GENERAL_TIM3, TIM_IT_Update) != RESET )               
	{	
    //��ʱ����
		TIM_ICUserValueStructure.Capture_Period ++;		
    //�����ʱ��־λ
		TIM_ClearITPendingBit ( GENERAL_TIM3, TIM_FLAG_Update ); 		
	}

	// �����ز����ж�
	if ( TIM_GetITStatus (GENERAL_TIM3, GENERAL_TIM3_IT_CCx ) != RESET)
	{
		// ��һ�β���
		if ( TIM_ICUserValueStructure.Capture_StartFlag == 0 )
		{
			// ��������0
			TIM_SetCounter ( GENERAL_TIM3, 0 );
			// �Զ���װ�ؼĴ������±�־��0
			TIM_ICUserValueStructure.Capture_Period = 0;
      // �沶��ȽϼĴ�����ֵ�ı�����ֵ��0			
			TIM_ICUserValueStructure.Capture_CcrValue = 0;

//			// ����һ�β���������֮�󣬾ͰѲ����������Ϊ�½���
//			GENERAL_TIM3_OCxPolarityConfig_FUN(GENERAL_TIM3, TIM_ICPolarity_Falling);
      // ��ʼ�����׼��1			
			TIM_ICUserValueStructure.Capture_StartFlag = 1;			
		}
		// �½��ز����ж�
		else if(TIM_ICUserValueStructure.Capture_StartFlag == 1 ) // �ڶ��β���
		{
			// ��ȡ����ȽϼĴ�����ֵ�����ֵ���ǲ����ź�����ʱ��
			TIM_ICUserValueStructure.Capture_CcrValue = GENERAL_TIM3_GetCapturex_FUN(GENERAL_TIM3);
//			// ���ڶ��β����½���֮�󣬾ͰѲ����������Ϊ�����أ����������أ�������ڲ���
//			GENERAL_TIM3_OCxPolarityConfig_FUN(GENERAL_TIM3, TIM_ICPolarity_Rising);    
       //�Զ���װ�ؼĴ������±�־*��ʱ�����ʱ��+��ʱ������ʱ�䣬��ʱ��������1M��ʱ��,ʱ�䵥λ��Ҫ���л�����ܵõ���/��
      oscilloscope.gatherFreq=(1.0f/(TIM_ICUserValueStructure.Capture_Period * (GENERAL_TIM3_PERIOD+1) + (TIM_ICUserValueStructure.Capture_CcrValue+1)))*1000000;
      // ��ʼ�����־��0		
			TIM_ICUserValueStructure.Capture_StartFlag = 0;
      // ������ɱ�־��1			
			TIM_ICUserValueStructure.Capture_FinishFlag = 1;  
    }
		TIM_ClearITPendingBit (GENERAL_TIM3,GENERAL_TIM3_IT_CCx);	    
	}		
}
