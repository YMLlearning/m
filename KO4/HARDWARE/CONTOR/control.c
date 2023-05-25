#include "control.h"



u8 Way_Angle=2;                             //获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波 
float Angle_Balance,Gyro_Balance,Gyro_Turn; //平衡倾角 平衡陀螺仪 转向陀螺仪
int Temperature;                            //温度变量
float Acceleration_Z;                       //Z轴加速度计

int Moto1,Moto2;//电机PWM变量 应是Motor的 向Moto致敬
float speedzuo = 0;
float speedyou = 0;
int i=0;
float Motor_journey_cm, Motor2_journey_cm;
float g_fTargetJourney = 0;  //存放小车左右轮所走路程和 ， 单位cm，需要在下一阶段任务中设置
float  MotorPWM =0.0, Motor2PWM =0.0;

long g_lMotorPulseSigma =0;
long g_lMotor2PulseSigma=0;
int g_nMotorPulse=0,g_nMotor2Pulse=0;//全局变量， 保存电机脉冲数值
unsigned char location_control_count = 0;  //执行频率不需要那么高的用这个事件计数，用在中断中
unsigned char xunji_control_count = 0;  //执行频率不需要那么高的用这个事件计数，用在中断中
unsigned char mpu6050_control_count = 0;  //执行频率不需要那么高的用这个事件计数，用在中断中
float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;


u8 Spin_start_flag , Spin_succeed_flag , Stop_Flag;
u8 stop_count , spin_count;
u8 Line_flag = 0, Turn_flag = 0,Xunji_flag = 0 ;

int line_out = 0;
int line_out1,line_out2;
float Xunji_Outval,Xunji_Outval2;


float Target_turn = 90;//90度kp = 6,180度kp = 4.5,每个角度对应的pid不一样，所以当需要转不同的角度时，所设定的pid也不一样
float actual_angle = 0;//经历过缩放之后，显示的就不是实际角度了
short gyrox,gyroy,gyroz;				//陀螺仪--角速度
float turn_pwm;
float turn_pwm1;
float turn_pwm2;
float mpu6050_Outval,mpu6050_Outval2;
int mpu6050_get_data = 0;


void EXTI15_10_IRQHandler(void) 
{    
   if(EXTI_GetITStatus(EXTI_Line12)!=0)//一级判定
	{
	if(INT==0)		
	{   
		EXTI_ClearITPendingBit(EXTI_Line12);//清除中断标志位   
		Get_Angle(Way_Angle);                     					//更新姿态，5ms一次，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
	 }
    }    
} 

void TIM3_IRQHandler(void)   //TIM3中断
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否  2ms中断一次   
		{ 
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);                        //===清除定时器1中断标志位
			
//		g_nMotorPulse = +Read_Encoder(2);
//			
//		g_nMotor2Pulse = -Read_Encoder(4);
//			
//		g_lMotorPulseSigma += g_nMotorPulse;//位置外环使用的脉冲累积      //完成一次位置控制之后才清除。
//		g_lMotor2PulseSigma += g_nMotor2Pulse;//位置外环使用的脉冲累积   //记录了之后也要及时清零呀！！！
//			
//			
//		Motor_journey_cm	  = ( g_lMotorPulseSigma /(30*13*4) ) * (6.5*3.14);
//		Motor2_journey_cm	 =  ( g_lMotor2PulseSigma /(30*13*4) ) * (6.5*3.14);	
//		
//       mpu6050_control();
//       MotorPWM = mpu6050_Outval; 
//       Motor2PWM = mpu6050_Outval2;    
//       Set_Pwm( MotorPWM, Motor2PWM); 
            
            
            
//        if(Xunji_flag==1)//循迹
//        {
//            Light_GoStraight_control();//只有正常循迹的时候才读取灰度
//            Xunji_speed_control();    
//                
//            MotorPWM = Xunji_Outval; 
//            Motor2PWM = Xunji_Outval2;    
//            Set_Pwm( MotorPWM, Motor2PWM); 
//        }            

