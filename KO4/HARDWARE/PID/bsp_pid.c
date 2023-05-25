#include "bsp_pid.h"
#include "protocol.h"
#include "common.h"
#include "control.h"

/*定义一个PID结构体型的全局变量*/
//定义全局变量

_pid pid_speed, pid_speed2;    
_pid pid_location, pid_location2;
_pid pid_JS;
_pid pid_mpu6050;
/**
  * @brief  PID参数初始化
  *	@note 	无
  * @retval 无
  */
void PID_param_init()
{
	float pid_temp[3];

	/* 初始化参数 */
/* 位置相关初始化参数 */
    pid_location.target_val=0.0;				
    pid_location.actual_val=0.0;
    pid_location.err=0.0;
    pid_location.err_last=0.0;
    pid_location.integral=0.0;
  
		pid_location.Kp = 0.5;
		pid_location.Ki = 0.0;
		pid_location.Kd = 0.0;
  
  	/* 速度相关初始化参数 */
    pid_speed.target_val=0.0;				
    pid_speed.actual_val=0.0;
    pid_speed.err=0.0;
    pid_speed.err_last=0.0;
    pid_speed.integral=0.0;
  
		pid_speed.Kp = 100;
		pid_speed.Ki = 0.1;
		pid_speed.Kd = 0.0;
		
			/* 位置相关初始化参数 */
    pid_location2.target_val=0.0;				
    pid_location2.actual_val=0.0;
    pid_location2.err=0.0;
    pid_location2.err_last=0.0;
    pid_location2.integral=0.0;
  
		pid_location2.Kp = 0.5;
		pid_location2.Ki = 0.0;
		pid_location2.Kd = 0.0;
  
  	/* 速度相关初始化参数 */
    pid_speed2.target_val=0.0;				
    pid_speed2.actual_val=0.0;
    pid_speed2.err=0.0;
    pid_speed2.err_last=0.0;
    pid_speed2.integral=0.0;
  
		pid_speed2.Kp = 100;
		pid_speed2.Ki = 0.1;
		pid_speed2.Kd =0.0;
        
        
        
      	/* 循迹差速相关初始化参数 */
    pid_JS.target_val=0.0;				
    pid_JS.actual_val=0.0;
    pid_JS.err=0.0;
    pid_JS.err_last=0.0;
    pid_JS.integral=0.0;
  
		pid_JS.Kp = 4;
		pid_JS.Ki = 0.0;
		pid_JS.Kd =0.0;
        
        
        /* mpu6050相关初始化参数 */
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
//	set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // 给通道1发送PID值
//#endif
}

extern float det_plus;
/**
  * @brief  设置目标值
  * @param  val		目标值
	*	@note 	无
  * @retval 无
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
float get_pid_target(_pid *pid)
{
  return pid->target_val;    // 设置当前的目标值
}

/**
  * @brief  设置比例、积分、微分系数
  * @param  p：比例系数 P
  * @param  i：积分系数 i
  * @param  d：微分系数 d
  *	@note 	无
  * @retval 无
  */
void set_p_i_d(_pid *pid, float p, float i, float d)
{
        pid->Kp = p;    // 设置比例系数 P
		pid->Ki = i;    // 设置积分系数 I
		pid->Kd = d;    // 设置微分系数 D
}


/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float location_pid_realize(_pid *pid, float actual_val)  //位置环光个Kp好像也可以
{
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-actual_val;
  
//    /* 设定闭环死区 */   //外环死区可以不要 
//    if((pid->err >= -0.1) && (pid->err <= 0.1)) 
//    {
//      pid->err = 0;
//      pid->integral = 0;
//    }
    
    pid->integral += pid->err;    // 误差累积

		/*PID算法实现*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                  +pid->Kd*(pid->err-pid->err_last);
  
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回当前实际值*/
    return pid->actual_val;
}

/**
  * @brief  速度PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-actual_val;

    if((pid->err<0.5f ) && (pid->err>-0.5f))   //假如以最大允许速度偏差运行1分钟，输出轴最大偏差为半圈
		{
      pid->err = 0.0f;
		}
	
		
    pid->integral += pid->err;    // 误差累积
	
	
	  /*积分限幅*/
	   	 if (pid->integral >= 1000) {pid->integral =1000;}
      else if (pid->integral < -1000)  {pid->integral = -1000;}

		/*PID算法实现*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                   +pid->Kd*(pid->err-pid->err_last);
  
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回当前实际值*/
    return pid->actual_val;
}



// 位置式PID控制函数
float PID_realize(_pid *pid, float actual_val)
{
	pid->actual_val = actual_val;//传递真实值
	pid->err = pid->target_val - pid->actual_val;////当前误差=目标值-真实值
	pid->integral += pid->err;//误差累计值 = 当前误差累计和
	pid->integral=(pid->integral>8000)?pid->integral=8000:((pid->integral<-8000)?pid->integral=-8000:pid->integral);
	//使用PID控制 输出 = Kp*当前误差  +  Ki*误差累计值 + Kd*(当前误差-上次误差)
	pid->actual_val = pid->Kp*pid->err + pid->Ki*pid->integral + pid->Kd*(pid->err - pid->err_last);
	//保存上次误差: 这次误差赋值给上次误差
	pid->err_last = pid->err;
	
	return pid->actual_val;//作为输出PWM
}


float mpu6050_pid_control(_pid *pid,float Gyroz)//dt的单位是s
{
    if(myabs(Gyroz)<150)
    {
        Gyroz = 0;
    }
    pid->actual_val += Gyroz;//更新当前角度   ，我在想是不是这里的缩放有问题，导致一直转的角度过大或过小
    pid->err = pid->target_val - pid->actual_val;////当前误差=目标值-真实值
    pid->integral += pid->err;//误差累计值 = 当前误差累计和
    pid->integral=(pid->integral>1000)?pid->integral=1000:((pid->integral<-1000)?pid->integral=-1000:pid->integral);
    //使用PID控制 输出 = Kp*当前误差  +  Ki*误差累计值 + Kd*(当前误差-上次误差)
	pid->actual_val = pid->Kp*pid->err + pid->Ki*pid->integral + pid->Kd*(pid->err - pid->err_last);
	//保存上次误差: 这次误差赋值给上次误差
	pid->err_last = pid->err;
    
    return pid->actual_val;//作为输入环的输入
}