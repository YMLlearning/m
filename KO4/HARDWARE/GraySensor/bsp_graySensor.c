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
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOA的时钟
	
	  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//IO管脚模式配置为浮空输入，这样就能获取传感器传回来的数字信号(高低电平)
	  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_4|GPIO_Pin_0;
	  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	  GPIO_Init(GPIOA,&GPIO_InitStructure);//PA4,PA5,PA6,PA7管脚的初始化
      GPIO_Init(GPIOB,&GPIO_InitStructure);//PA4,PA5,PA6,PA7管脚的初始化
}


/*******************灰度传感器巡线*************/
//只要把速度环控制好，补偿值即可确定。  不同速度值需要不同的补偿值。  测试好一个最好的。  不同的转速要对应不同的补偿系数或数值
void Light_GoStraight_control(void)   //灰度巡线直行, 需要有个判断需要直行多长距离的函数    //补偿要到中间的传感器测到回到线上为止
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


//把灰度传感器当作只输出高低电平。 高电平是识别到红线了。
void Get_Light_TTL(void)  
{
	sensor[0] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4);
	sensor[1] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
	sensor[2] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6);
	sensor[3] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7);
	sensor[4] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0);
}


