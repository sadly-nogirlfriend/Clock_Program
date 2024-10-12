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
#define  			CLOCK_MOD			5

// 定义的变量
unsigned int mins = 0;
unsigned int hours = 0;
unsigned int day = 1;
unsigned int mounth = 1;
unsigned int year = 2000; 
int min_time = 0; 

unsigned char window1_num[] = "0000";  //用于计数，窗口1
unsigned char window2_num[] = "0000";  //用于计数，窗口2
unsigned char window3_num[] = "0000";  //用于计数，窗口3
unsigned char windows = WINDOW1;  // 用于窗口标记，默认为窗口1

// 该函数用于数字转字符串,将四位整数转化为字符串
// 注意数字为无符号整数，一定要大于0
void num2str(unsigned int num,unsigned char* str)
{
	str[3] = num%10+'0';
	str[2] = (num/10)%10+'0';
	str[1] = (num/100)%10+'0';
	str[0] = (num/1000)%10+'0';
}

// 该函数用于显示对应窗口
void display_windows()
{
	switch (windows)
	{
	case WINDOW1:
		display_str_once(window1_num);
		break;
	case WINDOW2:
		display_str_once(window2_num);
		break;
	case WINDOW3:
		display_str_once(window3_num);
		break;
	default:
		display_str_once("----");
		break;
	}
}

// 窗口分配：window1 用于时钟
void clock() interrupt 1
{
	// 标志位清零
	TF0 = 0;
	// min_time为60ms
	TL0 = (65536 - 60000)%256;
	TH0 = (65536 - 60000)/256;
	min_time++;
	time_carry();  // 日历进位操作
	num2str(mins+hours*100,window1_num);
	num2str(day+mounth*100,window1_num);
	num2str(year,window1_num);
}

void time_carry()
{
	if(min_time == 1000)
	{
		mins++;
		min_time = 0;
	}
	if(mins == 60)   //60分钟进1小时
	{
		hours++;
		mins = 0;
	}
	if(hours == 24)	 //24小时进1天
	{
		day++;
		hours = 0;
	}

	// 年月日进位
    if((years%4 == 0)&&(years%100 != 0)||(years%400 == 0))   // 闰年
    {
        switch (mounth)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if(day > 31)
            {
                years++;
                mounth = 1;
                day = 1;
            }
            break;
        case 2:
            if(day > 29)
            {
                mounth++;
                day = 1;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(day > 30)
            {
                mounth++;
                day = 1;
            }
            break;
        default:
            break;
        }
    }
    else
	{
		switch (mounth)
		{
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			if(day == 31)
			{
				mounth++;
				day = 0;
			}
			break;
		case 2:
			if(day == 28)
			{
				mounth++;
				day = 0;
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			if(day == 30)
			{
				mounth++;
				day = 0;
			}
			break;
		default:
			break;
		}
	}
}