#include "sys.h"




extern long g_lMotorPulseSigma;
extern long g_lMotor2PulseSigma;
extern int g_nMotorPulse,g_nMotor2Pulse;//ȫ�ֱ����� ������������ֵ

extern float speedzuo;
extern float speedyou;

int start = 0;
int start_over = 0;

extern u8 Spin_start_flag , Spin_succeed_flag , Stop_Flag;

extern int mpu6050_get_data;

int show_oled=0;

extern float Angle_Balance,Gyro_Balance,Gyro_Turn;     						 //ƽ����� ƽ�������� ת��������
/*
����һ���⼸�������ĺ���:
Spin_start_flag , Spin_succeed_flag , Stop_Flag;

���ǵ�Ĭ��ֵ��Ϊ0���������Ϊ�����һ������Ľ�����

����������ִ��ֱ����������Ҫ���������������ж�

֮����ִ��ת������������Ҫ�ж�ֱ������Stop_flag�Ƿ�Ϊ1��
��ִ��ת������

��֮��ִ��ֱ������ʱ�����ж�Spin_succeed_flag�Ƿ�Ϊ1��
��ִ��ֱ������


*/


int main(void)
{
    
	delay_init();
    pwm_Init(7199,0);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    TIM3_Int_Init(7200-1,200-1);	  //�жϷ������ĳ�ʼ����Ҳ����PWM���ĳ�ʼ�������������Ϊ10ms��Ҳ����ÿ10ms��ִ��һ��control.c�ļ���TIM3_IRQHandler������Ĳ���
	uart_init(115200);
	Encoder_Init_TIM2();
	Encoder_Init_TIM4();
	motor_init();
	OLED_Init();
	OLED_Display_On();
	OLED_Clear();
    LED_GPIO_Config();
	//EXTIX_Init();
	//KEY_Init();
    hongwai_Init();
	/*Э�����ݽ�����ʼ��*/
	protocol_init();
 /* PID ������ʼ�� */
	PID_param_init();
    
    /*mpu6050�����ǳ�ʼ��*/
	IIC_Init();                     //IIC��ʼ��    
	MPU6050_initialize();           //MPU6050��ʼ��	
	DMP_Init();                     //��ʼ��DMP 
	MiniBalance_EXTI_Init();        //MPU6050 5ms��ʱ�жϳ�ʼ������ʡ��ʱ����Դ������cpu����
	while(1)
	{
	   show_oled++;
		/* �������ݴ��� */
	   receiving_process();
        
       printf("%.2f\n",Gyro_Turn);
       delay_ms(20);



	}
}


//		printf("%.2f,%.2f,%.2f\n",speedzuo,speedyou,pid_speed.target_val);
//		if(show_oled>=500)
//		{


//		}



//        if(mpu6050_get_data==1)
//        {
//            mpu6050_get_data = 0;
//        }



//              show_oled=0;
//            OLED_ShowString(0,1,(uint8_t *)("zuo:"),5);
//            //OLED_Float(1,40,pid_location.actual_val,5);
//            OLED_ShowNumber(40,1,g_lMotorPulseSigma,5,10);
//            OLED_ShowNumber(100,1,pid_location.target_val,5,10);
//    
//            OLED_ShowString(0,2,(uint8_t *)("you:"),5);
//            //OLED_Float(2,40,pid_location.actual_val,5);
//            OLED_ShowNumber(40,2,g_lMotor2PulseSigma,5,10);
//            OLED_ShowNumber(100,2,pid_location2.target_val,5,10);
//                OLED_ShowNumber(0,5,sensor[0],1,10);
//				OLED_ShowNumber(8,5,sensor[1],1,10);
//				OLED_ShowNumber(16,5,sensor[2],1,10);
//				OLED_ShowNumber(24,5,sensor[3],1,10);
//				OLED_ShowNumber(32,5,sensor[4],1,10);





//				switch(start)
//			{
//                
//                case 0:
//                    start++;
//                    Car_go(60);
//				break;
//				
//				case 1:									
//					if(Stop_Flag ==1)          //ֱ�С�ת�亯������ʹ��ʱ�����Բ��ֶ�����־λ����
//				 {
//					 start++;
//					 spin_Turn(right_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
//				 }
//				 break;
//				 
//				case 2:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(55);      //Stop_Flag�ٴ���1   +3��Ϊ�˽�ͣ����  
//				 }
//				 break;
//                    
//				case 3:
//                   if(Stop_Flag ==1)          //ֱ�С�ת�亯������ʹ��ʱ�����Բ��ֶ�����־λ����
//				 {
//                    start++;Car_go(28);
//                 }
//				break;
//				
//				case 4:									
//					if(Stop_Flag ==1)          //ֱ�С�ת�亯������ʹ��ʱ�����Բ��ֶ�����־λ����
//				 {
//					 start++;
//					 spin_Turn(right_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
//				 }
//				 break;
//				 
//				case 5:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(103);      //Stop_Flag�ٴ���1   +3��Ϊ�˽�ͣ����  
//				 }
//				 break;
//                 
//                case 6:							 
//					if(Stop_Flag ==1)          //ֱ�С�ת�亯������ʹ��ʱ�����Բ��ֶ�����־λ����
//				 {
//					 start++;
//					 spin_Turn(right_10);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
//				 }
//				 break;
//                 
//                case 7:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(150);      //Stop_Flag�ٴ���1   +3��Ϊ�˽�ͣ����  
//				 }
//				 break;
//				 
//				case 8:							 
//					if(Stop_Flag ==1)          //ֱ�С�ת�亯������ʹ��ʱ�����Բ��ֶ�����־λ����
//				 {
//					 start++;
//					 spin_Turn(right_90);   //��ʱSpin_succeed_flag== 0 ,��ɺ��Զ���1
//				 }
//				 break;
//				 
//				 case 9:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(20);      //Stop_Flag�ٴ���1   +3��Ϊ�˽�ͣ����  
//				 }
//				 break;
//				 
//				case 10:
//				if(Stop_Flag ==1)
//				{
//					//Set_Pwm(0,0);
//				}
//				break;
//            }

