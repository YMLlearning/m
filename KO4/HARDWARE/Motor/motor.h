#ifndef __MOTOR_H
#define __MOTOR_H


#define PWMA   TIM1->CCR1    //PID套件仅使用一路驱动 使用的A路，对应PB0引脚
#define AIN1   PBout(12)     //PID套件仅使用一路驱动 使用的A路
#define AIN2   PBout(13)     //PID套件仅使用一路驱动 使用的A路
#define BIN1   PBout(14)     //预留 备用
#define BIN2   PBout(15)     //预留 备用
#define PWMB   TIM1->CCR4    //预留 备用  ，对应PB1引脚
  



void motor_init(void);
void motorPidSetSpeed(float Motor1SetSpeed,float Motor2SetSpeed);	
	
#endif
