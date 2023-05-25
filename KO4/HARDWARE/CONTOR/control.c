#include "control.h"



u8 Way_Angle=2;                             //��ȡ�Ƕȵ��㷨��1����Ԫ��  2��������  3�������˲� 
float Angle_Balance,Gyro_Balance,Gyro_Turn; //ƽ����� ƽ�������� ת��������
int Temperature;                            //�¶ȱ���
float Acceleration_Z;                       //Z����ٶȼ�

int Moto1,Moto2;//���PWM���� Ӧ��Motor�� ��Moto�¾�
float speedzuo = 0;
float speedyou = 0;
int i=0;
float Motor_journey_cm, Motor2_journey_cm;
float g_fTargetJourney = 0;  //���С������������·�̺� �� ��λcm����Ҫ����һ�׶�����������
float  MotorPWM =0.0, Motor2PWM =0.0;

long g_lMotorPulseSigma =0;
long g_lMotor2PulseSigma=0;
int g_nMotorPulse=0,g_nMotor2Pulse=0;//ȫ�ֱ����� ������������ֵ
unsigned char location_control_count = 0;  //ִ��Ƶ�ʲ���Ҫ��ô�ߵ�������¼������������ж���
unsigned char xunji_control_count = 0;  //ִ��Ƶ�ʲ���Ҫ��ô�ߵ�������¼������������ж���
unsigned char mpu6050_control_count = 0;  //ִ��Ƶ�ʲ���Ҫ��ô�ߵ�������¼������������ж���
float speed_Outval, location_Outval;
float speed2_Outval, location2_Outval;


u8 Spin_start_flag , Spin_succeed_flag , Stop_Flag;
u8 stop_count , spin_count;
u8 Line_flag = 0, Turn_flag = 0,Xunji_flag = 0 ;

int line_out = 0;
int line_out1,line_out2;
float Xunji_Outval,Xunji_Outval2;


float Target_turn = 90;//90��kp = 6,180��kp = 4.5,ÿ���Ƕȶ�Ӧ��pid��һ�������Ե���Ҫת��ͬ�ĽǶ�ʱ�����趨��pidҲ��һ��
float actual_angle = 0;//����������֮����ʾ�ľͲ���ʵ�ʽǶ���
short gyrox,gyroy,gyroz;				//������--���ٶ�
float turn_pwm;
float turn_pwm1;
float turn_pwm2;
float mpu6050_Outval,mpu6050_Outval2;
int mpu6050_get_data = 0;


void EXTI15_10_IRQHandler(void) 
{    
   if(EXTI_GetITStatus(EXTI_Line12)!=0)//һ���ж�
	{
	if(INT==0)		
	{   
		EXTI_ClearITPendingBit(EXTI_Line12);//����жϱ�־λ   
		Get_Angle(Way_Angle);                     					//������̬��5msһ�Σ����ߵĲ���Ƶ�ʿ��Ը��ƿ������˲��ͻ����˲���Ч��
	 }
    }    
} 

void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������  2ms�ж�һ��   
		{ 
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);                        //===�����ʱ��1�жϱ�־λ
			
//		g_nMotorPulse = +Read_Encoder(2);
//			
//		g_nMotor2Pulse = -Read_Encoder(4);
//			
//		g_lMotorPulseSigma += g_nMotorPulse;//λ���⻷ʹ�õ������ۻ�      //���һ��λ�ÿ���֮��������
//		g_lMotor2PulseSigma += g_nMotor2Pulse;//λ���⻷ʹ�õ������ۻ�   //��¼��֮��ҲҪ��ʱ����ѽ������
//			
//			
//		Motor_journey_cm	  = ( g_lMotorPulseSigma /(30*13*4) ) * (6.5*3.14);
//		Motor2_journey_cm	 =  ( g_lMotor2PulseSigma /(30*13*4) ) * (6.5*3.14);	
//		
//       mpu6050_control();
//       MotorPWM = mpu6050_Outval; 
//       Motor2PWM = mpu6050_Outval2;    
//       Set_Pwm( MotorPWM, Motor2PWM); 
            
            
            