//         if(Line_flag==1)//走直线固定距离
//         {
//             
//             if( (Motor_journey_cm <= (g_fTargetJourney + 25)) && (Motor_journey_cm >= (g_fTargetJourney - 25)) )  //这里偏差这么多真的好吗？并不是真的偏差
//            {
//				stop_count++;   //stop_count不能超过256
//				if(stop_count >=  100)  //100 * 20 = 1.6s  最少也要至少在目标位置停留1s  //可以时间判断放长点，以便刹车停稳
//				{
//                    Line_flag = 0;
//                    Stop_Flag = 1; //这个标志位可以用来判断是否执行下一阶段任务
//                    stop_count = 0;					
//                    Set_Pwm( 0, 0); 	
//				}
//            }
//             else
//             {
//                    Stop_Flag = 0;  
//                    stop_count = 0;  
//             } 
//             
//            Location_Speed_control(); //位置环速度环串级PID的输出是速度环输出的PWM值
//                
//                
//            MotorPWM = speed_Outval; 
//            Motor2PWM = speed2_Outval; 	
//            
//            Set_Pwm(MotorPWM,Motor2PWM);
//         }
//         
//         
//         	if( Spin_start_flag == 1)  //转弯
//			{
//				
//						Location_Speed_control(); //位置环速度环串级PID的输出是速度环输出的PWM值

//						
//						MotorPWM = speed_Outval;
//						Motor2PWM = speed2_Outval;
//						
//						spin_count++;
//						if( spin_count >= 100)      //20ms进入一次   100*20 = 2s，以能过够完成倒转时间为下限，应该已经倒转完毕了
//						{
//							 Spin_start_flag = 0;
//							 spin_count = 0;
//							
//							//转向有点问题，转完之后还一直转,暂时用下面两句解决
//							MotorPWM =0;   
//							Motor2PWM = 0;
//												 
//							Spin_succeed_flag = 1;					
//						}
//					
//						Set_Pwm( MotorPWM, Motor2PWM);			
//			}
         
	}       	  
} 


         /*调参速度环*/
//		MotorPWM = speed_pid_control(); 
//		Motor2PWM = speed2_pid_control(); 
          /*调参位置环--其实位置环直接看实际效果调就OK的，只需要调KP*/
		//MotorPWM = location_pid_control();  
		//Motor2PWM = location2_pid_control();


void Car_go(int32_t location_cm)   //直走函数  
{
	float Car_location;
	
	Line_flag = 1;   
	Stop_Flag = 0;
	
	Spin_start_flag = 0;
	Spin_succeed_flag = 0;  
	
	Motor_journey_cm = 0;
	Motor2_journey_cm = 0;
	
	g_fTargetJourney = location_cm;   //防止长时间PID控制用
	
	//如果巡线效果不好就将3.142加大
	Car_location   = ( location_cm / (6.5 * 3.14) ) *   (30*4*13) ;    //Car_location 将  location_cm 转换为对应的脉冲数   g_fTargetJourney = 要转多少圈 * 1圈的脉冲数 
	//小车位置直接用一个电机的脉冲数累积就好，转向时不计数，开始一个位置前直接清零。   
	 
	  set_pid_target(&pid_location, Car_location);   
	  set_pid_target(&pid_location2, Car_location);
		
	  g_lMotorPulseSigma = 0;    //之前没清除所以用不了
	  g_lMotor2PulseSigma = 0; 
	
		
}



