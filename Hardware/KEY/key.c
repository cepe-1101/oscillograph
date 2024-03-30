#include "key.h"
#include "timer.h"
#include "tft.h"
#include "delay.h"
static uint8_t keyValue=0;

static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 配置NVIC为优先级组1 */
	/* 提示 NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置中断源：按键1 */
  NVIC_InitStructure.NVIC_IRQChannel = KEY1_INT_EXTI_IRQ;
  /* 配置抢占优先级 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 配置子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 配置中断源：按键2，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY2_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 配置中断源：按键3，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEY3_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 配置中断源：按键A，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEYA_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
  /* 配置中断源：按键D，其他使用上面相关配置 */  
  NVIC_InitStructure.NVIC_IRQChannel = KEYD_INT_EXTI_IRQ;
  NVIC_Init(&NVIC_InitStructure);
}


/*
*   函数内容：初始化按键GPIO
*   函数参数：无
*   返回值：  无
*/
void Init_Key_GPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
  
  /*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK,ENABLE);
  RCC_APB2PeriphClockCmd(KEY2_INT_GPIO_CLK,ENABLE);
  RCC_APB2PeriphClockCmd(KEY3_INT_GPIO_CLK,ENABLE);
  
  /* 配置 NVIC 中断*/
	NVIC_Configuration();
  /*--------------------------KEY1配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
  /* 配置为上拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  //清除标志位
  EXTI_ClearFlag(KEY1_INT_EXTI_LINE);
  
  /*--------------------------KEY2配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
  /* 配置为上拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 选择EXTI的信号源 */
  GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //清除标志位
  EXTI_ClearFlag(KEY2_INT_EXTI_LINE);
  
  /*--------------------------KEY3配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
  /* 配置为上拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 配置中断线 */
  GPIO_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 下降沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //清除标志位
  EXTI_ClearFlag(KEY3_INT_EXTI_LINE);
  
}


/*
*   函数内容：初始化EC11 GPIO
*   函数参数：无
*   返回值：  无
*/
void Init_EC11_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
  
  /*开启按键GPIO口的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
  
  /* 配置 NVIC 中断*/
	NVIC_Configuration();
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
  /*--------------------------KEYA配置-----------------------------*/ 
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEYA_INT_GPIO_PIN;
  /* 配置为上拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEYA_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 配置中断线 */
  GPIO_EXTILineConfig(KEYA_INT_EXTI_PORTSOURCE, KEYA_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEYA_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //清除标志位
  EXTI_ClearFlag(KEYA_INT_EXTI_LINE);
  
    /*--------------------------KEYB配置-----------------------------*/ 
  /* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  /* 配置为上拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    /*--------------------------KEYD配置-----------------------------*/
	/* 选择按键用到的GPIO */	
  GPIO_InitStructure.GPIO_Pin = KEYD_INT_GPIO_PIN;
  /* 配置为上拉输入 */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(KEYD_INT_GPIO_PORT, &GPIO_InitStructure);

	/* 配置中断线 */
  GPIO_EXTILineConfig(KEYD_INT_EXTI_PORTSOURCE, KEYD_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = KEYD_INT_EXTI_LINE;
	
	/* EXTI为中断模式 */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* 上升沿中断 */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  /* 使能中断 */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
  //清除标志位
  EXTI_ClearFlag(KEYD_INT_EXTI_LINE);
 
}

/*
*   函数内容：按键处理函数
*   函数参数：Oscilloscope *value：指向Oscilloscope结构体value值为按键按下状态量
*   返回值：  无
*/
void Key_Handle(volatile struct Oscilloscope *value)
{
  float tempValue=0;
	switch((*value).keyValue)
	{
    //KEY1按键触发函数
		case KEY1:
          //每次按下占空比5%递增
          (*value).pwmOut=((*value).timerPeriod*0.05f)+(*value).pwmOut;
          //占空比大于周期
          if((*value).pwmOut > (*value).timerPeriod)
          {
            //占空比为0
            (*value).pwmOut = 0;
          }
          //更新PWM占空比输出
          Set_Output_PWMComparex((*value).pwmOut);
        break;
    //KEY2按键触发函数
    case KEY2:
          //PWM输出标志位为0
          if((*value).ouptputbit == 0)
          {
            //将输出标志位置1
            (*value).ouptputbit=1;
            //开启PWM定时器
            Set_Output_PWMComparex((*value).pwmOut);
            TIM_Cmd(TIM2, ENABLE);
          }
          //标志位不为零表示PWM功能已打开
          else
          {
            //PWM功能标志位置0
            (*value).ouptputbit=0;
            //关闭定时器2
            Set_Output_PWMComparex(0);
            delay_ms(10); 
          }
        break;
    //KEY3按键触发函数
    case KEY3:
            //计算当前占空比
            tempValue=(*value).pwmOut/((*value).timerPeriod+0.0f);
            //计算当前设置周期
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
      //逆时针旋转
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
    //参数显示UI
    TFT_ShowUI(value); 
}

extern volatile struct Oscilloscope oscilloscope;
//编码器中断
void KEYA_IRQHandler(void)
{
    static uint8_t clockwiseNum=0;
    static uint8_t anticlockwiseNum=0;
    if(EXTI_GetITStatus(EXTI_Line4)!=RESET) 
    {
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==RESET) //顺时针转两格
        {
          delay_ms(5);
          anticlockwiseNum=0;
          if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==RESET) //顺时针转两格
            {
                clockwiseNum++;
                if(clockwiseNum>=1)
                {
                    clockwiseNum=0;
                    oscilloscope.keyValue=KEYB; 
                }            
            }
        }
        else                                            //逆时针转两格
        {
            delay_ms(5);
            clockwiseNum=0;
            if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==SET) //逆时针转两格
            {
                anticlockwiseNum++;
                if(anticlockwiseNum>=1)
                {
                    anticlockwiseNum=0;
                    oscilloscope.keyValue=KEYA;
                }
            }
        }
          //清除标志位
          EXTI_ClearFlag(KEYA_INT_EXTI_LINE);
    }
}
//编码器按键中断
void KEYD_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEYD_INT_EXTI_LINE)!=RESET)
    {
        delay_ms(5);
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)==RESET)
        {
            oscilloscope.keyValue=KEYD;
        }
        //清除标志位
          EXTI_ClearFlag(KEYD_INT_EXTI_LINE);
    }
}
//独立按键中断
void EXTI15_10_IRQHandler(void)
{
  if(EXTI_GetITStatus(KEY1_INT_EXTI_LINE)!=RESET)
  {
    delay_ms(5);
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==RESET)
    {    
        oscilloscope.keyValue=KEY1;
    }
    //清除标志位
    EXTI_ClearFlag(KEY1_INT_EXTI_LINE);
  }
  if(EXTI_GetITStatus(KEY2_INT_EXTI_LINE)!=RESET)
  {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==RESET)
      {
          oscilloscope.keyValue=KEY2;
      }
      //清除标志位
      EXTI_ClearFlag(KEY2_INT_EXTI_LINE);
  }
  if(EXTI_GetITStatus(KEY3_INT_EXTI_LINE)!=RESET)
  {
      delay_ms(5);
      if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET)
      {
          oscilloscope.keyValue=KEY3;
      }
      //清除标志位
      EXTI_ClearFlag(KEY3_INT_EXTI_LINE);
  }
}
