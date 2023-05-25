#include "menu.h"
#include "key.h"
#include "oled.h"
#include "stm32f10x.h"                  // Device header


void (*current_operation_index)(); //ִ�е�ǰ��������
uint8_t func_index = 0;
uint8_t key2;
uint8_t key3;

/*�������������飬��������Ҫ��������򰴼�ֵ����ѡ��*/
 Menu_table  table[26]=
{
    {0,0,1,(*show0)},//һ������
	
    {1,2, 5,(*show1)},//��������    ��һ��
    {2,3, 9,(*show2)},//��������    �ڶ���
    {3,4,13,(*show3)},//��������   ������
    {4,1, 0,(*show4)},//��������    Back
	
    {5, 6, 4, (*show5)},  //�����˵� Back
    {6, 7, 5, (*show6)}, //�����˵� Electric Info
    {7, 8, 5, (*show7)}, //�����˵� Alarm Info
    {8, 5, 5, (*show8)}, //�����˵� Vehicle Info

    {9 , 10, 4, (*show9)},    //�����˵�  Back
    {10, 11, 9, (*show10)}, //�����˵� Electric Info
    {11, 12, 9, (*show11)}, //�����˵� Alarm Info
    {12, 9 , 9, (*show12)},  //�����˵� Vehicle Info

    {13, 14, 4, (*show13)},  //�����˵�  Back
    {14, 15, 13, (*show14)}, //�����˵� Electric Info
    {15, 16, 13, (*show15)}, //�����˵� Alarm Info
    {16, 13, 13, (*show16)}, 
};


void  Menu_key_set(void)
{
  key2 = KEY2_Get();
  key3 = KEY3_Get();
  if(key2 == 1)
  {   
    func_index=table[func_index].next;//����next���º��������
    OLED_Clear();
  }
  if(key3 == 1)
  {
    func_index=table[func_index].enter;
    OLED_Clear();
  }

  current_operation_index=table[func_index].current_operation;//ִ�е�ǰ����������Ӧ�Ĺ��ܺ�����
  (*current_operation_index)();//ִ�е�ǰ��������
}

void show0(void)//��һҳ
{
  OLED_ShowString(0,0,(uint8_t*)"Menu Test",16);
	OLED_ShowString(5,2,(uint8_t*)"TASK 1",16);
	OLED_ShowString(5,4,(uint8_t*)"TASK 2",16);
	OLED_ShowString(5,6,(uint8_t*)"TASK 3",16);
}


void show1(void)//�ڶ�ҳ
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

void show5(void)//�ڶ�ҳ��һ���Ӧ�ĵ���ҳ
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

void show9(void)//�ڶ�ҳ�ڶ����Ӧ�ĵ���ҳ
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

void show13(void)//�ڶ�ҳ�������Ӧ�ĵ���ҳ
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
