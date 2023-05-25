#include "Openmv.h"
#include "usart2.h"
#include "control.h"
#include "delay.h"
#include "beep.h"
#include "led.h"

extern int count;
int xx1=0,xx2=0,car_flag=0,line_num;
extern int Target_Velocity,Moto1,Moto2;
int state1=0;
int start_time=0;
int cnt=0,ok=0;
extern int mode;  
extern int quan,stflag;
int start_in = 0;
extern float speed1;
extern float speed2;
extern int start_come;
int Start_turn = 0;
void USART2_IRQHandler(void)
{
	unsigned char temp;
	static unsigned char RXBUF[8];
	static unsigned char RXCUNT=0;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
	temp=USART_ReceiveData(USART2); 
	if(temp=='W')
	{
    RXCUNT=0;	
	}
  else 
	{
		RXBUF[RXCUNT]=temp;
		RXCUNT++;
	}
  if(temp=='Z')
  {
     xx1=(RXBUF[0]-'0')*100+(RXBUF[1]-'0')*10+(RXBUF[2]-'0');
     xx2=(RXBUF[3]-'0')*100+(RXBUF[4]-'0')*10+(RXBUF[5]-'0');
		 car_flag=(RXBUF[6]-'0');
	   ok = (RXBUF[7]-'0');
	}
}
}
void read_state(void)
{
	switch(car_flag)
	{
		case 0: //正常巡线
						switch(mode)
						{
							case 1:line_num = -(80-xx1)*1;state1=0;cnt=1;break;
							case 2:line_num = -(80-xx1)/4;state1=0;cnt=1;break;
							case 3:line_num = -(80-xx1)/6;state1=0;cnt=1;break;
							case 4:line_num = -(80-xx1)*15;state1=0;cnt=1;break;
						}
						break;

		
		case 1: //停车线标志
			      if(cnt)
						{start_time++;count = 0;}cnt=0;
			
						if(stflag == 1||start_time==4)
						{stflag = 1;speed1=0;speed2=0;Set_Pwm(0,0);}
						break;
						
		case 2: 
						//岔路口——in
						switch(mode)
						{
							case 1:cnt=1;
										if(start_time==0||start_time==2)
										{delay_ms(500);line_num=0;delay_ms(300);break;}   //外圈
										
										else if(start_time==1||start_time==3)
										{delay_ms(400);line_num=2;delay_ms(280);break;}    //内圈   break;
							case 2:cnt=1;
										if(start_time==0||start_time==1||start_time==2)
										{line_num=0;delay_ms(20);break;}   //外圈
										
							case 3:cnt=1;
										if(start_time==0||start_time==1||start_time==3)
										{line_num=0;delay_ms(20);break;}   //外圈
										else if(start_time==2)
										{line_num=70;LED = 0;Start_turn=1;break;}    //内圈   break;
							case 4:cnt=1;
										if(start_time==0||start_time==2)
										{delay_ms(750);line_num=0;delay_ms(300);break;}   //外圈
										
										else if(start_time==1||start_time==3)
										{delay_ms(400);line_num=2;delay_ms(180);break;}    //内圈   break;
									}
						break;
		case 3: //岔路口——out
			      cnt=1; 
						switch(mode)
						{
							case 1:line_num = (80-xx1)/6;break;
						  case 2:line_num = (80-xx1)/6;break;
							case 3:
							if(start_time==2)
							{								

								line_num=170;
								Start_turn=0;break;
							}
							else
							{
								line_num = (80-xx1)/6;break;
							}
						}break;

		case 4: break;
		
		case 5: //弯道

						switch(mode)
						{
							case 1:
							cnt=1;
							if(state1>=1)
							{for(int x=0;x<2;x++){delay_ms(50);}}
							state1++;line_num = -(80-xx1)*1;break;
							case 2:	
							cnt=1;
							if(state1>=1)
							{for(int x=0;x<2;x++){delay_ms(50);}}
							state1++;line_num = -(80-xx1)*1;break;
							case 3:
							cnt=1;
							if(state1>=1)
							{for(int x=0;x<2;x++){delay_ms(50);}}
							state1++;line_num = -(80-xx1)*1;break;
							case 4:			    
							cnt=1;
							if(state1>=1)
							{for(int x=0;x<2;x++){delay_ms(50);}}
							state1++;line_num = -(80-xx1)*15;break;
							}
		default : break;
	}
}
