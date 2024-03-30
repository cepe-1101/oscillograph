#include "tft_init.h"
#include "delay.h"
/*
*   函数内容：初始化SPI0引脚
*   函数参数：无
*   返回值：无
*/
static void Init_SPI0_GPIO(void)
{
  //SPI句柄
  SPI_InitTypeDef SPI_InitStructure;
  //GPIO句柄
	GPIO_InitTypeDef GPIO_InitStructure;
  /* 使能SPI时钟 */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SPI1, ENABLE );
  /* 使能端口时钟 */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );
  /* 使能端口时钟 */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );
  /* 使能复用时钟 */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO, ENABLE );
  
  /************引脚配置*************/
  
	GPIO_InitStructure.GPIO_Pin = TFT_SPI_CS_PIN;  //SPI_NSS-CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_CS_PORT, &GPIO_InitStructure);
	TFT_CS_HIGH();//拉高CS电平
  
	GPIO_InitStructure.GPIO_Pin = TFT_SPI_SCK_PIN;		//SPI_SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_SCK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  TFT_SPI_MISO_PIN;		//SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TFT_SPI_MOSI_PIN;        //SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_MOSI_PORT, &GPIO_InitStructure);


  /* SPI 模式配置 */
  SPI_Cmd(TFT_SPIx, DISABLE);	 //必须先禁用,才能改变MODE
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//单线发送
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8bit数据位宽
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//时钟级性高
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//时钟触发模式第二边沿
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//软件触发片选
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//分频
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位在前
  SPI_InitStructure.SPI_CRCPolynomial = 7;//数据校验
  SPI_Init(TFT_SPIx , &SPI_InitStructure);

  /* 使能 SPI  */
  SPI_Cmd(TFT_SPIx , ENABLE);

}

/*
*   函数内容：SPI0发送数据
*   函数参数：无
*   返回值：无
*/
static void SPI0_Write(uint8_t TxData)
{
    //等待发送缓冲区空闲
    while(SPI_I2S_GetFlagStatus(TFT_SPIx,SPI_I2S_FLAG_TXE)==RESET);
    TFT_SPIx->DR=TxData; //通过外设SPIx发送一个byte  数据
    TFT_SPIx->SR&=(1<<0);
//  //经试验发现单线模式下在发送数据时不能使用SPI_I2S_FLAG_TXE来判断发送完成，
//  //只能使用SPI_I2S_FLAG_BSY标志来判断设备是否空闲。
//    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)== SET){}
}

/*
*   函数内容：初始化REST和BLK引脚
*   函数参数：无
*   返回值：无
*/
static void TFT_GPIO_Init(void)
{
  //GPIO初始化句柄
	GPIO_InitTypeDef GPIO_InitStructure;  
  
  //配置REST接口
  RCC_APB2PeriphClockCmd(REST_GPIO_CLK,ENABLE);
  GPIO_InitStructure.GPIO_Pin = REST_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(REST_GPIO_PORT, &GPIO_InitStructure);
  
  //配置TFT_BLK
  RCC_APB2PeriphClockCmd(BLK_GPIO_CLK,ENABLE);
  GPIO_InitStructure.GPIO_Pin = BLK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(BLK_GPIO_PORT, &GPIO_InitStructure);
  
  //配置TFT_DC
  RCC_APB2PeriphClockCmd(DC_GPIO_CLK,ENABLE);
  GPIO_InitStructure.GPIO_Pin = DC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DC_GPIO_PORT, &GPIO_InitStructure);
}

/*
*   函数内容：TFT发送单个字节数据
*   函数参数：无
*   返回值：无
*/
void TFT_WR_DATA8(uint8_t data)
{
    SPI_TFT_CS_LOW();   //拉低片选信号
    
    SPI0_Write(data);
    
    SPI_TFT_CS_HIGH();     //拉高片选信号
}

/*
*   函数内容：TFT发送2个字节数据
*   函数参数：无
*   返回值：无
*/
void TFT_WR_DATA(uint16_t data)
{
    SPI_TFT_CS_LOW();   //拉低片选信号
    
    SPI0_Write(data>>8);
    
    SPI0_Write(data);
    
    SPI_TFT_CS_HIGH();     //拉高片选信号
}

/*
*   函数内容：TFT发送命令数据
*   函数参数：无
*   返回值：无
*/
void TFT_WR_REG(uint8_t reg)
{
    SPI_TFT_CS_LOW();   //拉低片选信号
    TFT_DC_LOW();       //拉低片选信号
    
    SPI0_Write(reg);
    
    TFT_DC_HIGH();     //拉高命令信号
    TFT_DC_HIGH();;     //拉高片选信号
}


