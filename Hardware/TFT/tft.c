#include "tft.h"
#include "tft_init.h"
#include "string.h"
#include "font.h"

/*
*   �������ݣ�  ��ָ�����������ɫ
*   ����������  xsta,ysta---��ʼ����
*               xend��yend---��ֹ����
*               color--------Ҫ������ɫ
*   ����ֵ��    ��
*/
void TFT_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color)
{
    uint16_t i=0,j=0;
    TFT_Address_Set(xsta,ysta,xend-1,yend-1);   //������ʾ��Χ
    for(i=ysta;i<yend;i++)
    {
        for(j=xsta;j<xend;j++)
        {
            TFT_WR_DATA(color);
        }
    }
}

/*
*   �������ݣ�  ��ָ��λ�û���
*   ����������  x,y---��������
*               color--------Ҫ������ɫ
*   ����ֵ��    ��
*/
void TFT_DrawPoint(uint16_t x,uint16_t y, uint16_t color)
{
    TFT_Address_Set(x,y,x,y);   //������ʾ��Χ
    TFT_WR_DATA(color);
}

/*
*   �������ݣ�  ����
*   ����������  x1,y1   ��ʼ����
*               x2,y2   ��ֹ����
*               color   Ҫ������ɫ
*   ����ֵ��    ��
*/
void TFT_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
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
		TFT_DrawPoint(uRow,uCol,color);//���� 

        for(i=30;i<80;i++)
        {
            TFT_DrawPoint(uRow+1,i,BLACK);//����
        }
        
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
*   �������ݣ�  ������
*   ����������  x1,y1   ��ʼ����
*               x2,y2   ��ֹ����
*               color   ���ε���ɫ
*   ����ֵ��    ��
*/
void TFT_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	TFT_DrawLine(x1,y1,x2,y1,color);
	TFT_DrawLine(x1,y1,x1,y2,color);
	TFT_DrawLine(x1,y2,x2,y2,color);
	TFT_DrawLine(x2,y1,x2,y2,color);
}

