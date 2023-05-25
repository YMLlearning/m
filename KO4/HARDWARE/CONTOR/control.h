#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

/* 编码器物理分辨率 */
#define ENCODER_RESOLUTION                     13

/* 经过倍频之后的总分辨率 */
#define ENCODER_TOTAL_RESOLUTION             (ENCODER_RESOLUTION * 4)  /* 4倍频后的总分辨率 */

/* 减速电机减速比 */
#define REDUCTION_RATIO  30

/*在sysTick里调用PID计算的周期，以毫秒为单位*/
#define SPEED_PID_PERIOD  20    //这个要看定时器7的中断周期
#define TARGET_SPEED_MAX  100  //// 60rpm可以3s走完60cm

#define LUN_JU 17.5   //单位cm

#define PI 3.14159265							//PI圆周率

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
void Xianfu_Pwm(void);  //限制PWM幅值
int myabs(int a);   //取绝对值


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