//        if(Xunji_flag==1)//ѭ��
//        {
//            Light_GoStraight_control();//ֻ������ѭ����ʱ��Ŷ�ȡ�Ҷ�
//            Xunji_speed_control();    
//                
//            MotorPWM = Xunji_Outval; 
//            Motor2PWM = Xunji_Outval2;    
//            Set_Pwm( MotorPWM, Motor2PWM); 
//        }            

//         if(Line_flag==1)//��ֱ�߹̶�����
//         {
//             
//             if( (Motor_journey_cm <= (g_fTargetJourney + 25)) && (Motor_journey_cm >= (g_fTargetJourney - 25)) )  //����ƫ����ô����ĺ��𣿲��������ƫ��
//            {
//				stop_count++;   //stop_count���ܳ���256
//				if(stop_count >=  100)  //100 * 20 = 1.6s  ����ҲҪ������Ŀ��λ��ͣ��1s  //����ʱ���жϷų��㣬�Ա�ɲ��ͣ��
//				{
//                    Line_flag = 0;
//                    Stop_Flag = 1; //�����־λ���������ж��Ƿ�ִ����һ�׶�����
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
//            Location_Speed_control(); //λ�û��ٶȻ�����PID��������ٶȻ������PWMֵ
//                
//                
//            MotorPWM = speed_Outval; 
//            Motor2PWM = speed2_Outval; 	
//            
//            Set_Pwm(MotorPWM,Motor2PWM);
//         }
//         
//         
//         	if( Spin_start_flag == 1)  //ת��
//			{
//				
//						Location_Speed_control(); //λ�û��ٶȻ�����PID��������ٶȻ������PWMֵ

//						
//						MotorPWM = speed_Outval;
//						Motor2PWM = speed2_Outval;
//						
//						spin_count++;
//						if( spin_count >= 100)      //20ms����һ��   100*20 = 2s�����ܹ�����ɵ�תʱ��Ϊ���ޣ�Ӧ���Ѿ���ת�����
//						{
//							 Spin_start_flag = 0;
//							 spin_count = 0;
//							
//							//ת���е����⣬ת��֮��һֱת,��ʱ������������
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


         /*�����ٶȻ�*/
//		MotorPWM = speed_pid_control(); 
//		Motor2PWM = speed2_pid_control(); 
          /*����λ�û�--��ʵλ�û�ֱ�ӿ�ʵ��Ч������OK�ģ�ֻ��Ҫ��KP*/
		//MotorPWM = location_pid_control();  
		//Motor2PWM = location2_pid_control();


void Car_go(int32_t location_cm)   //ֱ�ߺ���  
{
	float Car_location;
	
	Line_flag = 1;   
	Stop_Flag = 0;
	
	Spin_start_flag = 0;
	Spin_succeed_flag = 0;  
	
	Motor_journey_cm = 0;
	Motor2_journey_cm = 0;
	
	g_fTargetJourney = location_cm;   //��ֹ��ʱ��PID������
	
	//���Ѳ��Ч�����þͽ�3.142�Ӵ�
	Car_location   = ( location_cm / (6.5 * 3.14) ) *   (30*4*13) ;    //Car_location ��  location_cm ת��Ϊ��Ӧ��������   g_fTargetJourney = Ҫת����Ȧ * 1Ȧ�������� 
	//С��λ��ֱ����һ��������������ۻ��ͺã�ת��ʱ����������ʼһ��λ��ǰֱ�����㡣   
	 
	  set_pid_target(&pid_location, Car_location);   
	  set_pid_target(&pid_location2, Car_location);
		
	  g_lMotorPulseSigma = 0;    //֮ǰû��������ò���
	  g_lMotor2PulseSigma = 0; 
	
		
}