/*
*   �������ݣ�  ��Բ
*   ����������  x0,y0   Բ������
*               r       �뾶
*               color   ���ε���ɫ
*   ����ֵ��    ��
*/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	uint16_t a=0,b=0;
	a=0;b=r;	  
	while(a<=b)
	{
		TFT_DrawPoint(x0-b,y0-a,color);             //3           
		TFT_DrawPoint(x0+b,y0-a,color);             //0           
		TFT_DrawPoint(x0-a,y0+b,color);             //1                
		TFT_DrawPoint(x0-a,y0-b,color);             //2             
		TFT_DrawPoint(x0+b,y0+a,color);             //4               
		TFT_DrawPoint(x0+a,y0-b,color);             //5
		TFT_DrawPoint(x0+a,y0+b,color);             //6 
		TFT_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

static uint16_t lastX=0,lastY=0;
static uint8_t firstPoint = 1;

/*
*   �������ݣ�������
*   ����������short int rawValue--Y�����ֵ
*   ����ֵ��  ��
*/
void drawCurve(uint8_t yOffset,short int rawValue)  
{
	uint16_t x=0,y=0;
	y = yOffset - rawValue;  	//data processing code
	if(firstPoint)//����ǵ�һ�λ��㣬���������ߣ�ֱ����㼴��
	{
		TFT_DrawPoint(0,y,GREEN);   
		lastX=0;
		lastY=y;
		firstPoint=0; 
	}
	else
	{
		x=lastX+1;
        
		if(x<100)  //��������Ļ���
		{
			TFT_DrawLine(lastX,lastY,x,y,GREEN);            
			lastX=x;
			lastY=y;
		}
		else  //������Ļ��ȣ��������ӵ�һ���㿪ʼ���ƣ�ʵ�ֶ�̬����Ч��
		{         
			//TFT_Fill(0,0,160,128,WHITE);//��������ɫ����
			TFT_DrawPoint(0,y,GREEN);   
			lastX=0;
			lastY=y;
		}
  }
}

/*
*   �������ݣ���ʾ�ַ�
*   ����������  x,y---��ʼ����
*               num Ҫ��ʾ���ַ�
*               fc �ֵ���ɫ
*               bc �ֵı���ɫ
*               sizey �ֺ�
*               mode:  0�ǵ���ģʽ  1����ģʽ
*   ����ֵ��    ��
*/
void TFT_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t temp,sizex,t,m=0;
	uint16_t i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	TFT_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12){
            temp=ascii_1206[num][i];		       //����6x12����
        }
		else if(sizey==16){
            temp=ascii_1608[num][i];		 //����8x16����
        }
		else if(sizey==24){
            temp=ascii_2412[num][i];		 //����12x24����
        }
		else {
            return;
        }
		for(t=0;t<8;t++)
		{
			if(!mode)//�ǵ���ģʽ
			{
				if(temp&(0x01<<t)){
                    TFT_WR_DATA(fc);
                }
				else {
                    TFT_WR_DATA(bc);
                }
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//����ģʽ
			{
				if(temp&(0x01<<t)){
                    TFT_DrawPoint(x,y,fc);//��һ����
                }
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}

/*
*   �������ݣ�  ��ʾ�ַ���
*   ����������  x,y---��ʼ����
*               *p Ҫ��ʾ���ַ���
*               fc �ֵ���ɫ
*               bc �ֵı���ɫ
*               sizey �ֺ�
*               mode:  0�ǵ���ģʽ  1����ģʽ
*   ����ֵ��    ��
*/
void TFT_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{         
	while(*p!='\0')
	{       
		TFT_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}

/******************************************************************************
      ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void TFT_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	while(*s!=0)
	{
		if(sizey==12) {
            TFT_ShowChinese12x12(x,y,s,fc,bc,sizey,mode);
        }
		else if(sizey==16) {
            TFT_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
        }
		else if(sizey==24) {
            TFT_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
        }
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      ����˵������ʾ����12x12����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void TFT_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i=0,j=0,m=0;
	uint16_t k=0;
	uint16_t HZnum=0;//������Ŀ
	uint16_t TypefaceNum=0;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	                         
	HZnum=sizeof(tfont12)/sizeof(typFNT_GB12);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
		{ 	
			TFT_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont12[k].Msk[i]&(0x01<<j)){
                            TFT_WR_DATA(fc);
                        }
						else {
                            TFT_WR_DATA(bc);
                        }
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont12[k].Msk[i]&(0x01<<j))	{
                            TFT_DrawPoint(x,y,fc);//��һ����
                        }
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����16x16����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void TFT_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i=0,j=0,m=0;
	uint16_t k=0;
	uint16_t HZnum=0;//������Ŀ
	uint16_t TypefaceNum=0;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			TFT_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j)){
                            TFT_WR_DATA(fc);
                        }
						else {
                            TFT_WR_DATA(bc);
                        }
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j)){
                            TFT_DrawPoint(x,y,fc);//��һ����
                        }
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����24x24����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void TFT_ShowChinese24x24(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey,uint8_t mode)
{
	uint8_t i=0,j=0,m=0;
	uint16_t k=0;
	uint16_t HZnum=0;//������Ŀ
	uint16_t TypefaceNum=0;//һ���ַ���ռ�ֽڴ�С
	uint16_t x0=x;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			TFT_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j)){
                            TFT_WR_DATA(fc);
                        }
						else{
                            TFT_WR_DATA(bc);
                        }
						m++;
						if(m%sizey==0)
						{
							m=0;
							break;
						}
					}
					else//���ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j)){
                            TFT_DrawPoint(x,y,fc);//��һ����
                        }
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/*
*   �������ݣ���ֹUI����
*   ������������
*   ����ֵ��  ��
*/
void TFT_StaticUI(void)
{
    uint16_t i=0;
    
    char showData[32]={0};
    
    TFT_ShowChinese(10,0,(uint8_t *)"����ʾ����",BLACK,GREEN,16,0);
    
    sprintf(showData,"  PWM ");
    TFT_ShowString(110,0,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,32);
    
    TFT_ShowChinese(110,20,(uint8_t *)"���״̬",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,36,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,32);
    
    TFT_ShowChinese(110,56,(uint8_t *)"���Ƶ��",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,32);
    
    sprintf(showData,"        ");
    TFT_ShowString(110,92,(uint8_t *)showData,WHITE,PURPLE,12,0);  
    memset(showData,0,32);
    TFT_ShowChinese(118,92,(uint8_t *)"ռ�ձ�",WHITE,PURPLE,12,0);
    
    sprintf(showData,"      ");
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,32);
    
    TFT_ShowChinese(5,92,(uint8_t *)"�����ֵ",WHITE,PURPLE,12,0);
    
    TFT_ShowChinese(55,92,(uint8_t *)"����Ƶ��",WHITE,PURPLE,12,0);
    
    for(i=0;i<=128;i=i+2)
    {
        TFT_DrawPoint(106,i,YELLOW);
    }
    
    for(i=0;i<100;i++)
    {
        TFT_DrawPoint(i,81,GREEN);
    }
    for(i=0;i<10;i++)
    {
        TFT_DrawPoint((i*10)+2,82,GREEN); 
        TFT_DrawPoint((i*10)+3,82,GREEN); 
    }
    for(i=0;i<10;i++)
    {
        TFT_DrawPoint((i*10)+2,83,GREEN); 
        TFT_DrawPoint((i*10)+3,83,GREEN);
    }
}
/*
*   �������ݣ�  ��ʾ�ַ���
*   ����������  uint16_t vpp--���ֵ
*               uint16_t freq-Ƶ��
*               float DoBias--ֱ��ƫִ�ź�
*   ����ֵ��    ��
*/
void TFT_ShowUI(volatile const struct Oscilloscope *value)
{
    char showData[64]={0};
    
    sprintf(showData,"%1.2fV ",(*value).vpp);
    TFT_ShowString(5,106,(uint8_t *)showData,BLACK,GREEN,16,0);  
    memset(showData,0,64);
    
    if((*value).gatherFreq>=1000)
    {
        sprintf(showData,"%2.0fKHz ",(*value).gatherFreq/1000.0f);
        TFT_ShowString(55,106,(uint8_t *)showData,BLACK,GREEN,16,0);  
        memset(showData,0,64);
    }
    else
    {
        sprintf(showData,"%3.0fHz ",(*value).gatherFreq);
        TFT_ShowString(55,106,(uint8_t *)showData,BLACK,GREEN,16,0);  
        memset(showData,0,64);  
    }

    if((*value).ouptputbit == 1)
    {   
        TFT_ShowChinese(118,36,(uint8_t *)"��",BLACK,YELLOW,16,0);
    }
    else
    {
        TFT_ShowChinese(118,36,(uint8_t *)"�ر�",BLACK,YELLOW,16,0);     
    }
    
    if((*value).outputFreq>=1000)
    {
        sprintf(showData,"%3dKHz",(*value).outputFreq/1000);
        TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
        memset(showData,0,64); 
    }
    else
    {
        sprintf(showData," %3dHz",(*value).outputFreq);
        TFT_ShowString(110,72,(uint8_t *)showData,BLACK,YELLOW,16,0);  
        memset(showData,0,64);
    }  
    
    sprintf(showData,"%3.1f%%  ",(((*value).pwmOut)/((*value).timerPeriod+0.0f))*100);
    TFT_ShowString(110,106,(uint8_t *)showData,BLACK,YELLOW,16,0);  
    memset(showData,0,64);        
}




