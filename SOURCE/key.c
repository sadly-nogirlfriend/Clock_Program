/*******************************************************
file.name : key.c
version : 1.1
description : 存放了一些用于按键的功能函数
*******************************************************/

#include "key.h"
#include "led.h"
#include <reg51.h>
#include "timer.h"

// 定义按键的状态
#define				KEY1_ON 			0x1   //0001
#define				KEY2_ON 			0x2		//0010
#define				KEY3_ON 			0x4   //0100
#define				KEY4_ON 			0x8		//1000

// 定义按键io口
sbit key1 = P0^0;
sbit key2 = P0^1;
sbit key3 = P0^2;
sbit key4 = P0^3;

// 定义变量
unsigned char key_state = 0x0;
unsigned char mark = 0;

// 扫描指定的io口电平，查看开关的开合
// 输出的是一个四位二进制数，其对应的二进制数就是开关开合的状态
// 从低位到高位是开关1到4
// 按键的扫描有两个模式:
// MODEL1为按钮模式，按下为1，松开为0
// MODEL2为开关模式，按键切换1和0的状态
unsigned char Key_State_Scan(unsigned char model,unsigned char wait_mod)
{
	unsigned char key_state_1 = 0x0;
	if(model == MODEL1)		//模式1，使用局部变量
	{
		if(!key1 || !key2 || !key3 || !key4)  // 检测是否有按键被按下
		{
			if(mark == 0)
			{
				short_delay_5ms();		//消抖
				if(!key1)
				{
					key_state_1 = key_state_1 + KEY1_ON;  //按键赋值
				}
				if(!key2)
				{
					key_state_1 = key_state_1 + KEY2_ON;
				}
				if(!key3)
				{
					key_state_1 = key_state_1 + KEY3_ON;
				}
				if(!key4)
				{
					key_state_1 = key_state_1 + KEY4_ON;
				}
				mark = 1;
			}
		}
		else
		{
			mark = 0;
		}
		key_state = key_state_1;
	}
	if(model == MODEL2)		//模式2，使用静态变量
	{
		if(!key1 || !key2 || !key3 || !key4)  // 检测是否有按键被按下
		{
			if(mark == 0)
			{
				short_delay_5ms();		//消抖
				if(!key1)
				{
					key_state = key_state ^ (1 << 0);  //按键赋值，按位取反
				}
				if(!key2)
				{
					key_state = key_state ^ (1 << 1);
				}
				if(!key3)
				{
					key_state = key_state ^ (1 << 2);
				}
				if(!key4)
				{
					key_state = key_state ^ (1 << 3);
				}
			}
		}
		else
		{
			mark = 1;
		}
	}
	return key_state;
}