void spin_Turn(spin_dir_t zhuanxiang)   //传入小车的轮距(mm) 175mm  //其实转向环并不需要很精准，转弯后直接用直走时的巡线函数回正车身就好
{
		float spin90_val;//用来保存小车转90度单个轮子所转动的距离
        float spin45_val;//用来保存小车转45度单个轮子所转动的距离
        float spin10_val;//用来保存小车转45度单个轮子所转动的距离
        float spin20_val;//用来保存小车转45度单个轮子所转动的距离
		float Car_Turn_val;
     
    	Line_flag = 0;  //不进行巡线的补偿了
        Stop_Flag = 0;   //执行转弯时，将直走完成的标志位清零. 即如果上一次是直行，这次是转弯，则不用在业务代码里手动置位
        Spin_start_flag = 1;   
        Spin_succeed_flag = 0; 
		
		Motor_journey_cm = 0;
		Motor2_journey_cm = 0;
		
		spin90_val = 0.25*3.1416*LUN_JU;
        spin45_val = 0.125*3.1416*LUN_JU;
        spin10_val = 0.028*3.1416*LUN_JU;
        spin20_val = 0.056*3.1416*LUN_JU;
		
	/****转弯后不循线可以调转向系数**********/
		if(zhuanxiang == left_90) //左转90
		{
			Car_Turn_val = ( spin90_val / (6.5 * 3.142) ) *   (30*4*13) ; 
			Car_Turn_val =  Car_Turn_val;   //90*0.94 = 84.6   //惯性影响，导致转弯比理论设定的多。直接设90度接下来的巡线就寻不回来了
			
		}
		else if(zhuanxiang == right_90)//右转90
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.142) ) *   (30*4*13); 
			Car_Turn_val =  Car_Turn_val;  //90*0.96 = 86.4    //惯性影响，导致转弯比理论设定的多。 接下来的巡线可能就寻不回来了
		}
		else if(zhuanxiang == back_180)//向后转180
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val = Car_Turn_val;   //同理
			Car_Turn_val = 2*Car_Turn_val;      //0.96*180 = 175.5
		}
        else if(zhuanxiang == left_45)//左转45度
		{
			Car_Turn_val = (spin45_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //同理
		}
        else if(zhuanxiang == right_45)//右转45度
		{
			Car_Turn_val = -(spin45_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //同理
		}
         else if(zhuanxiang == right_10)//右转10度
		{
			Car_Turn_val = -(spin10_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //同理
		}
          else if(zhuanxiang == right_20)//右转20度
		{
			Car_Turn_val = -(spin20_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //同理
		}
		set_pid_target(&pid_location, -Car_Turn_val);   
		set_pid_target(&pid_location2, Car_Turn_val);
		
		g_lMotorPulseSigma = 0;    //之前没清除所以用不了，哈哈哈哈哈
		g_lMotor2PulseSigma = 0; 

}






void Xunji_speed_control(void)//循迹差速串级PID
{
    xunji_control_count++;
    if(xunji_control_count>=2)
    {
        xunji_control_count = 0;
        line_out = PID_realize(&pid_JS,Line_Num);
    }
    line_out1 = 50 + line_out;
    line_out2 = 50 - line_out;
    
    line_out1 = (line_out1 > 65) ? 65 : (line_out1 < -65) ? -65 : line_out1;
    line_out2 = (line_out2 > 65) ? 65 : (line_out2 < -65) ? -65 : line_out2; 
    
    set_pid_target(&pid_speed, line_out1);   
    set_pid_target(&pid_speed2, line_out2);
    
    Xunji_Outval = speed_pid_control();
    Xunji_Outval2 = speed2_pid_control(); 
}


void mpu6050_control(void)
{
    mpu6050_control_count++;
    if(mpu6050_control_count>=2)
    {
        mpu6050_get_data = 1;
        turn_pwm = mpu6050_pid_control(&pid_mpu6050,gyroz);//定时器周期为20ms
    }
     turn_pwm1 = -turn_pwm;
     turn_pwm2 = +turn_pwm;
    
     turn_pwm1 = (turn_pwm1 > 40) ? 40 : (turn_pwm1 < -40) ? -40 : turn_pwm1;
     turn_pwm2 = (turn_pwm2 > 40) ? 40 : (turn_pwm2 < -40) ? -40 : turn_pwm2;
    
     set_pid_target(&pid_speed, turn_pwm1);   
     set_pid_target(&pid_speed2, turn_pwm2);
    
    
     mpu6050_Outval = speed_pid_control();
     mpu6050_Outval2 = speed2_pid_control(); 
}




/*********************各PID**********************/
		/****速度环位置环串级PID控制*****/   


void Location_Speed_control(void)          //原地转向可以直接 调用这个
{
	 location_control_count++;
	if(location_control_count >= 2)
	{
		location_control_count = 0; 
		
		location_Outval = location_pid_control();
		location2_Outval = location2_pid_control();
	}
	
	set_pid_target(&pid_speed, location_Outval); //每次都必须有位置环的值     //调试速度环PID时取消这两句
	set_pid_target(&pid_speed2, location2_Outval); //每次都必须有位置环的值
 
	speed_Outval = speed_pid_control();    //要是电机转向不符合预期，就在这两句里取反数值
	speed2_Outval = speed2_pid_control();  

}





float location_pid_control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	
	  actual_location =  g_lMotorPulseSigma;   //1圈 = 2464个脉冲 = 56*11*4  //这里位置用圈数代替。

    cont_val = location_pid_realize(&pid_location, actual_location);   
	  
	  //还没加串级pID之前，位置环的cont_val对应PWM。 改成串级PID后，位置换的cont_val对应目标速度
	    
     	/* 目标速度上限处理 */
      if (cont_val > TARGET_SPEED_MAX)
      {
        cont_val = TARGET_SPEED_MAX;
      }
      else if (cont_val < -TARGET_SPEED_MAX)
      {
        cont_val = -TARGET_SPEED_MAX;
      }
	
//	 #if (PID_ASSISTANT_EN)
//		i++;
//		if(i==8)
//		{
//			i=0;
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1); // 给通道 1 发送实际值
//		}
//	 #endif

	return cont_val;
}


float speed_pid_control(void)  
{
   
    float cont_val = 0.0;                       // 当前控制值
    int32_t actual_speed;
	
	
	  actual_speed = ((float)g_nMotorPulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

		speedzuo = actual_speed;
    cont_val = speed_pid_realize(&pid_speed, actual_speed);    // 进行 PID 计算
		
//	 #if (PID_ASSISTANT_EN)
//		i++;
//		if(i==8)
//		{
//			i=0;
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1); // 给通道 1 发送实际值
//		}
//  #endif
	
	return cont_val;
}


float location2_pid_control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	  actual_location =  g_lMotor2PulseSigma;   //1圈 = 2464个脉冲 = 56*11*4  //这里位置用圈数代替。

    cont_val = location_pid_realize(&pid_location2, actual_location);   
	  
	  // 改成串级PID后，位置换的cont_val对应目标速度
	
	//目标速度限幅
	    	/* 目标速度上限处理 */
      if (cont_val > TARGET_SPEED_MAX)
      {
        cont_val = TARGET_SPEED_MAX;
      }
      else if (cont_val < -TARGET_SPEED_MAX)
      {
        cont_val = -TARGET_SPEED_MAX;
      }
	
//	 #if (PID_ASSISTANT_EN)
//		i++;
//		if(i==8)
//		{
//			i=0;
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1); // 给通道 1 发送实际值
//		}
//	 #endif
			
	
	return cont_val;
}


