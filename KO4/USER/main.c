#include "sys.h"




extern long g_lMotorPulseSigma;
extern long g_lMotor2PulseSigma;
extern int g_nMotorPulse,g_nMotor2Pulse;//全局变量， 保存电机脉冲数值

extern float speedzuo;
extern float speedyou;

int start = 0;
int start_over = 0;

extern u8 Spin_start_flag , Spin_succeed_flag , Stop_Flag;

extern int mpu6050_get_data;

int show_oled=0;

extern float Angle_Balance,Gyro_Balance,Gyro_Turn;     						 //平衡倾角 平衡陀螺仪 转向陀螺仪
/*
解释一下这几个变量的含义:
Spin_start_flag , Spin_succeed_flag , Stop_Flag;

他们的默认值都为0，如果变量为零代表一个任务的结束。

假如我们先执行直走任务：则不需要进行其他变量的判断

之后再执行转弯任务：则先需要判断直走任务Stop_flag是否为1；
再执行转弯任务。

再之后执行直走任务时，先判断Spin_succeed_flag是否为1，
再执行直走任务。


*/


int main(void)
{
    
	delay_init();
    pwm_Init(7199,0);	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    TIM3_Int_Init(7200-1,200-1);	  //中断服务函数的初始化，也就是PWM波的初始化，这里的周期为10ms，也就是每10ms会执行一次control.c文件里TIM3_IRQHandler函数里的操作
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
	/*协议数据解析初始化*/
	protocol_init();
 /* PID 参数初始化 */
	PID_param_init();
    
    /*mpu6050陀螺仪初始化*/
	IIC_Init();                     //IIC初始化    
	MPU6050_initialize();           //MPU6050初始化	
	DMP_Init();                     //初始化DMP 
	MiniBalance_EXTI_Init();        //MPU6050 5ms定时中断初始化，节省定时器资源，减少cpu负担
	while(1)
	{
	   show_oled++;
		/* 接收数据处理 */
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
//					if(Stop_Flag ==1)          //直行、转弯函数交替使用时，可以不手动将标志位清零
//				 {
//					 start++;
//					 spin_Turn(right_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
//				 }
//				 break;
//				 
//				case 2:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(55);      //Stop_Flag再次置1   +3是为了进停车区  
//				 }
//				 break;
//                    
//				case 3:
//                   if(Stop_Flag ==1)          //直行、转弯函数交替使用时，可以不手动将标志位清零
//				 {
//                    start++;Car_go(28);
//                 }
//				break;
//				
//				case 4:									
//					if(Stop_Flag ==1)          //直行、转弯函数交替使用时，可以不手动将标志位清零
//				 {
//					 start++;
//					 spin_Turn(right_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
//				 }
//				 break;
//				 
//				case 5:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(103);      //Stop_Flag再次置1   +3是为了进停车区  
//				 }
//				 break;
//                 
//                case 6:							 
//					if(Stop_Flag ==1)          //直行、转弯函数交替使用时，可以不手动将标志位清零
//				 {
//					 start++;
//					 spin_Turn(right_10);   //此时Spin_succeed_flag== 0 ,完成后自动置1
//				 }
//				 break;
//                 
//                case 7:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(150);      //Stop_Flag再次置1   +3是为了进停车区  
//				 }
//				 break;
//				 
//				case 8:							 
//					if(Stop_Flag ==1)          //直行、转弯函数交替使用时，可以不手动将标志位清零
//				 {
//					 start++;
//					 spin_Turn(right_90);   //此时Spin_succeed_flag== 0 ,完成后自动置1
//				 }
//				 break;
//				 
//				 case 9:							 
//				if(Spin_succeed_flag == 1)
//				 {
//					 start++;
//					 Car_go(20);      //Stop_Flag再次置1   +3是为了进停车区  
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