void TFT_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		TFT_WR_REG(0x2a);//列地址设置
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//行地址设置
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==1)
	{
		TFT_WR_REG(0x2a);//列地址设置
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//行地址设置
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//储存器写
	}
	else if(USE_HORIZONTAL==2)
	{
		TFT_WR_REG(0x2a);//列地址设置
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//行地址设置
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//储存器写
	}
	else
	{
		TFT_WR_REG(0x2a);//列地址设置
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//行地址设置
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//储存器写
	}    
}

void TFT_Init(void)
{
    TFT_GPIO_Init();
    Init_SPI0_GPIO();
    
    TFT_REST_LOW();   //复位
    delay_ms(100);
    TFT_REST_HIGH();     //
    delay_ms(100);
    
    TFT_BLK_HIGH();     //打开背光
    delay_ms(100);
    
	//************* Start Initial Sequence **********//
	TFT_WR_REG(0x11); //Sleep out 
	delay_ms(120);              //Delay 120ms 
	//------------------------------------ST7735S Frame Rate-----------------------------------------// 
	TFT_WR_REG(0xB1); 
	TFT_WR_DATA8(0x05); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_REG(0xB2); 
	TFT_WR_DATA8(0x05);
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_REG(0xB3); 
	TFT_WR_DATA8(0x05); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x05); 
	TFT_WR_DATA8(0x3C); 
	TFT_WR_DATA8(0x3C); 
	//------------------------------------End ST7735S Frame Rate---------------------------------// 
	TFT_WR_REG(0xB4); //Dot inversion 
	TFT_WR_DATA8(0x03); 
	//------------------------------------ST7735S Power Sequence---------------------------------// 
	TFT_WR_REG(0xC0); 
	TFT_WR_DATA8(0x28); 
	TFT_WR_DATA8(0x08); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_REG(0xC1); 
	TFT_WR_DATA8(0XC0); 
	TFT_WR_REG(0xC2); 
	TFT_WR_DATA8(0x0D); 
	TFT_WR_DATA8(0x00); 
	TFT_WR_REG(0xC3); 
	TFT_WR_DATA8(0x8D); 
	TFT_WR_DATA8(0x2A); 
	TFT_WR_REG(0xC4); 
	TFT_WR_DATA8(0x8D); 
	TFT_WR_DATA8(0xEE); 
	//---------------------------------End ST7735S Power Sequence-------------------------------------// 
	TFT_WR_REG(0xC5); //VCOM 
	TFT_WR_DATA8(0x1A); 
	TFT_WR_REG(0x36); //MX, MY, RGB mode 
	if(USE_HORIZONTAL==0){
        TFT_WR_DATA8(0x00);
    }
	else if(USE_HORIZONTAL==1){
        TFT_WR_DATA8(0xC0);
    }
	else if(USE_HORIZONTAL==2){
        TFT_WR_DATA8(0x70);
    }
	else {
        TFT_WR_DATA8(0xA0); 
    }
	//------------------------------------ST7735S Gamma Sequence---------------------------------// 
	TFT_WR_REG(0xE0); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_DATA8(0x22); 
	TFT_WR_DATA8(0x07); 
	TFT_WR_DATA8(0x0A); 
	TFT_WR_DATA8(0x2E); 
	TFT_WR_DATA8(0x30); 
	TFT_WR_DATA8(0x25); 
	TFT_WR_DATA8(0x2A); 
	TFT_WR_DATA8(0x28); 
	TFT_WR_DATA8(0x26); 
	TFT_WR_DATA8(0x2E); 
	TFT_WR_DATA8(0x3A); 
	TFT_WR_DATA8(0x00); 
	TFT_WR_DATA8(0x01); 
	TFT_WR_DATA8(0x03); 
	TFT_WR_DATA8(0x13); 
	TFT_WR_REG(0xE1); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_DATA8(0x16); 
	TFT_WR_DATA8(0x06); 
	TFT_WR_DATA8(0x0D); 
	TFT_WR_DATA8(0x2D); 
	TFT_WR_DATA8(0x26); 
	TFT_WR_DATA8(0x23); 
	TFT_WR_DATA8(0x27); 
	TFT_WR_DATA8(0x27); 
	TFT_WR_DATA8(0x25); 
	TFT_WR_DATA8(0x2D); 
	TFT_WR_DATA8(0x3B); 
	TFT_WR_DATA8(0x00); 
	TFT_WR_DATA8(0x01); 
	TFT_WR_DATA8(0x04); 
	TFT_WR_DATA8(0x13); 
	//------------------------------------End ST7735S Gamma Sequence-----------------------------// 
	TFT_WR_REG(0x3A); //65k mode 
	TFT_WR_DATA8(0x05); 
	TFT_WR_REG(0x29); //Display on   
}

