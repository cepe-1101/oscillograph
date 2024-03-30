#ifndef __DMA_H
#define __DMA_H

#include "stm32f10x.h" 
#include "stm32f10x_dma.h"
#include <stdio.h>

void DMA_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);

void DMA_ShowBMP(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t num,volatile uint8_t* showData);

void Init_ShowData(volatile uint8_t *showData);

void DMAdrawCurve(uint8_t yOffset,short int rawValue,volatile uint8_t *showData);

#endif
