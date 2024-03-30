#include "key.h"
#include "timer.h"
#include "tft.h"
#include "delay.h"
static uint8_t keyValue=0;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
	/* ��ʾ NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* �����ж�Դ������2������ʹ������������� */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
  
  /* �����ж�Դ������3������ʹ������������� */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY3_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
  
  /* �����ж�Դ������A������ʹ������������� */  
  NVIC_InitStructure.NVIC_IRQChannel = KEYA_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
  /* �����ж�Դ������D������ʹ������������� */  
  NVIC_InitStructure.NVIC_IRQChannel = KEYD_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}


/*
*   �������ݣ���ʼ������GPIO
*   ������������
*   ����ֵ��  ��
*/
void Init_Key_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
  
  /*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
  RCC_APB2PeriphClockCmd(KEY2_INT_GPIO_CLK,ENABLE);
  RCC_APB2PeriphClockCmd(KEY3_INT_GPIO_CLK,ENABLE);
  
  /* ���� NVIC �ж�*/
	NVIC_Configuration();
  /*--------------------------KEY1����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �½����ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  //�����־λ
  EXTI_ClearFlag(KEY1_INT_EXTI_LINE);
  
  /*--------------------------KEY2����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �½����ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //�����־λ
  EXTI_ClearFlag(KEY2_INT_EXTI_LINE);
  
  /*--------------------------KEY3����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);

	/* �����ж��� */
  GPIO_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �½����ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //�����־λ
  EXTI_ClearFlag(KEY3_INT_EXTI_LINE);
  
}


/*
*   �������ݣ���ʼ��EC11 GPIO
*   ������������
*   ����ֵ��  ��
*/
void Init_EC11_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
  
  /*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
  
  /* ���� NVIC �ж�*/
	NVIC_Configuration();
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
  /*--------------------------KEYA����-----------------------------*/ 
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEYA_INT_GPIO_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEYA_INT_GPIO_PORT, &GPIO_InitStructure);

	/* �����ж��� */
  GPIO_EXTILineConfig(KEYA_INT_EXTI_PORTSOURCE, KEYA_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEYA_INT_EXTI_LINE;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //�����־λ
  EXTI_ClearFlag(KEYA_INT_EXTI_LINE);
  
    /*--------------------------KEYB����-----------------------------*/ 
  /* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /*--------------------------KEYD����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEYD_INT_GPIO_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEYD_INT_GPIO_PORT, &GPIO_InitStructure);

	/* �����ж��� */
  GPIO_EXTILineConfig(KEYD_INT_EXTI_PORTSOURCE, KEYD_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEYD_INT_EXTI_LINE;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //�����־λ
  EXTI_ClearFlag(KEYD_INT_EXTI_LINE);
 
}

/*
*   �������ݣ�����������
*   ����������Oscilloscope *value��ָ��Oscilloscope�ṹ��valueֵΪ��������״̬��
*   ����ֵ��  ��
*/
void Key_Handle(volatile struct Oscilloscope *value)
{
  float tempValue=0;
	switch((*value).keyValue)
	{
    //KEY1������������
		case KEY1:
          //ÿ�ΰ���ռ�ձ�5%����
          (*value).pwmOut=((*value).timerPeriod*0.05f)+(*value).pwmOut;
          //ռ�ձȴ�������
          if((*value).pwmOut > (*value).timerPeriod)
          {
            //ռ�ձ�Ϊ0
            (*value).pwmOut = 0;
          }
          //����PWMռ�ձ����
          Set_Output_PWMComparex((*value).pwmOut);
        break;
    //KEY2������������
    case KEY2:
          //PWM�����־λΪ0
          if((*value).ouptputbit == 0)
          {
            //�������־λ��1
            (*value).ouptputbit=1;
            //����PWM��ʱ��
            Set_Output_PWMComparex((*value).pwmOut);
            TIM_Cmd(TIM2, ENABLE);
          }
          //��־λ��Ϊ���ʾPWM�����Ѵ�
          else
          {
            //PWM���ܱ�־λ��0
            (*value).ouptputbit=0;
            //�رն�ʱ��2
            Set_Output_PWMComparex(0);
            delay_ms(10); 
          }
        break;
    //KEY3������������
    case KEY3:
            //���㵱ǰռ�ձ�
            tempValue=(*value).pwmOut/((*value).timerPeriod+0.0f);
            //���㵱ǰ��������
            (*value).timerPeriod = (*value).timerPeriod/2.0f;
            if((*value).timerPeriod < 250)
            {
                (*value).timerPeriod = 1000;
            }
            (*value).outputFreq=1000000/(*value).timerPeriod;
            (*value).pwmOut=(*value).timerPeriod*tempValue;
            Set_Output_PWMComparex((*value).pwmOut);
            Set_Output_Freq((*value).timerPeriod-1);
            tempValue=0;
			break;
      //��ʱ����ת
    case KEYA:
           switch((*value).sampletime)
            {
              case ADC_SampleTime_28Cycles5:
                  (*value).sampletime=ADC_SampleTime_41Cycles5;
                  break;
              case ADC_SampleTime_41Cycles5:
                  (*value).sampletime=ADC_SampleTime_55Cycles5;
                  break;
              case ADC_SampleTime_55Cycles5:
                  (*value).sampletime=ADC_SampleTime_71Cycles5;
                  break;
              case ADC_SampleTime_71Cycles5:
                  (*value).sampletime=ADC_SampleTime_239Cycles5;
                  break;
              case ADC_SampleTime_239Cycles5:
                  (*value).sampletime=ADC_SampleTime_239Cycles5;
                  break;
              default:
                  (*value).sampletime=ADC_SampleTime_239Cycles5;
                  break;
            }
            ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1,(*value).sampletime);               
      break;
    case KEYB:
            switch((*value).sampletime)
            {
                case ADC_SampleTime_239Cycles5:
                    (*value).sampletime=ADC_SampleTime_71Cycles5;
                    break;
                case ADC_SampleTime_71Cycles5:
                    (*value).sampletime=ADC_SampleTime_55Cycles5;
                    break;
                case ADC_SampleTime_55Cycles5:
                    (*value).sampletime=ADC_SampleTime_41Cycles5;
                    break;
                case ADC_SampleTime_41Cycles5:
                    (*value).sampletime=ADC_SampleTime_28Cycles5;
                    break;
                case ADC_SampleTime_28Cycles5:
                    (*value).sampletime=ADC_SampleTime_28Cycles5;
                    break;
                default:
                    (*value).sampletime=ADC_SampleTime_28Cycles5;
                    break;
            }    
           ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1,(*value).sampletime);                
      break;
    case KEYD:
        break;
  default:
      break;
	}
    (*value).keyValue=0;
    //������ʾUI
    TFT_ShowUI(value); 
}

