#include "adc.h"
#include "main.h"
#define ADC_VALUE_NUM 300
//ADC�ɼ�����
uint16_t adc_value[ADC_VALUE_NUM];

/*
*   �������ݣ�ADC_DMA�����ʼ��
*   ������������
*   ����ֵ��  ��
*/
void ADC_DMA_Start(void)
{
  ADCx_GPIO_Config();
  ADCx_Mode_Config();
  ADC_DMA_Init();
  ADC_DMA_NVIC_Config();
}
/*
*   �������ݣ��õ�ADCֵ
*   ����������value--�����±�
*   ����ֵ��  ��������ADCֵ
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
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN ( ADC_GPIO_CLK, ENABLE );
	// ���� ADC IO ����ģʽ
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);			
}

/*
*   �������ݣ���ʼ��ADC
*   ������������
*   ����ֵ��    ��
*/
static void ADCx_Mode_Config(void)
{
  //ADC���
  ADC_InitTypeDef ADC_InitStructure;
  // ��ADCʱ��
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
  
  // ADC ģʽ����
	// ֻʹ��һ��ADC�����ڶ���ģʽ
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
		
	// ��ʼ��ADC
	ADC_Init(ADCx, &ADC_InitStructure);
	
	// ����ADCʱ��ΪPCLK2��8��Ƶ����9MHz
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
	
	// ���� ADC ͨ��ת��˳��Ϊ1����һ��ת��������ʱ��Ϊ239.5��ʱ������
	ADC_RegularChannelConfig(ADC1, ADC_CHANNEL, 1, ADC_SampleTime_239Cycles5);
	
	// ʹ��ADC DMA ����
	ADC_DMACmd(ADC1, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC1, ENABLE);
	
	// ��ʼ��ADC У׼�Ĵ���  
	ADC_ResetCalibration(ADC1);
	// �ȴ�У׼�Ĵ�����ʼ�����
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	// ADC��ʼУ׼
	ADC_StartCalibration(ADC1);
	// �ȴ�У׼���
	while(ADC_GetCalibrationStatus(ADC1));
  
   /* ʹ��ADC1DMA */
	ADC_DMACmd(ADC1, ENABLE);
	/* ����û�в����ⲿ����������ʹ���������ADCת�� */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
  
}

static void ADC_DMA_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  DMA_DeInit(DMA1_Channel1); //DMA��λ
  // ��DMAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  //����DMA
	DMA_DeInit(ADC_DMA_CHANNEL);
  
  // ���� DMA ��ʼ���ṹ��
	// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t )&( ADC1->DR );
	
	// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_value;
	
	// ����Դ��������
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	
	// �������Ĵ�СӦ�õ��ڴ洢���Ĵ�С
	DMA_InitStructure.DMA_BufferSize = ADC_VALUE_NUM;
	
	// ����Ĵ���ֻ��һ������ַ���õ���
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	// �洢����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	
	// �������ݴ�СΪ���֣��������ֽ�
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	
	// �洢�����ݴ�СҲΪ���֣����������ݴ�С��ͬ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	
	// ѭ������ģʽ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	

	// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	
	// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	
	// ��ʼ��DMA
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);
	
	// ʹ�� DMA ͨ��
	DMA_Cmd(ADC_DMA_CHANNEL , ENABLE);
  /* ʹ�ܴ�������жϣ��������ܽ��յ����ж� */
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
}
extern volatile struct Oscilloscope oscilloscope;
// �ж����ȼ�����
static void ADC_DMA_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  /* �����жϷ���Ϊ2 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	/* ����DMA1ͨ��1���ж� */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	/* ������ռ���ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/* ���ô����ȼ� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* ʹ���ж� */
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


