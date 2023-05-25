#ifndef __GRAYSENSOR_H
#define __GRAYSENSOR_H
#include "sys.h"

extern int sensor[5];

extern int Line_Num;

void Get_Light_TTL(void);
void Light_GoStraight_control(void);

void hongwai_Init(void);
#endif
