#ifndef __PWM_H
#define __PWM_H
#include "sys.h"

void pwm_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);

#endif
