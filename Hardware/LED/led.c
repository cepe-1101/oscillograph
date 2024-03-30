#include "led.h"

/*
*   �������ݣ���ʼ��LED����
*   ������������
*   ����ֵ��  ��
*/
void Init_LED_GPIO(void)
{
  
  /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*����LED��ص�GPIO����ʱ��*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
		/*ѡ��Ҫ���Ƶ�GPIO����*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;	

		/*��������ģʽΪͨ���������*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*������������Ϊ50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*���ÿ⺯������ʼ��GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);	

    //Ĭ�ϲ�����
    CLose_LED(led2);
    CLose_LED(led3);
}

/*
*   �������ݣ��򿪶�ӦLED��
*   ������������
*   ����ֵ��  ��
*/
void Open_LED(uint8_t value)
{
    switch(value)
    {
        case 1:
            GPIO_ResetBits(GPIOC,GPIO_Pin_14);
            break;
        case 2:
            GPIO_ResetBits(GPIOC,GPIO_Pin_14);
            break;
        default:
            break;
    }
}

/*
*   �������ݣ��رն�ӦLED��
*   ������������
*   ����ֵ��  ��
*/
void CLose_LED(uint8_t value)
{
    switch(value)
    {
        case 1:
            GPIO_SetBits(GPIOC,GPIO_Pin_15);
            break;
        case 2:
            GPIO_SetBits(GPIOC,GPIO_Pin_15);
            break;
        default:
            break;
    }
}