void spin_Turn(spin_dir_t zhuanxiang)   //����С�����־�(mm) 175mm  //��ʵת�򻷲�����Ҫ�ܾ�׼��ת���ֱ����ֱ��ʱ��Ѳ�ߺ�����������ͺ�
{
		float spin90_val;//��������С��ת90�ȵ���������ת���ľ���
        float spin45_val;//��������С��ת45�ȵ���������ת���ľ���
        float spin10_val;//��������С��ת45�ȵ���������ת���ľ���
        float spin20_val;//��������С��ת45�ȵ���������ת���ľ���
		float Car_Turn_val;
     
    	Line_flag = 0;  //������Ѳ�ߵĲ�����
        Stop_Flag = 0;   //ִ��ת��ʱ����ֱ����ɵı�־λ����. �������һ����ֱ�У������ת�䣬������ҵ��������ֶ���λ
        Spin_start_flag = 1;   
        Spin_succeed_flag = 0; 
		
		Motor_journey_cm = 0;
		Motor2_journey_cm = 0;
		
		spin90_val = 0.25*3.1416*LUN_JU;
        spin45_val = 0.125*3.1416*LUN_JU;
        spin10_val = 0.028*3.1416*LUN_JU;
        spin20_val = 0.056*3.1416*LUN_JU;
		
	/****ת���ѭ�߿��Ե�ת��ϵ��**********/
		if(zhuanxiang == left_90) //��ת90
		{
			Car_Turn_val = ( spin90_val / (6.5 * 3.142) ) *   (30*4*13) ; 
			Car_Turn_val =  Car_Turn_val;   //90*0.94 = 84.6   //����Ӱ�죬����ת��������趨�Ķࡣֱ����90�Ƚ�������Ѳ�߾�Ѱ��������
			
		}
		else if(zhuanxiang == right_90)//��ת90
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.142) ) *   (30*4*13); 
			Car_Turn_val =  Car_Turn_val;  //90*0.96 = 86.4    //����Ӱ�죬����ת��������趨�Ķࡣ ��������Ѳ�߿��ܾ�Ѱ��������
		}
		else if(zhuanxiang == back_180)//���ת180
		{
			Car_Turn_val = -(spin90_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val = Car_Turn_val;   //ͬ��
			Car_Turn_val = 2*Car_Turn_val;      //0.96*180 = 175.5
		}
        else if(zhuanxiang == left_45)//��ת45��
		{
			Car_Turn_val = (spin45_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //ͬ��
		}
        else if(zhuanxiang == right_45)//��ת45��
		{
			Car_Turn_val = -(spin45_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //ͬ��
		}
         else if(zhuanxiang == right_10)//��ת10��
		{
			Car_Turn_val = -(spin10_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //ͬ��
		}
          else if(zhuanxiang == right_20)//��ת20��
		{
			Car_Turn_val = -(spin20_val / (6.5 * 3.142) ) *   (30*4*13);
			Car_Turn_val =  Car_Turn_val;   //ͬ��
		}
		set_pid_target(&pid_location, -Car_Turn_val);   
		set_pid_target(&pid_location2, Car_Turn_val);
		
		g_lMotorPulseSigma = 0;    //֮ǰû��������ò��ˣ�����������
		g_lMotor2PulseSigma = 0; 

}






void Xunji_speed_control(void)//ѭ�����ٴ���PID
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
        turn_pwm = mpu6050_pid_control(&pid_mpu6050,gyroz);//��ʱ������Ϊ20ms
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




/*********************��PID**********************/
		/****�ٶȻ�λ�û�����PID����*****/   


void Location_Speed_control(void)          //ԭ��ת�����ֱ�� �������
{
	 location_control_count++;
	if(location_control_count >= 2)
	{
		location_control_count = 0; 
		
		location_Outval = location_pid_control();
		location2_Outval = location2_pid_control();
	}
	
	set_pid_target(&pid_speed, location_Outval); //ÿ�ζ�������λ�û���ֵ     //�����ٶȻ�PIDʱȡ��������
	set_pid_target(&pid_speed2, location2_Outval); //ÿ�ζ�������λ�û���ֵ
 
	speed_Outval = speed_pid_control();    //Ҫ�ǵ��ת�򲻷���Ԥ�ڣ�������������ȡ����ֵ
	speed2_Outval = speed2_pid_control();  

}





float location_pid_control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	
	  actual_location =  g_lMotorPulseSigma;   //1Ȧ = 2464������ = 56*11*4  //����λ����Ȧ�����档

    cont_val = location_pid_realize(&pid_location, actual_location);   
	  
	  //��û�Ӵ���pID֮ǰ��λ�û���cont_val��ӦPWM�� �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�
	    
     	/* Ŀ���ٶ����޴��� */
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
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1); // ��ͨ�� 1 ����ʵ��ֵ
//		}
//	 #endif

	return cont_val;
}


float speed_pid_control(void)  
{
   
    float cont_val = 0.0;                       // ��ǰ����ֵ
    int32_t actual_speed;
	
	
	  actual_speed = ((float)g_nMotorPulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

		speedzuo = actual_speed;
    cont_val = speed_pid_realize(&pid_speed, actual_speed);    // ���� PID ����
		
//	 #if (PID_ASSISTANT_EN)
//		i++;
//		if(i==8)
//		{
//			i=0;
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1); // ��ͨ�� 1 ����ʵ��ֵ
//		}
//  #endif
	
	return cont_val;
}


float location2_pid_control(void)  
{
	float cont_val = 0.0; 
	int32_t actual_location;
	
	  actual_location =  g_lMotor2PulseSigma;   //1Ȧ = 2464������ = 56*11*4  //����λ����Ȧ�����档

    cont_val = location_pid_realize(&pid_location2, actual_location);   
	  
	  // �ĳɴ���PID��λ�û���cont_val��ӦĿ���ٶ�
	
	//Ŀ���ٶ��޷�
	    	/* Ŀ���ٶ����޴��� */
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
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_location, 1); // ��ͨ�� 1 ����ʵ��ֵ
//		}
//	 #endif
			
	
	return cont_val;
}


