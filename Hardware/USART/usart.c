#include "usart.h"

/*
*   �������ݣ���ʼ����ӡ����
*   ����������bound--������
*   ����ֵ��  ��
*/
void Init_USART(uint32_t bound)
{
    //ʹ��ʱ��
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    
    //���ù�������
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_10);
    
    //�˿�ģʽ����
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9);
    
    //�˿�ģʽ����
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
    
    //��λUSART0
    usart_deinit(USART0);
    usart_word_length_set(USART0, USART_WL_8BIT);       //8λ����λ��
    usart_stop_bit_set(USART0, USART_STB_1BIT);         //1λֹͣλ
    usart_parity_config(USART0, USART_PM_NONE);         //��У��λ
    usart_baudrate_set(USART0, bound);                  //������
    usart_receive_config(USART0, USART_RECEIVE_ENABLE); //����ʹ��
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);//����ʹ��
    
    //ʹ�ܴ���0
    usart_enable(USART0);
}

/*
*   �������ݣ�fputc�˿��ض���
*   ������������
*   ����ֵ��  ��
*/
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t) ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}