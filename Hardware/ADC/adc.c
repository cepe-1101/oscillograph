#include "adc.h"
#include "main.h"
#define ADC_VALUE_NUM 300
//ADC采集数组
uint16_t adc_value[ADC_VALUE_NUM];

/*
*   函数内容：ADC_DMA传输初始化
*   函数参数：无
*   返回值：  无
*/
void ADC_DMA_Start(void)
{
  ADCx_GPIO_Config();
  ADCx_Mode_Config();
  ADC_DMA_Init();
  ADC_DMA_NVIC_Config();
}
/*
*   函数内容：得到ADC值
*   函数参数：value--数组下标
*   返回值：  返回数组ADC值
*/
uint16_t Get_ADC_Value(uint16_t value)
{
    uint16_t returnValue=0;
    if(value>ADC_VALUE_NUM)
    {
        value=0;
    }
    returnValue=adc_value[value];
    adc_value[value]=0;
    return returnValue;
}

/**
  * @brief  ADC GPIO 初始化
  * @param  无
  * @retval 无
  */
static void ADCx_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	// 打开 ADC IO端口时钟
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	// 配置 ADC IO 引脚模式
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	// 初始化 ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);			
}

/*
*   函数内容：初始化ADC
*   函数参数：无
*   返回值：    无
*/
static void ADCx_Mode_Config(void)
{
  //ADC句柄
  ADC_InitTypeDef ADC_InitStructure;
  // 打开ADC时钟
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
  
  // ADC 模式配置
	// 只使用一个ADC，属于独立模式
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// 禁止扫描模式，多通道才要，单通道不需要
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// 连续转换模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// 不用外部触发转换，软件开启即可
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// 转换结果右对齐
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// 转换通道1个
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// 初始化ADC
	ADC_Init(ADCx, &ADC_InitStructure);
	
	// 配置ADC时钟为PCLK2的8分频，即9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// 配置 ADC 通道转换顺序为1，第一个转换，采样时间为239.5个时钟周期
	ADC_RegularChannelConfig(ADC1, ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
	
	// 使能ADC DMA 请求
	ADC_DMACmd(ADC1, ENABLE);
	
	// 开启ADC ，并开始转换
	ADC_Cmd(ADC1, ENABLE);
	
	// 初始化ADC 校准寄存器  
	ADC_ResetCalibration(ADC1);
	// 等待校准寄存器初始化完成
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC开始校准
	ADC_StartCalibration(ADC1);
	// 等待校准完成
	while(ADC_GetCalibrationStatus(ADC1));
  
   /* 使能ADC1DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* 由于没有采用外部触发，所以使用软件触发ADC转换 */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  
}

static void ADC_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  DMA_DeInit(DMA1_Channel1); //DMA复位
  // 打开DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  //配置DMA
	DMA_DeInit(ADC_DMA_CHANNEL);
  
  // 配置 DMA 初始化结构体
	// 外设基址为：ADC 数据寄存器地址
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t )&( ADC1->DR );
	
	// 存储器地址，实际上就是一个内部SRAM的变量
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_value;
	
	// 数据源来自外设
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// 缓冲区的大小应该等于存储器的大小
	DMA_InitStructure.DMA_BufferSize = ADC_VALUE_NUM;
	
	// 外设寄存器只有一个，地址不用递增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// 存储器地址递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// 外设数据大小为半字，即两个字节
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// 存储器数据大小也为半字，跟外设数据大小相同
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// 循环传输模式
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// 禁止存储器到存储器模式，因为是从外设到存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// 初始化DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// 使能 DMA 通道
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
  /* 使能传输完毕中断，这样才能接收到该中断 */
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
}
extern volatile struct Oscilloscope oscilloscope;
// 中断优先级配置
static void ADC_DMA_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* 设置中断分组为2 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* 设置DMA1通道1的中断 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	/* 设置抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/* 设置次优先级 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能中断 */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void DMA1_Channel1_IRQHandler(void) 
{
  if (DMA_GetITStatus(DMA1_IT_TC1)==SET) 
  {
    DMA_Cmd(ADC_DMA_CHANNEL,DISABLE);
    DMA_ClearITPendingBit(DMA1_IT_TC1);
    oscilloscope.showbit=1;
     }
 }


