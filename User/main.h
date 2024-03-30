#ifndef MAIN_H_
#define MAIN_H_
#include "stdint.h"
struct Oscilloscope
{
   uint8_t showbit;         //������±�־λ
   uint8_t keyValue;        //����ֵ
   uint8_t ouptputbit;      //�����־λ
   uint16_t outputFreq;     //�������Ƶ��
   uint16_t pwmOut;         //PWM���������PWMռ�ձ�
   uint32_t sampletime;     //��ѹ�ɼ�ʱ��
   uint32_t timerPeriod;    //��ʱ����������
   float gatherFreq;        //ʾ�����ɼ�Ƶ��
   float vpp;               //���ֵ
   float voltageValue[300]; //ADC�ɼ���ѹֵ
};

#endif
