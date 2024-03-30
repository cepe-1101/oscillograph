#include "dma.h"
#include "tft.h"
#include "tft_init.h"

uint16_t DMA1_MEM_LEN=0;
static uint16_t lastX=0,lastY=0;
static uint8_t firstPoint = 1;

/*
*   �������ݣ�DMAͨ������
*   ����������
*           uint32_t cpar--DMAҪ���˵��������ַ
*           uint32_t cmar--DMAҪ���˵��ڴ�ĵ�ַ
*           uint16_t cndtr-Ҫ���˵��ڴ�Ĵ�С
*   ����ֵ����
*/
void MYDMA_Config(uint32_t cpar,uint32_t cmar,uint16_t cndtr)
{
  //DMA�����ṹ��
  DMA_InitTypeDef DMA_InitStructure;
    
  // ����DMAʱ��
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  
  // ����DMAԴ��ַ��TFT�Դ�Ĵ�����ַ*/
  DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;
  
  // �ڴ��ַ(Ҫ����ı�����ָ��)
  DMA_InitStructure.DMA_MemoryBaseAddr = cmar;
  // /���ݴ����� ,������Ϊ0�� ʵ�ʷ���ʱ����������	
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST; 
  // �����С	
  DMA_InitStructure.DMA_BufferSize = cndtr;
  // �����ַ����	    
		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		// �ڴ��ַ����
		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		
    // �������ݵ�λ	
		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
		// �ڴ����ݵ�λ
		DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;	 
		// DMAģʽ��һ�λ���ѭ��ģʽ
		DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
		//DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
		// ���ȼ�����	
		DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
		// ��ֹ�ڴ浽�ڴ�Ĵ���
		DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		// ����DMAͨ��		   
		DMA_Init(DMA1_Channel3, &DMA_InitStructure);	
 /* ������������ж�  */		
    DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);    
   DMA_Cmd(DMA1_Channel3,ENABLE);
}

/*
*   �������ݣ�ʹ��dma1��ͨ��3����ʼ��������
*   ������������
*   ����ֵ��  ��
*/
void DMA_SetDateCounter(void)
{ 
  //ʧ��DMAͨ��2
  DMA_Cmd(DMA1_Channel3,DISABLE);
  //while (DMA_GetCmdStatus(DMA1) != DISABLE){}  //ȷ��DMA���Ա�����
  //�޸�DMA������������
  DMA_SetCurrDataCounter(DMA1_Channel3,);
  //ʹ��DMAͨ��2����ʼ����
  DMA_Cmd(DMA1_Channel3,DISABLE);    
}

/*
*   �������ݣ�DMA��亯��
*   ����������
*               uint16_t xsta--��ʼX����
*               uint16_t ysta--��ʼY����
*               uint16_t xend--����X����
*               uint16_t yend--����Y����
*               uint16_t color-�����ɫֵ
*/
void DMA_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{      
    
    //����ɫֵת��Ϊ4����λ���ݣ�DMA���ò�����uint32_t,����ʵ�ʰ�����uint16_t������ʹ��uint16_t 2�ֽ�
    //���������⣬������ɫ��ʾ����������ʱû�뵽����취
    uint8_t color1[4]={0};
    
    //���˵���������
    uint16_t num=0;
    
    //ת�������ɫ����
    color1[0]=(uint8_t)(color>>8);
    color1[1]=(uint8_t)(color);
    
    num=(xend-xsta)*(yend-ysta);
    
    TFT_Address_Set(xsta,ysta,xend,yend);//������ʾ��Χ
    
    TFT_CS_LOW();   //����Ƭѡ�ź�
    
    //���÷�������λ��Ϊ16bit
    SPI_DataSizeConfig(SPI1,SPI_DataSize_16b);
    
    //DMA����
    MYDMA_Config((uint32_t)&SPI1->DR,(uint32_t)color1,num);
       
    //SPI DMAʹ��
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    
    //DMAʹ��
    MYDMA_Enable();
}