float speed2_pid_control(void)  
{
   
    float cont_val = 0.0;                       // ��ǰ����ֵ
    int32_t actual_speed;    
	
	
	  actual_speed = ((float)g_nMotor2Pulse*1000.0*60.0)/(ENCODER_TOTAL_RESOLUTION*REDUCTION_RATIO*SPEED_PID_PERIOD);

	
    cont_val = speed_pid_realize(&pid_speed2, actual_speed);    // ���� PID ����
    speedyou = actual_speed;
//	 #if (PID_ASSISTANT_EN)
//		i++;
//		if(i==8)
//		{
//			i=0;
//			set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed, 1); // ��ͨ�� 1 ����ʵ��ֵ
//		}
//  #endif
	
	return cont_val;
}




/**************************************************************************
�������ܣ���ֵ��PWM�Ĵ���
��ڲ�����PWM
����  ֵ����
**************************************************************************/
void Set_Pwm(int moto1,int moto2)
{
    	if(moto1>0)			AIN2=0,			AIN1=1;
			else 	          AIN2=1,			AIN1=0;
		TIM_SetCompare1(TIM1,myabs(moto1));//�������
	    if(moto2<0)			BIN2=0,			BIN1=1;
			else 	          BIN2=1,			BIN1=0;

	    TIM_SetCompare4(TIM1,myabs(moto2));//�������
}



/**************************************************************************
�������ܣ�����PWM��ֵ 
��ڲ�������
����  ֵ����
**************************************************************************/
void Xianfu_Pwm(void)
{	
	  int Amplitude=6000;    //===PWM������7200 ������7100
	  if(Moto1<-Amplitude) Moto1=-Amplitude;	
		if(Moto1>Amplitude)  Moto1=Amplitude;	
	
		if(Moto2<-Amplitude) Moto2=-Amplitude;	
		if(Moto2>Amplitude)  Moto2=Amplitude;		
	
}

