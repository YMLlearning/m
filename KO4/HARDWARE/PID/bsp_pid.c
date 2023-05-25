#include "bsp_pid.h"
#include "protocol.h"
#include "common.h"
#include "control.h"

/*����һ��PID�ṹ���͵�ȫ�ֱ���*/
//����ȫ�ֱ���

_pid pid_speed, pid_speed2;    
_pid pid_location, pid_location2;
_pid pid_JS;
_pid pid_mpu6050;
/**
  * @brief  PID������ʼ��
  *	@note 	��
  * @retval ��
  */
void PID_param_init()
{
	float pid_temp[3];

	/* ��ʼ������ */
/* λ����س�ʼ������ */
    pid_location.target_val=0.0;				
    pid_location.actual_val=0.0;
    pid_location.err=0.0;
    pid_location.err_last=0.0;
    pid_location.integral=0.0;
  
		pid_location.Kp = 0.5;
		pid_location.Ki = 0.0;
		pid_location.Kd = 0.0;
  
  	/* �ٶ���س�ʼ������ */
    pid_speed.target_val=0.0;				
    pid_speed.actual_val=0.0;
    pid_speed.err=0.0;
    pid_speed.err_last=0.0;
    pid_speed.integral=0.0;
  
		pid_speed.Kp = 100;
		pid_speed.Ki = 0.1;
		pid_speed.Kd = 0.0;
		
			/* λ����س�ʼ������ */
    pid_location2.target_val=0.0;				
    pid_location2.actual_val=0.0;
    pid_location2.err=0.0;
    pid_location2.err_last=0.0;
    pid_location2.integral=0.0;
  
		pid_location2.Kp = 0.5;
		pid_location2.Ki = 0.0;
		pid_location2.Kd = 0.0;
  
  	/* �ٶ���س�ʼ������ */
    pid_speed2.target_val=0.0;				
    pid_speed2.actual_val=0.0;
    pid_speed2.err=0.0;
    pid_speed2.err_last=0.0;
    pid_speed2.integral=0.0;
  
		pid_speed2.Kp = 100;
		pid_speed2.Ki = 0.1;
		pid_speed2.Kd =0.0;
        
        
        
      	/* ѭ��������س�ʼ������ */
    pid_JS.target_val=0.0;				
    pid_JS.actual_val=0.0;
    pid_JS.err=0.0;
    pid_JS.err_last=0.0;
    pid_JS.integral=0.0;
  
		pid_JS.Kp = 4;
		pid_JS.Ki = 0.0;
		pid_JS.Kd =0.0;
        
        
        /* mpu6050��س�ʼ������ */
    pid_mpu6050.target_val=90.0;				
    pid_mpu6050.actual_val=0.0;
    pid_mpu6050.err=0.0;
    pid_mpu6050.err_last=0.0;
    pid_mpu6050.integral=0.0;
  
		pid_mpu6050.Kp = 0.25;
		pid_mpu6050.Ki = 0.0;
		pid_mpu6050.Kd =0.0;    
      

	
	
	
	
//#if defined(PID_ASSISTANT_EN)
//	pid_temp[0] = pid_speed.Kp;
//	pid_temp[1] = pid_speed.Ki;
//	pid_temp[2] = pid_speed.Kd;
//	set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // ��ͨ��1����PIDֵ
//#endif
}

extern float det_plus;
/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
	*	@note 	��
  * @retval ��
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
	*	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(_pid *pid)
{
  return pid->target_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ���ñ��������֡�΢��ϵ��
  * @param  p������ϵ�� P
  * @param  i������ϵ�� i
  * @param  d��΢��ϵ�� d
  *	@note 	��
  * @retval ��
  */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
        pid->Kp = p;    // ���ñ���ϵ�� P
		pid->Ki = i;    // ���û���ϵ�� I
		pid->Kd = d;    // ����΢��ϵ�� D
}


/**
  * @brief  λ��PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float location_pid_realize(_pid *pid, float actual_val)  //λ�û����Kp����Ҳ����
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-actual_val;
  
//    /* �趨�ջ����� */   //�⻷�������Բ�Ҫ 
//    if((pid->err >= -0.1) && (pid->err <= 0.1)) 
//    {
//      pid->err = 0;
//      pid->integral = 0;
//    }
    
    pid->integral += pid->err;    // ����ۻ�

		/*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                  +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}

/**
  * @brief  �ٶ�PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-actual_val;

    if((pid->err<0.5f ) && (pid->err>-0.5f))   //��������������ٶ�ƫ������1���ӣ���������ƫ��Ϊ��Ȧ
		{
      pid->err = 0.0f;
		}
	
		
    pid->integral += pid->err;    // ����ۻ�
	
	
	  /*�����޷�*/
	   	 if (pid->integral >= 1000) {pid->integral =1000;}
      else if (pid->integral < -1000)  {pid->integral = -1000;}

		/*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                   +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}



// λ��ʽPID���ƺ���
float PID_realize(_pid *pid, float actual_val)
{
	pid->actual_val = actual_val;//������ʵֵ
	pid->err = pid->target_val - pid->actual_val;////��ǰ���=Ŀ��ֵ-��ʵֵ
	pid->integral += pid->err;//����ۼ�ֵ = ��ǰ����ۼƺ�
	pid->integral=(pid->integral>8000)?pid->integral=8000:((pid->integral<-8000)?pid->integral=-8000:pid->integral);
	//ʹ��PID���� ��� = Kp*��ǰ���  +  Ki*����ۼ�ֵ + Kd*(��ǰ���-�ϴ����)
	pid->actual_val = pid->Kp*pid->err + pid->Ki*pid->integral + pid->Kd*(pid->err - pid->err_last);
	//�����ϴ����: �����ֵ���ϴ����
	pid->err_last = pid->err;
	
	return pid->actual_val;//��Ϊ���PWM
}


float mpu6050_pid_control(_pid *pid,float Gyroz)//dt�ĵ�λ��s
{
    if(myabs(Gyroz)<150)
    {
        Gyroz = 0;
    }
    pid->actual_val += Gyroz;//���µ�ǰ�Ƕ�   ���������ǲ�����������������⣬����һֱת�ĽǶȹ�����С
    pid->err = pid->target_val - pid->actual_val;////��ǰ���=Ŀ��ֵ-��ʵֵ
    pid->integral += pid->err;//����ۼ�ֵ = ��ǰ����ۼƺ�
    pid->integral=(pid->integral>1000)?pid->integral=1000:((pid->integral<-1000)?pid->integral=-1000:pid->integral);
    //ʹ��PID���� ��� = Kp*��ǰ���  +  Ki*����ۼ�ֵ + Kd*(��ǰ���-�ϴ����)
	pid->actual_val = pid->Kp*pid->err + pid->Ki*pid->integral + pid->Kd*(pid->err - pid->err_last);
	//�����ϴ����: �����ֵ���ϴ����
	pid->err_last = pid->err;
    
    return pid->actual_val;//��Ϊ���뻷������
}