float speed2_pid_control(void)  
{
   
    float cont_val = 0.0;                       // 当前控制值
    int32_t actual_speed;    
	
	
	  actual_speed = ((float)g_nMotor2Pulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

	
    cont_val = speed_pid_realize(&pid_speed2, actual_speed);    // 进行 PID 计算
    speedyou = actual_speed;
//	 #if (PID_ASSISTANT_EN)
//		i++;
//		if(i==8)
//		{
//			i=0;
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1); // 给通道 1 发送实际值
//		}
//  #endif
	
	return cont_val;
}




/**************************************************************************
函数功能：赋值给PWM寄存器
入口参数：PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
    	if(moto1>0)			AIN2=0,			AIN1=1;
			else 	          AIN2=1,			AIN1=0;
		TIM_SetCompare1(TIM1,myabs(moto1));//电机控制
	    if(moto2<0)			BIN2=0,			BIN1=1;
			else 	          BIN2=1,			BIN1=0;

	    TIM_SetCompare4(TIM1,myabs(moto2));//电机控制
}



/**************************************************************************
函数功能：限制PWM赋值 
入口参数：无
返回  值：无
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=6000;    //===PWM满幅是7200 限制在7100
	  if(Moto1<-Amplitude) Moto1=-Amplitude;	
		if(Moto1>Amplitude)  Moto1=Amplitude;	
	
		if(Moto2<-Amplitude) Moto2=-Amplitude;	
		if(Moto2>Amplitude)  Moto2=Amplitude;		
	
}

/**************************************************************************
函数功能：取绝对值
入口参数：int
返回  值：unsigned int
**************************************************************************/
int myabs(int a)
{ 		   
	  int temp;
		if(a<0)  temp=-a;  
	  else temp=a;
	  return temp;
}




