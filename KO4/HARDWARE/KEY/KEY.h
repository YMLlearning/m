#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"  	 


#define KEY1 PCin(14) 
#define KEY2 PCin(15) 
#define KEY3 PBin(5) 
 

#define KEY1_PRES	1		//KEY0  
#define KEY2_PRES	2		//KEY1 
#define KEY3_PRES	3		//KEY1 
void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8 mode);  	//����ɨ�躯��					    
#endif
