#include "tft_init.h"
#include "delay.h"
/*
*   �������ݣ���ʼ��SPI0����
*   ������������
*   ����ֵ����
*/
static void Init_SPI0_GPIO(void)
{
  //SPI���
  SPI_InitTypeDef SPI_InitStructure;
  //GPIO���
	GPIO_InitTypeDef GPIO_InitStructure;
  /* ʹ��SPIʱ�� */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_SPI1, ENABLE );
  /* ʹ�ܶ˿�ʱ�� */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA, ENABLE );
  /* ʹ�ܶ˿�ʱ�� */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB, ENABLE );
  /* ʹ�ܸ���ʱ�� */
	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_AFIO, ENABLE );
  
  /************��������*************/
  
	GPIO_InitStructure.GPIO_Pin = TFT_SPI_CS_PIN;  //SPI_NSS-CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_CS_PORT, &GPIO_InitStructure);
	TFT_CS_HIGH();//����CS��ƽ
  
	GPIO_InitStructure.GPIO_Pin = TFT_SPI_SCK_PIN;		//SPI_SCK
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_SCK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  TFT_SPI_MISO_PIN;		//SPI_MISO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = TFT_SPI_MOSI_PIN;        //SPI_MOSI
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TFT_SPI_MOSI_PORT, &GPIO_InitStructure);


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

}

/*
*   �������ݣ�SPI0��������
*   ������������
*   ����ֵ����
*/
static void SPI0_Write(uint8_t TxData)
{
    //�ȴ����ͻ���������
    while(SPI_I2S_GetFlagStatus(TFT_SPIx,SPI_I2S_FLAG_TXE)==RESET);
    TFT_SPIx->DR=TxData; //ͨ������SPIx����һ��byte  ����
    TFT_SPIx->SR&=(1<<0);
//  //�����鷢�ֵ���ģʽ���ڷ�������ʱ����ʹ��SPI_I2S_FLAG_TXE���жϷ�����ɣ�
//  //ֻ��ʹ��SPI_I2S_FLAG_BSY��־���ж��豸�Ƿ���С�
//    while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)== SET){}
}

/*
*   �������ݣ���ʼ��REST��BLK����
*   ������������
*   ����ֵ����
*/
static void TFT_GPIO_Init(void)
{
  //GPIO��ʼ�����
	GPIO_InitTypeDef GPIO_InitStructure;  
  
  //����REST�ӿ�
  RCC_APB2PeriphClockCmd(REST_GPIO_CLK,ENABLE);
  GPIO_InitStructure.GPIO_Pin = REST_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(REST_GPIO_PORT, &GPIO_InitStructure);
  
  //����TFT_BLK
  RCC_APB2PeriphClockCmd(BLK_GPIO_CLK,ENABLE);
  GPIO_InitStructure.GPIO_Pin = BLK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(BLK_GPIO_PORT, &GPIO_InitStructure);
  
  //����TFT_DC
  RCC_APB2PeriphClockCmd(DC_GPIO_CLK,ENABLE);
  GPIO_InitStructure.GPIO_Pin = DC_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DC_GPIO_PORT, &GPIO_InitStructure);
}

/*
*   �������ݣ�TFT���͵����ֽ�����
*   ������������
*   ����ֵ����
*/
void TFT_WR_DATA8(uint8_t data)
{
    SPI_TFT_CS_LOW();   //����Ƭѡ�ź�
    
    SPI0_Write(data);
    
    SPI_TFT_CS_HIGH();     //����Ƭѡ�ź�
}

/*
*   �������ݣ�TFT����2���ֽ�����
*   ������������
*   ����ֵ����
*/
void TFT_WR_DATA(uint16_t data)
{
    SPI_TFT_CS_LOW();   //����Ƭѡ�ź�
    
    SPI0_Write(data>>8);
    
    SPI0_Write(data);
    
    SPI_TFT_CS_HIGH();     //����Ƭѡ�ź�
}

/*
*   �������ݣ�TFT������������
*   ������������
*   ����ֵ����
*/
void TFT_WR_REG(uint8_t reg)
{
    SPI_TFT_CS_LOW();   //����Ƭѡ�ź�
    TFT_DC_LOW();       //����Ƭѡ�ź�
    
    SPI0_Write(reg);
    
    TFT_DC_HIGH();     //���������ź�
    TFT_DC_HIGH();;     //����Ƭѡ�ź�
}


void TFT_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	if(USE_HORIZONTAL==0)
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==1)
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}
	else if(USE_HORIZONTAL==2)
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}
	else
	{
		TFT_WR_REG(0x2a);//�е�ַ����
		TFT_WR_DATA(x1);
		TFT_WR_DATA(x2);
		TFT_WR_REG(0x2b);//�е�ַ����
		TFT_WR_DATA(y1);
		TFT_WR_DATA(y2);
		TFT_WR_REG(0x2c);//������д
	}    
}

void TFT_Init(void)
{
    TFT_GPIO_Init();
    Init_SPI0_GPIO();
    
    TFT_REST_LOW();   //��λ
    delay_ms(100);
    TFT_REST_HIGH();     //
    delay_ms(100);
    
    TFT_BLK_HIGH();     //�򿪱���
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

