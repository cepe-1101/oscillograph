#include "led.h"

/*
*   函数内容：初始化LED引脚
*   函数参数：无
*   返回值：  无
*/
void Init_LED_GPIO(void)
{
  
  /*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED相关的GPIO外设时钟*/
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
		/*选择要控制的GPIO引脚*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_15;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIO*/
		GPIO_Init(GPIOC, &GPIO_InitStructure);	

    //默认不开灯
    CLose_LED(led2);
    CLose_LED(led3);
}

/*
*   函数内容：打开对应LED灯
*   函数参数：无
*   返回值：  无
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
*   函数内容：关闭对应LED灯
*   函数参数：无
*   返回值：  无
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
