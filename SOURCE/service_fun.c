/*******************************************************
file.name : service_fun.c
version : 1.1
description : 存放了一些用于程序运行的功能函数
*******************************************************/

// 包含的头文件
#include "service_fun.h"
#include <reg51.h>
#include "key.h"
#include "led.h"

// 宏定义
#define 			WINDOW1				2
#define 			WINDOW2				3
#define 			WINDOW3				4

// 定义的变量
unsigned int count1 = 0;  //用于计数，窗口1
unsigned int count2 = 0;  //用于计数，窗口2
unsigned int count3 = 0;  //用于计数，窗口3
unsigned char windows = WINDOW1;  // 用于窗口标记，默认为窗口1

// 该函数用于数字转字符串,将四位整数转化为字符串
// 注意数字为无符号整数，一定要大于0
unsigned char* num2str(unsigned int num)
{
	unsigned char str[4] = "0000";
	str[3] = num%10+'0';
	str[2] = (num/10)%10+'0';
	str[1] = (num/100)%10+'0';
	str[0] = (num/1000)%10+'0';
	return str;
}

// 该函数用于实现计数器功能。
// key2计数，key1切换窗口，key3减一，key4归零
void counting_machine()
{
	unsigned char key_state;
	// 获取按键状态
	key_state = Key_State_Scan(MODEL1,WAITING_RELEASE);
	if(key_state&1)		//查看key1
	{
		windows++;
		if(windows==WINDOW3+1){windows=WINDOW1;}
	}
	//在每一个周期，根据当前窗口显示对应的计数
	// 注意显示的计数无符号，显示范围为0~9999
	switch(windows)
	{
		case WINDOW1:
			if((key_state>>1)&1)		//查看key2
			{
				count1++;
			}
			if((key_state>>2)&1)		//查看key3
			{
				count1--;
			}
			if((key_state>>3)&1)		//查看key4
			{
				count1 = 0;
			}
			if(count1 > 9999){count1 = 0;}   // 防溢出
			display_str_once(num2str(count1));
			break;
		case WINDOW2:
			if((key_state>>1)&1)
			{
				count2++;
			}
			if((key_state>>2)&1)
			{
				count2--;
			}
			if((key_state>>3)&1)
			{
				count2 = 0;
			}
			if(count2 > 9999){count2 = 0;}   // 防溢出
			display_str_once(num2str(count2));
			break;
		case WINDOW3:
			if((key_state>>1)&1)
			{
				count3++;
			}
			if((key_state>>2)&1)
			{
				count3--;
			}
			if((key_state>>3)&1)
			{
				count3 = 0;
			}
			if(count3 > 9999){count3 = 0;}   // 防溢出
			display_str_once(num2str(count3));
			break;
		default:
			display_str_once("----");
	}
}