/**************************************************************************
�������ܣ�ȡ����ֵ
��ڲ�����int
����  ֵ��unsigned int
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
Input   : way��The algorithm of getting angle 1��DMP  2��kalman  3��Complementary filtering
Output  : none
�������ܣ���ȡ�Ƕ�	
��ڲ�����way����ȡ�Ƕȵ��㷨 1��DMP  2�������� 3�������˲�
����  ֵ����
**************************************************************************/	
void Get_Angle(u8 way)
{ 
	float Accel_Y,Accel_Z,Accel_X,Accel_Angle_x,Accel_Angle_y,Gyro_X,Gyro_Z,Gyro_Y;
	Temperature=Read_Temperature();      //��ȡMPU6050�����¶ȴ��������ݣ����Ʊ�ʾ�����¶ȡ�
	if(way==1)                           //DMP�Ķ�ȡ�����ݲɼ��ж϶�ȡ���ϸ���ѭʱ��Ҫ��
	{	
		Read_DMP();                      	 //��ȡ���ٶȡ����ٶȡ����
		Angle_Balance=Pitch;             	 //����ƽ�����,ǰ��Ϊ��������Ϊ��
		Gyro_Balance=gyro[0];              //����ƽ����ٶ�,ǰ��Ϊ��������Ϊ��
		Gyro_Turn=gyro[2];                 //����ת����ٶ�
		Acceleration_Z=accel[2];           //����Z����ٶȼ�
	}			
	else
	{
		Gyro_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L);    //��ȡX��������
		Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //��ȡY��������
		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //��ȡZ��������
		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //��ȡX����ٶȼ�
		Accel_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L); //��ȡX����ٶȼ�
		Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //��ȡZ����ٶȼ�
		if(Gyro_X>32768)  Gyro_X-=65536;                 //��������ת��  Ҳ��ͨ��shortǿ������ת��
		if(Gyro_Y>32768)  Gyro_Y-=65536;                 //��������ת��  Ҳ��ͨ��shortǿ������ת��
		if(Gyro_Z>32768)  Gyro_Z-=65536;                 //��������ת��
		if(Accel_X>32768) Accel_X-=65536;                //��������ת��
		if(Accel_Y>32768) Accel_Y-=65536;                //��������ת��
		if(Accel_Z>32768) Accel_Z-=65536;                //��������ת��
		Gyro_Balance=-Gyro_X;                            //����ƽ����ٶ�
		Accel_Angle_x=atan2(Accel_Y,Accel_Z)*180/PI;     //������ǣ�ת����λΪ��	
		Accel_Angle_y=atan2(Accel_X,Accel_Z)*180/PI;     //������ǣ�ת����λΪ��
		Gyro_X=Gyro_X/16.4;                              //����������ת�������̡�2000��/s��Ӧ������16.4���ɲ��ֲ�
		Gyro_Y=Gyro_Y/16.4;                              //����������ת��	
		if(Way_Angle==2)		  	
		{
			 Pitch = -Kalman_Filter_x(Accel_Angle_x,Gyro_X);//�������˲�
			 Roll = -Kalman_Filter_y(Accel_Angle_y,Gyro_Y);
		}
		else if(Way_Angle==3) 
		{  
			 Pitch = -Complementary_Filter_x(Accel_Angle_x,Gyro_X);//�����˲�
			 Roll = -Complementary_Filter_y(Accel_Angle_y,Gyro_Y);
		}
		Angle_Balance=Pitch;                              //����ƽ�����
		Gyro_Turn=Gyro_Z;                                 //����ת����ٶ�
		Acceleration_Z=Accel_Z;                           //����Z����ٶȼ�	
	}
}
