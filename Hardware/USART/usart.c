#include "usart.h"

/*
*   函数内容：初始化打印串口
*   函数参数：bound--波特率
*   返回值：  无
*/
void Init_USART(uint32_t bound)
{
    //使能时钟
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    
    //复用功能设置
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
    
    //端口模式设置
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
    
    //端口模式设置
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
    
    //复位USART0
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);       //8位数据位宽
    usart_stop_bit_set(USART0, USART_STB_1BIT);         //1位停止位
    usart_parity_config(USART0, USART_PM_NONE);         //无校验位
    usart_baudrate_set(USART0, bound);                  //波特率
    usart_receive_config(USART0, USART_RECEIVE_ENABLE); //接收使能
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//发送使能
    
    //使能串口0
    usart_enable(USART0);
}

/*
*   函数内容：fputc端口重定向
*   函数参数：无
*   返回值：  无
*/
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}