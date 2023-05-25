#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

/* ����������ֱ��� */
#define ENCODER_RESOLUTION                     13

/* ������Ƶ֮����ֱܷ��� */
#define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4��Ƶ����ֱܷ��� */

/* ���ٵ�����ٱ� */
#define REDUCTION_RATIO  30

/*��sysTick�����PID��������ڣ��Ժ���Ϊ��λ*/
#define SPEED_PID_PERIOD  20    //���Ҫ����ʱ��7���ж�����
#define TARGET_SPEED_MAX  100  //// 60rpm����3s����60cm

#define LUN_JU 17.5   //��λcm

#define PI 3.14159265							//PIԲ����

typedef enum
{
    left_90,
	right_90,
	back_180,
    left_45,
    right_45,
    right_10,
    right_20
}spin_dir_t;


void Set_Pwm(int moto1,int moto2);
void Xianfu_Pwm(void);  //����PWM��ֵ
int myabs(int a);   //ȡ����ֵ


float speed_pid_control(void);
float location_pid_control(void);
float speed2_pid_control(void);
float location2_pid_control(void);
void Location_Speed_control(void);


void Car_go(int32_t location_cm);
void spin_Turn(spin_dir_t zhuanxiang);
void Xunji_speed_control(void);
void mpu6050_control(void);
void Get_Angle(u8 way);
void EXTI15_10_IRQHandler(void);
#endif