extern volatile struct Oscilloscope oscilloscope;
//�������ж�
void KEYA_IRQHandler(void)
{
    static uint8_t clockwiseNum=0;
    static uint8_t anticlockwiseNum=0;
    if(EXTI_GetITStatus(EXTI_Line4)!=RESET) 
    {
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==RESET) //˳ʱ��ת����
        {
          delay_ms(5);
          anticlockwiseNum=0;
          if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==RESET) //˳ʱ��ת����
            {
                clockwiseNum++;
                if(clockwiseNum>=1)
                {
                    clockwiseNum=0;
                    oscilloscope.keyValue=KEYB; 
                }            
            }
        }
        else                                            //��ʱ��ת����
        {
            delay_ms(5);
            clockwiseNum=0;
            if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==SET) //��ʱ��ת����
            {
                anticlockwiseNum++;
                if(anticlockwiseNum>=1)
                {
                    anticlockwiseNum=0;
                    oscilloscope.keyValue=KEYA;
                }
            }
        }
          //�����־λ
          EXTI_ClearFlag(KEYA_INT_EXTI_LINE);
    }
}
//�����������ж�
void KEYD_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEYD_INT_EXTI_LINE)!=RESET)
    {
        delay_ms(5);
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==RESET)
        {
            oscilloscope.keyValue=KEYD;
        }
        //�����־λ
          EXTI_ClearFlag(KEYD_INT_EXTI_LINE);
    }
}
//���������ж�
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE)!=RESET)
  {
    delay_ms(5);
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==RESET)
    {    
        oscilloscope.keyValue=KEY1;
    }
    //�����־λ
    EXTI_ClearFlag(KEY1_INT_EXTI_LINE);
  }
  if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE)!=RESET)
  {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==RESET)
      {
          oscilloscope.keyValue=KEY2;
      }
      //�����־λ
      EXTI_ClearFlag(KEY2_INT_EXTI_LINE);
  }
  if(EXTI_GetITStatus(KEY3_INT_EXTI_LINE)!=RESET)
  {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET)
      {
          oscilloscope.keyValue=KEY3;
      }
      //�����־λ
      EXTI_ClearFlag(KEY3_INT_EXTI_LINE);
  }
}
