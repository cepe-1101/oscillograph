#include "stm32f10x.h"  // Device header
#include "main.h"
#include "tft_init.h"
#include "tft.h"
#include "SysTick.h"
#include "led.h"
#include "adc.h"
#include "timer.h"
#include "key.h"
#include "freq.h"

volatile struct Oscilloscope oscilloscope={0};

void Init_Oscilloscope(volatile struct Oscilloscope *value);
 
int main(void)
{
  uint16_t i=0;
  
  //�м�ֵ
  float median=0;
  
  //���ֵ
  float voltage=0,vpp=0;
  
  //������ѹֵ
  float max_data=1.0f;
  
  //���ηŴ���
  float gainFactor=0;
  
  //�����ر��
  uint16_t Trigger_number=0;

  //��ʼ��ʾ��������
  Init_Oscilloscope(&oscilloscope);

  //ʱ�ӳ�ʼ��
  SysTick_Init();

  //LED��ʼ��
  Init_LED_GPIO();

  //��Ļ��ʼ��
  TFT_Init();

  //��ʼ��ADC\DMA����
  ADC_DMA_Start();

  //��ʼ��EC11����
  Init_EC11_GPIO();
  
  //��ʼ����������
  Init_Key_GPIO();
  
  //��ʼ��PWM���
  Init_PWM_Output(oscilloscope.timerPeriod-1,oscilloscope.pwmOut);
  
  //��ʼ��Ƶ�ʶ�ʱ��
  Init_FreqTimer();
  
  //����ɫ
  TFT_Fill(0,0,160,128,BLACK);
  
  //��ʼ����̬UI
  TFT_StaticUI();
  while(1)
  {
    //����ɨ�账����
    Key_Handle(&oscilloscope);
    //�����ȡ��ѹֵ��ɣ���ʼˢ��
    if(oscilloscope.showbit==1)
    {           
      oscilloscope.showbit=0;
      oscilloscope.vpp=0;
      //ת����ѹֵ
      for(i=0;i<300;i++)
      {
        oscilloscope.voltageValue[i]=(Get_ADC_Value(i)*3.3f)/4096.0f;
        
        //��ȡ�������ݵķ�ֵ��ѹ
        vpp=(5-(2.0f*oscilloscope.voltageValue[i]));
//        vpp=oscilloscope.voltageValue[i];
        if((oscilloscope.vpp) < vpp)
        {
          oscilloscope.vpp = vpp;
        }
        if(oscilloscope.vpp <= 0.3)
        {
          oscilloscope.gatherFreq=0;
        }
      }
        
      //ˢ����ͬʱ��ȡ��ѹֵ
      DMA_Cmd(DMA1_Channel1,ENABLE);
        
      //����������ѹ
      for(i=0;i<200;i++)
      {
        //�ҵ�һ��С�ڴ�����ѹ�Ĳɼ���
        if(oscilloscope.voltageValue[i] < max_data)
        {
          for(;i<200;i++)
          {
            //��С�ڴ�����ѹ�������һ�����ڴ�����ѹ�Ĳɼ��㣬�Դ˵�Ϊ������ѹ���Ʋ���
            if(oscilloscope.voltageValue[i] > max_data)
            {
              Trigger_number=i;
              break;
            }
          }
          break;
        }
      }
        
        //�����ֵ��С������ַŴ��������²�����ʾ�쳣������
        if(oscilloscope.vpp > 0.3)
        {
            //�����м���ȣ�������͵���2.5V�������(5-vpp)/2,�м���Ⱦ������ֵ��ȥ���ֵ/2��
            median = (2.5-(5 - oscilloscope.vpp)/2.0f)/2.0f;
            
            //�Ŵ�������Ҫȷ���Ŵ�֮������䣬�ҽ����ι̶���ʾ�ڣ�18.75~41.25�У���(41.25-18.75)/2=11.25f
            gainFactor = 11.25f/median;
            
            //��͵ļ�ȥ�м���ȵ����м�ֵ
            median = 2.5 - median;
        }
        
        //������ʾ����100�����ݣ��������Է�ֹ���ι���
        for(i=Trigger_number;i<Trigger_number+100;i++)
        {
            if(oscilloscope.keyValue == KEYD)
            {
                oscilloscope.keyValue=0;
                do
                {
                    if(oscilloscope.keyValue == KEYD){
                        oscilloscope.keyValue=0;
                        break;
                    }
                }while(1);
            }
            voltage=oscilloscope.voltageValue[i];

            if(voltage >= median)
            {
                voltage = 30 - (voltage - median)*gainFactor;
            }
            else
            {
                voltage = 30 + (median - voltage)*gainFactor;
            }
            drawCurve(80,voltage);
        }          
    } 
    
  }    
  
}
/*
*   �������ݣ���ʼ��ʾ���������ṹ��
*   ����������volatile struct Oscilloscope *value--ʾ���������ṹ��ָ��
*   ����ֵ����
*/
void Init_Oscilloscope(volatile struct Oscilloscope *value)
{

    (*value).showbit    =0;                         //�����ʾ��־λ
    (*value).sampletime =ADC_SampleTime_239Cycles5;  //adc��������
    (*value).keyValue   =0;                         //�������ֵ
    (*value).ouptputbit =0;                         //�����־λ
    (*value).gatherFreq =0;                         //�ɼ�Ƶ��
    (*value).outputFreq =1000;                      //���Ƶ��
    (*value).pwmOut     =500;                       //PWM���������PWMռ�ձ�
    (*value).timerPeriod=1000;                      //PWM�����ʱ������
    (*value).vpp        =0.0f;                      //���ֵ
}


