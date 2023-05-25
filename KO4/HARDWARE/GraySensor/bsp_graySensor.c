#include "bsp_GraySensor.h"
#include "sys.h"
#include "control.h"

int Line_Num;
int sensor[5]={0,0,0,0,0};

int stop_car = 0;


int start_time= 1;

void hongwai_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//ʹ��GPIOA��ʱ��
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//IO�ܽ�ģʽ����Ϊ�������룬�������ܻ�ȡ�������������������ź�(�ߵ͵�ƽ)
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_0;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);//PA4,PA5,PA6,PA7�ܽŵĳ�ʼ��
      GPIO_Init(GPIOB,&GPIO_InitStructure);//PA4,PA5,PA6,PA7�ܽŵĳ�ʼ��
}


/*******************�Ҷȴ�����Ѳ��*************/
//ֻҪ���ٶȻ����ƺã�����ֵ����ȷ����  ��ͬ�ٶ�ֵ��Ҫ��ͬ�Ĳ���ֵ��  ���Ժ�һ����õġ�  ��ͬ��ת��Ҫ��Ӧ��ͬ�Ĳ���ϵ������ֵ
void Light_GoStraight_control(void)   //�Ҷ�Ѳ��ֱ��, ��Ҫ�и��ж���Ҫֱ�ж೤����ĺ���    //����Ҫ���м�Ĵ������⵽�ص�����Ϊֹ
{
	  Get_Light_TTL();  

		if(sensor[1] == 0 && sensor[2] == 1 && sensor[3] == 0 ) 
		{			Line_Num =  0;start_time=1;}
	  if(sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 0 ) 
		{Line_Num =  2;start_time=1;}
	  if(sensor[1] == 1 && sensor[2] == 0 && sensor[3] == 0 ) 
		{Line_Num =  5;start_time=1;}
	  if(sensor[1] == 0 && sensor[2] == 0 && sensor[3] == 1 ) 
		{Line_Num = -5;start_time=1;}
		if(sensor[1] == 0 && sensor[2] == 1 && sensor[3] == 1 )  
		{Line_Num = -2;start_time=1;}
		if(sensor[1] == 1 && sensor[2] == 1 && sensor[3] == 1 )  
		{		
			if(start_time)
			{
					stop_car++;
					start_time=0;
			}
		}
	
}


//�ѻҶȴ���������ֻ����ߵ͵�ƽ�� �ߵ�ƽ��ʶ�𵽺����ˡ�
void Get_Light_TTL(void)  
{
	sensor[0] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
	sensor[1] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
	sensor[2] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	sensor[3] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
	sensor[4] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
}