/**************************************************************************
Function: Get angle
Input   : way：The algorithm of getting angle 1：DMP  2：kalman  3：Complementary filtering
Output  : none
函数功能：获取角度	
入口参数：way：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/	
void Get_Angle(u8 way)
{ 
	float Accel_Y,Accel_Z,Accel_X,Accel_Angle_x,Accel_Angle_y,Gyro_X,Gyro_Z,Gyro_Y;
	Temperature=Read_Temperature();      //读取MPU6050内置温度传感器数据，近似表示主板温度。
	if(way==1)                           //DMP的读取在数据采集中断读取，严格遵循时序要求
	{	
		Read_DMP();                      	 //读取加速度、角速度、倾角
		Angle_Balance=Pitch;             	 //更新平衡倾角,前倾为正，后倾为负
		Gyro_Balance=gyro[0];              //更新平衡角速度,前倾为正，后倾为负
		Gyro_Turn=gyro[2];                 //更新转向角速度
		Acceleration_Z=accel[2];           //更新Z轴加速度计
	}			
	else
	{
		Gyro_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L);    //读取X轴陀螺仪
		Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //读取Z轴陀螺仪
		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度计
		Accel_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L); //读取X轴加速度计
		Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //读取Z轴加速度计
		if(Gyro_X>32768)  Gyro_X-=65536;                 //数据类型转换  也可通过short强制类型转换
		if(Gyro_Y>32768)  Gyro_Y-=65536;                 //数据类型转换  也可通过short强制类型转换
		if(Gyro_Z>32768)  Gyro_Z-=65536;                 //数据类型转换
		if(Accel_X>32768) Accel_X-=65536;                //数据类型转换
		if(Accel_Y>32768) Accel_Y-=65536;                //数据类型转换
		if(Accel_Z>32768) Accel_Z-=65536;                //数据类型转换
		Gyro_Balance=-Gyro_X;                            //更新平衡角速度
		Accel_Angle_x=atan2(Accel_Y,Accel_Z)*180/PI;     //计算倾角，转换单位为度	
		Accel_Angle_y=atan2(Accel_X,Accel_Z)*180/PI;     //计算倾角，转换单位为度
		Gyro_X=Gyro_X/16.4;                              //陀螺仪量程转换，量程±2000°/s对应灵敏度16.4，可查手册
		Gyro_Y=Gyro_Y/16.4;                              //陀螺仪量程转换	
		if(Way_Angle==2)		  	
		{
			 Pitch = -Kalman_Filter_x(Accel_Angle_x,Gyro_X);//卡尔曼滤波
			 Roll = -Kalman_Filter_y(Accel_Angle_y,Gyro_Y);
		}
		else if(Way_Angle==3) 
		{  
			 Pitch = -Complementary_Filter_x(Accel_Angle_x,Gyro_X);//互补滤波
			 Roll = -Complementary_Filter_y(Accel_Angle_y,Gyro_Y);
		}
		Angle_Balance=Pitch;                              //更新平衡倾角
		Gyro_Turn=Gyro_Z;                                 //更新转向角速度
		Acceleration_Z=Accel_Z;                           //更新Z轴加速度计	
	}
}
