#ifndef __TFT_INIT_H
#define __TFT_INIT_H
#include "stm32f10x.h"                  // Device header

#include <stdio.h>
//#define TFT_SCL PA5*
//#define TFT_SDA PA7*
//#define TFT_RES PB5
//#define TFT_DC  PB6
//#define TFT_CS  PB7*
//#define TFT_BLK PB8

#define USE_HORIZONTAL 2  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

/*SPI接口定义-开头****************************/
#define      TFT_SPIx                        SPI1
//#define      TFT_SPI_APBxClock_FUN         RCC_APB2PeriphClockCmd
//#define      TFT_SPI_CLK                   RCC_APB2Periph_SPI1

//CS(NSS)引脚 片选选普通GPIO即可
#define      TFT_SPI_CS_APBxClock_FUN        RCC_APB2PeriphClockCmd
#define      TFT_SPI_CS_CLK                  RCC_APB2Periph_GPIOB    
#define      TFT_SPI_CS_PORT                 GPIOB
#define      TFT_SPI_CS_PIN                  GPIO_Pin_7

//SCK引脚
#define      TFT_SPI_SCK_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define      TFT_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define      TFT_SPI_SCK_PORT                GPIOA   
#define      TFT_SPI_SCK_PIN                 GPIO_Pin_5
//MISO引脚,示波器只使用半双工
#define      TFT_SPI_MISO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      TFT_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define      TFT_SPI_MISO_PORT               GPIOA 
#define      TFT_SPI_MISO_PIN                GPIO_Pin_6
//MOSI引脚
#define      TFT_SPI_MOSI_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define      TFT_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define      TFT_SPI_MOSI_PORT               GPIOA 
#define      TFT_SPI_MOSI_PIN                GPIO_Pin_7

#define  		SPI_TFT_CS_LOW()     						 GPIO_ResetBits( TFT_SPI_CS_PORT, TFT_SPI_CS_PIN )
#define  		SPI_TFT_CS_HIGH()    						 GPIO_SetBits( TFT_SPI_CS_PORT, TFT_SPI_CS_PIN )

/*SPI接口定义-结尾****************************/

//背光接口
#define      BLK_GPIO_PORT      GPIOB
#define      BLK_GPIO_CLK       RCC_APB2Periph_GPIOB    
#define      BLK_GPIO_PIN       GPIO_Pin_8 

         
//REST接口
#define      REST_GPIO_PORT      GPIOB
#define      REST_GPIO_CLK       RCC_APB2Periph_GPIOB    
#define      REST_GPIO_PIN       GPIO_Pin_5  


//DC接口,此接口为TFT屏幕命令接口
#define      DC_GPIO_PORT       GPIOB
#define      DC_GPIO_CLK        RCC_APB2Periph_GPIOB    
#define      DC_GPIO_PIN        GPIO_Pin_6 

#define TFT_SCLK_LOW()      GPIO_ResetBits(GPIOA,GPIO_Pin_5)//SCL=SCLK
#define TFT_SCLK_HIGH()     GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define TFT_MOSI_LOW()      GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SDA=MOSI
#define TFT_MOSI_HIGH()     GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define TFT_REST_LOW()     	GPIO_ResetBits( REST_GPIO_PORT, REST_GPIO_PIN )//REST
#define TFT_REST_HIGH()    	GPIO_SetBits( REST_GPIO_PORT, REST_GPIO_PIN )

#define TFT_DC_LOW()        GPIO_ResetBits( DC_GPIO_PORT, DC_GPIO_PIN )//DC
#define TFT_DC_HIGH()       GPIO_SetBits( DC_GPIO_PORT, DC_GPIO_PIN )
 		     
#define TFT_CS_LOW()        GPIO_ResetBits(GPIOB,GPIO_Pin_7)//CS
#define TFT_CS_HIGH()       GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define TFT_BLK_LOW()     	GPIO_ResetBits( BLK_GPIO_PORT, BLK_GPIO_PIN )//BLK
#define TFT_BLK_HIGH()    	GPIO_SetBits( BLK_GPIO_PORT, BLK_GPIO_PIN )



void TFT_WR_DATA8(uint8_t TxData);
void TFT_WR_DATA(uint16_t data);
void TFT_WR_REG(uint8_t reg);
void TFT_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void TFT_Init(void);

#endif