/*
*   �������ݣ�DMA��ʾ����ͼ����
*   ����������
*               uint16_t xsta--��ʼX����
*               uint16_t ysta--��ʼY����
*               uint16_t xend--����X����
*               uint16_t yend--����Y����
*               uint16_t num---���ݳ���
*               uint8_t* showData����ָ��
*/
void DMA_ShowBMP(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t num,volatile uint8_t* showData)
{      
    
    TFT_Address_Set(xsta,ysta,xend,yend);//������ʾ��Χ
    
    TFT_CS_LOW();   //����Ƭѡ�ź�
    
    //���÷�������λ��Ϊ16bit
    SPI_DataSizeConfig(SPI1,SPI_DataSize_16b);
    
    //DMA����
    MYDMA_Config((uint32_t)&SPI1->DR,(uint32_t)showData,num);    
    
    //SPI DMAʹ��
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
        
    //DMAʹ��
    MYDMA_Enable();
}

/*
*   �������ݣ���ʼ����ʾ����
*   ����������uint8_t *showData--����ָ��
*   ����ֵ��    ��
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
*   �������ݣ�  ����
*   ����������  x1,y1   ��ʼ����
*               x2,y2   ��ֹ����
*               color   Ҫ������ɫ
*   ����ֵ��    ��
*/
void LCD_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color,volatile uint8_t* showData)
{ 
    uint16_t i=0;
    uint16_t t=0; 
    int xerr=0,yerr=0,delta_x=0,delta_y=0,distance=0;
    int incx=0,incy=0,uRow=0,uCol=0;
    delta_x=x2-x1; //������������ 
    delta_y=y2-y1;
    uRow=x1;//�����������
    uCol=y1;
    if(delta_x>0){
        incx=1; //���õ������� 
    }
    else if (delta_x==0){
        incx=0;//��ֱ�� 
    }
    else {
        incx=-1;
        delta_x=-delta_x;
    }
    if(delta_y>0){
        incy=1;
    }
    else if (delta_y==0){
        incy=0;//ˮƽ�� 
    }
    else {
        incy=-1;
        delta_y=-delta_y;
    }
    if(delta_x>delta_y){
        distance=delta_x; //ѡȡ�������������� 
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
*   �������ݣ�������
*   ����������short int rawValue--Y�����ֵ
*   ����ֵ��  ��
*/
void DMAdrawCurve(uint8_t yOffset,short int rawValue,volatile uint8_t *showData)  
{
    uint16_t i=0;
    uint16_t x=0,y=0;
    y = yOffset - rawValue;  	//data processing code
    if(firstPoint)//����ǵ�һ�λ��㣬���������ߣ�ֱ����㼴��
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
        
        if(x<50)  //��������Ļ���
        {
            LCD_DrawLine(lastX,lastY,x,y,RED,showData);            
            lastX=x;
            lastY=y;
        }
        else  //������Ļ��ȣ��������ӵ�һ���㿪ʼ���ƣ�ʵ�ֶ�̬����Ч��
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
  //SPI���
  SPI_InitTypeDef SPI_InitStructure;
   //������� 
  if(DMA_GetFlagStatus(DMA1_FLAG_TC4)==SET){
        
    TFT_CS_LOW();   //����Ƭѡ�ź�

    //SPI DMAʧ��
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);

    //ʧ��DMAͨ��2
    DMA_Cmd(DMA1_Channel3,DISABLE);
        
    //SPI0��λ
    //SPI_Cmd(SPI1, DISABLE);	 //�����Ƚ���,���ܸı�MODE

//    //SPI������ʼ��
//    I2S_StructInit();

   /* SPI ģʽ���� */
  SPI_Cmd(TFT_SPIx, DISABLE);	 //�����Ƚ���,���ܸı�MODE
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//���߷���
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;//8bit����λ��
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//ʱ�Ӽ��Ը�
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//ʱ�Ӵ���ģʽ�ڶ�����
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//�������Ƭѡ
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//��Ƶ
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//��λ��ǰ
  SPI_InitStructure.SPI_CRCPolynomial = 7;//����У��
  SPI_Init(TFT_SPIx , &SPI_InitStructure);

  /* ʹ�� SPI  */
  SPI_Cmd(TFT_SPIx , ENABLE);
//        
//  oscilloscope.dma_flag=0;
//        
//  dma_interrupt_flag_clear(DMA_CH2, DMA_INT_FLAG_G);
    }
}