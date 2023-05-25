#include "menu.h"
#include "key.h"
#include "oled.h"
#include "stm32f10x.h"                  // Device header


void (*current_operation_index)(); //执行当前操作函数
uint8_t func_index = 0;
uint8_t key2;
uint8_t key3;

/*定义了索引数组，索引数组要配合主程序按键值进行选择*/
 Menu_table  table[26]=
{
    {0,0,1,(*show0)},//一级界面
	
    {1,2, 5,(*show1)},//二级界面    第一行
    {2,3, 9,(*show2)},//二级界面    第二行
    {3,4,13,(*show3)},//二级界面   第三行
    {4,1, 0,(*show4)},//三级界面    Back
	
    {5, 6, 4, (*show5)},  //三级菜单 Back
    {6, 7, 5, (*show6)}, //三级菜单 Electric Info
    {7, 8, 5, (*show7)}, //三级菜单 Alarm Info
    {8, 5, 5, (*show8)}, //三级菜单 Vehicle Info

    {9 , 10, 4, (*show9)},    //三级菜单  Back
    {10, 11, 9, (*show10)}, //三级菜单 Electric Info
    {11, 12, 9, (*show11)}, //三级菜单 Alarm Info
    {12, 9 , 9, (*show12)},  //三级菜单 Vehicle Info

    {13, 14, 4, (*show13)},  //三级菜单  Back
    {14, 15, 13, (*show14)}, //三级菜单 Electric Info
    {15, 16, 13, (*show15)}, //三级菜单 Alarm Info
    {16, 13, 13, (*show16)}, 
};


void  Menu_key_set(void)
{
  key2 = KEY2_Get();
  key3 = KEY3_Get();
  if(key2 == 1)
  {   
    func_index=table[func_index].next;//按键next按下后的索引号
    OLED_Clear();
  }
  if(key3 == 1)
  {
    func_index=table[func_index].enter;
    OLED_Clear();
  }

  current_operation_index=table[func_index].current_operation;//执行当前索引号所对应的功能函数。
  (*current_operation_index)();//执行当前操作函数
}

void show0(void)//第一页
{
  OLED_ShowString(0,0,(uint8_t*)"Menu Test",16);
	OLED_ShowString(5,2,(uint8_t*)"TASK 1",16);
	OLED_ShowString(5,4,(uint8_t*)"TASK 2",16);
	OLED_ShowString(5,6,(uint8_t*)"TASK 3",16);
}


void show1(void)//第二页
{
	OLED_ShowString(5,0,(uint8_t*)"TASK 1   <",16);
	OLED_ShowString(5,2,(uint8_t*)"TASK 2",16);
	OLED_ShowString(5,4,(uint8_t*)"TASK 3",16);
	OLED_ShowString(5,6,(uint8_t*)"Back ",16);
}


void show2(void)
{
	OLED_ShowString(5,0,(uint8_t*)"TASK 1",16);
	OLED_ShowString(5,2,(uint8_t*)"TASK 2   <",16);
	OLED_ShowString(5,4,(uint8_t*)"TASK 3",16);
	OLED_ShowString(5,6,(uint8_t*)"Back ",16);   
}

void show3(void)
{
	OLED_ShowString(5,0,(uint8_t*)"TASK 1",16);
	OLED_ShowString(5,2,(uint8_t*)"TASK 2",16);
	OLED_ShowString(5,4,(uint8_t*)"TASK 3   <",16);
	OLED_ShowString(5,6,(uint8_t*)"Back ",16);
}

void show4(void)
{
	OLED_ShowString(5,0,(uint8_t*)"TASK 1",16);
	OLED_ShowString(5,2,(uint8_t*)"TASK 2",16);
	OLED_ShowString(5,4,(uint8_t*)"TASK 3",16);
	OLED_ShowString(5,6,(uint8_t*)"Back     < ",16);  
}

void show5(void)//第二页第一项对应的第三页
{
	OLED_ShowString(5,0,(uint8_t*)"Back     < ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16); 
}

void show6(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back    ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3 < ",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16); 
}

void show7(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back    ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5 < ",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16);
}

void show8(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back    ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0 < ",16);
}

void show9(void)//第二页第二项对应的第三页
{
	OLED_ShowString(5,0,(uint8_t*)"Back     < ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16);
}

void show10(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back",16);
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3 <",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16); 
}

void show11(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back",16);
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5 < ",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16); 
}

void show12(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back",16);
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0 < ",16); 
}

void show13(void)//第二页第三项对应的第三页
{
	OLED_ShowString(5,0,(uint8_t*)"Back     < ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16);
}

void show14(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back    ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3 < ",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16);
}

void show15(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back    ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5 < ",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0",16);
}

void show16(void)
{
	OLED_ShowString(5,0,(uint8_t*)"Back    ",16); 
	OLED_ShowString(5,2,(uint8_t*)"speed 0.3",16);
	OLED_ShowString(5,4,(uint8_t*)"speed 0.5",16);
	OLED_ShowString(5,6,(uint8_t*)"speed 1.0 < ",16);
    
}
