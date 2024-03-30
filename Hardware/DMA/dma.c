#include "dma.h"
#include "tft.h"
#include "tft_init.h"

uint16_t DMA1_MEM_LEN=0;
static uint16_t lastX=0,lastY=0;
static uint8_t firstPoint = 1;

/*
*   函数内容：DMA通道配置
*   函数参数：
*           uint32_t cpar--DMA要搬运到的外设地址
*           uint32_t cmar--DMA要搬运的内存的地址
*           uint16_t cndtr-要搬运的内存的大小
*   返回值：无
*/
void MYDMA_Config(uint32_t cpar,uint32_t cmar,uint16_t cndtr)
{
  //DMA参数结构体
  DMA_InitTypeDef DMA_InitStructure;
    
  // 开启DMA时钟
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  // 设置DMA源地址：TFT显存寄存器地址*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;
  
  // 内存地址(要传输的变量的指针)
  DMA_InitStructure.DMA_MemoryBaseAddr = cmar;
  // /数据传输量 ,可设置为0， 实际发送时会重新设置	
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
  // 传输大小	
  DMA_InitStructure.DMA_BufferSize = cndtr;
  // 外设地址不增	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// 内存地址自增
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		
    // 外设数据单位	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		// 内存数据单位
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;	 
		// DMA模式，一次或者循环模式
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
		// 优先级：中	
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
		// 禁止内存到内存的传输
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// 配置DMA通道		   
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);	
 /* 开启传输完成中断  */		
    DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);    
   DMA_Cmd(DMA1_Channel3,ENABLE);
}

/*
*   函数内容：使能dma1的通道3，开始搬运数据
*   函数参数：无
*   返回值：  无
*/
void DMA_SetDateCounter(void)
{ 
  //失能DMA通道2
  DMA_Cmd(DMA1_Channel3,DISABLE);
  //while (DMA_GetCmdStatus(DMA1) != DISABLE){}  //确保DMA可以被设置
  //修改DMA传输数据配置
  DMA_SetCurrDataCounter(DMA1_Channel3,);
  //使能DMA通道2，开始搬运
  DMA_Cmd(DMA1_Channel3,DISABLE);    
}

/*
*   函数内容：DMA填充函数
*   函数参数：
*               uint16_t xsta--起始X坐标
*               uint16_t ysta--起始Y坐标
*               uint16_t xend--结束X坐标
*               uint16_t yend--结束Y坐标
*               uint16_t color-填充颜色值
*/
void DMA_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{      
    
    //将颜色值转换为4个八位数据，DMA配置参数是uint32_t,但是实际搬运是uint16_t，但是使用uint16_t 2字节
    //数组会出问题，导致颜色显示不正常，暂时没想到解决办法
    uint8_t color1[4]={0};
    
    //搬运的数据数量
    uint16_t num=0;
    
    //转化填充颜色数据
    color1[0]=(uint8_t)(color>>8);
    color1[1]=(uint8_t)(color);
    
    num=(xend-xsta)*(yend-ysta);
    
    TFT_Address_Set(xsta,ysta,xend,yend);//设置显示范围
    
    TFT_CS_LOW();   //拉低片选信号
    
    //设置发送数据位宽为16bit
    SPI_DataSizeConfig(SPI1,SPI_DataSize_16b);
    
    //DMA配置
    MYDMA_Config((uint32_t)&SPI1->DR,(uint32_t)color1,num);
       
    //SPI DMA使能
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    
    //DMA使能
    MYDMA_Enable();
}

/*
*   函数内容：DMA显示区域图像函数
*   函数参数：
*               uint16_t xsta--起始X坐标
*               uint16_t ysta--起始Y坐标
*               uint16_t xend--结束X坐标
*               uint16_t yend--结束Y坐标
*               uint16_t num---数据长度
*               uint8_t* showData数据指针
*/
void DMA_ShowBMP(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t num,volatile uint8_t* showData)
{      
    
    TFT_Address_Set(xsta,ysta,xend,yend);//设置显示范围
    
    TFT_CS_LOW();   //拉低片选信号
    
    //设置发送数据位宽为16bit
    SPI_DataSizeConfig(SPI1,SPI_DataSize_16b);
    
    //DMA配置
    MYDMA_Config((uint32_t)&SPI1->DR,(uint32_t)showData,num);    
    
    //SPI DMA使能
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
        
    //DMA使能
    MYDMA_Enable();
}

