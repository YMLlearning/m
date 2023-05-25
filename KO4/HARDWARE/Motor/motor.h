#ifndef __MOTOR_H
#define __MOTOR_H


#define PWMA   TIM1->CCR1    //PID�׼���ʹ��һ·���� ʹ�õ�A·����ӦPB0����
#define AIN1   PBout(12)     //PID�׼���ʹ��һ·���� ʹ�õ�A·
#define AIN2   PBout(13)     //PID�׼���ʹ��һ·���� ʹ�õ�A·
#define BIN1   PBout(14)     //Ԥ�� ����
#define BIN2   PBout(15)     //Ԥ�� ����
#define PWMB   TIM1->CCR4    //Ԥ�� ����  ����ӦPB1����
  



void motor_init(void);
void motorPidSetSpeed(float Motor1SetSpeed,float Motor2SetSpeed);	
	
#endif