/*
*   函数内容：初始化显示数据
*   函数参数：uint8_t *showData--数据指针
*   返回值：    无
*/
void Init_ShowData(volatile uint8_t *showData)
{
    uint16_t i=0,j=0;
    for(i=0;i<=40;i++)
    {
        for(j=0;j<=50;j++)
        {
            if(((i%10)==0)||(i==0))
            {
              *showData++=(uint8_t)(BLUE>>8);
              *showData++=(uint8_t)(BLUE);  
            }
            else
            {
              *showData++=(uint8_t)(BLACK>>8);
              *showData++=(uint8_t)(BLACK);  
            }
        }
    }
}

/*
*   函数内容：  画线
*   函数参数：  x1,y1   起始坐标
*               x2,y2   终止坐标
*               color   要填充的颜色
*   返回值：    无
*/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color,volatile uint8_t* showData)
{ 
    uint16_t i=0;
    uint16_t t=0; 
    int xerr=0,yerr=0,delta_x=0,delta_y=0,distance=0;
    int incx=0,incy=0,uRow=0,uCol=0;
    delta_x=x2-x1; //计算坐标增量 
    delta_y=y2-y1;
    uRow=x1;//画线起点坐标
    uCol=y1;
    if(delta_x>0){
        incx=1; //设置单步方向 
    }
    else if (delta_x==0){
        incx=0;//垂直线 
    }
    else {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0){
        incy=1;
    }
    else if (delta_y==0){
        incy=0;//水平线 
    }
    else {
        incy=-1;
        delta_y=-delta_y;
    }
    if(delta_x>delta_y){
        distance=delta_x; //选取基本增量坐标轴 
    }
    else {
        distance=delta_y;
    }
    for(t=0;t<distance+1;t++)
    {
        i=uRow*2+(uCol-40)*51*2;
        
        if((i%2)==0)
        {
            *(showData+i)=(uint8_t)(color>>8);
            *(showData+i+1)=(uint8_t)(color);
        }
        else
        {
            *(showData+i-1)=(uint8_t)(color>>8);
            *(showData+i)=(uint8_t)(color); 
        }
        i=0;
        
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
/*
*   函数内容：画折线
*   函数参数：short int rawValue--Y轴参数值
*   返回值：  无
*/
void DMAdrawCurve(uint8_t yOffset,short int rawValue,volatile uint8_t *showData)  
{
    uint16_t i=0;
    uint16_t x=0,y=0;
    y = yOffset - rawValue;  	//data processing code
    if(firstPoint)//如果是第一次画点，则无需连线，直接描点即可
    {
        i=0+(y-40)*51*2;
        if((i%2)==0)
        {
            *(showData+i)=(uint8_t)(RED>>8);
            *(showData+i+1)=(uint8_t)(RED);
        }
        else
        {
            *(showData+i-1)=(uint8_t)(RED>>8);
            *(showData+i)=(uint8_t)(RED);
        } 
        i=0;
        
        lastX=0;
        lastY=y;
        firstPoint=0;   
    }
    else
    {
        x=lastX+1;
        
        if(x<50)  //不超过屏幕宽度
        {
            LCD_DrawLine(lastX,lastY,x,y,RED,showData);            
            lastX=x;
            lastY=y;
        }
        else  //超出屏幕宽度，清屏，从第一个点开始绘制，实现动态更新效果
        {    
            LCD_DrawLine(lastX,lastY,x,y,RED,showData);
            lastX=0;
            lastY=y;
        }
  }
}

extern volatile struct Oscilloscope oscilloscope;

void DMA1_Channel3_IRQHandler(void)
{
  //SPI句柄
  SPI_InitTypeDef SPI_InitStructure;
   //发送完成 
  if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET){
        
    TFT_CS_LOW();   //拉低片选信号

    //SPI DMA失能
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);

    //失能DMA通道2
    DMA_Cmd(DMA1_Channel3,DISABLE);
        
    //SPI0复位
    //SPI_Cmd(SPI1, DISABLE);	 //必须先禁用,才能改变MODE

//    //SPI参数初始化
//    I2S_StructInit();

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
//        
//  oscilloscope.dma_flag=0;
//        
//  dma_interrupt_flag_clear(DMA_CH2, DMA_INT_FLAG_G);
    }
}