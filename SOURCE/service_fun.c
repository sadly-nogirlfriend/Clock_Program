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
#define				STOPWATCH_WINDOW	5
#define  			CLOCK_MOD			6
#define 			STOPWATCH_MOD		7

// 时间变量区
unsigned int second = 0;
unsigned int mins = 21;
unsigned int hours = 11;
unsigned int day = 14;
unsigned int mounth = 10;
unsigned int year = 2024;
int min_time = 0; 
unsigned int stopwatch_sec = 0;  // 秒表计数
unsigned int stopwatch_10ms = 0;

unsigned char window1_num[] = "0000";  // 用于计数，窗口1
unsigned char window2_num[] = "0000";  // 用于计数，窗口2
unsigned char window3_num[] = "0000";  // 用于计数，窗口3
unsigned char stopwatch_window_num[] = "0000";	// 用于计数，秒表窗口

// 标记变量区
unsigned char windows = WINDOW1;  	   // 窗口标记，默认为窗口1
unsigned char Mod = CLOCK_MOD;	  	   // 模式标记，默认为时钟模式
unsigned char stopwatch_state = 0;     // 秒表开关标记
// 该函数用于日历的进位计算
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
    if((year%4 == 0)&&(year%100 != 0)||(year%400 == 0))   // 闰年
    {
        switch (mounth)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
		    if(day > 31)
            {
                mounth++;
                day = 1;
            }
            break;
        case 12:
            if(day > 31)
            {
                year++;
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
		    if(day > 31)
            {
                mounth++;
                day = 1;
            }
            break;
        case 12:
            if(day > 31)
            {
                year++;
                mounth = 1;
                day = 1;
            }
            break;
        case 2:
            if(day > 28)
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
}

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
	switch (Mod)
	{
	case CLOCK_MOD:
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
		break;
	case STOPWATCH_MOD:
		display_str_once(stopwatch_window_num);
		break;
	default:
		break;
	}
}

void stopwatch_function()
{
    if(stopwatch_state)
    {
        TR1 = 1;
    }
    else
    {
        TR1 = 0;
    }
}

void key_fucntions()
{
    unsigned char key_state = 0x0;      // 用于储存按键状态
    key_state = Key_State_Scan(MODEL1);
	// 当模式为时钟模式的时候，按键1的作用是切换窗口，分别显示年月时分
	if(((key_state>>0) & 1 )&&(Mod == CLOCK_MOD))
	{
		windows++;
		if(windows == WINDOW3 + 1)
		{
			windows = WINDOW1;
		}
	}
	// 当模式为秒表模式的时候，按键1的作用是开始和停止秒表
	if(((key_state>>0) & 1 )&&(Mod == STOPWATCH_MOD))
	{
		stopwatch_state = ~stopwatch_state;
	}
    // 当按键2被按下的时候，清零秒表
    if((key_state>>1) & 1)
    {
				stopwatch_state = 0;
        // 关闭T1定时器
        TR1 = 0;
        // 标志位清零
        TF1 = 0;
        // 重置TL1,TH1
        TL1 = (65536 - 10000)%256;
        TH1 = (65536 - 10000)/256;
        // 归零秒表计数
        stopwatch_sec = 0;
        stopwatch_10ms = 0;
        stopwatch_window_num[0] = '0';
        stopwatch_window_num[1] = '0';
        stopwatch_window_num[2] = '0';
        stopwatch_window_num[3] = '0';
    }
    // 当按键3被按下的时候，切换当前模式
    if((key_state>>2) & 1)
	{
		Mod++;
		if(Mod == STOPWATCH_MOD+1)
		{
			Mod = CLOCK_MOD;
		}
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
	num2str(day+mounth*100,window2_num);
	num2str(year,window3_num);
}

void stopwatch() interrupt 3
{
    // 标志位清零
    TF1 = 0;
    // 定时10ms(一般秒表显示的最小单位就为10ms)
    TL1 = (65536 - 10000)%256;
    TH1 = (65536 - 10000)/256;
    stopwatch_10ms++;
    // 进位
    if(stopwatch_10ms == 100)
    {
        stopwatch_sec++;
        stopwatch_10ms = 0;
    }
    if(stopwatch_sec == 61)
    {
        stopwatch_sec = 0; // 设定最长秒表计时60s
    }

    /*********************************************************
     * 注意，因为两个中断重用了同一个函数，如果在其中一个中断函数执
     * 行期间，另一个中断打断了当前终端，那么重用的函数传入的参数将
     * 会被覆写，可能造成一定问题。在这里，我们使用OVERLAY进行解决，
     * 在配置文件中为该函数添加单独的内存空间，将两个中断函数排除在
     * 重叠分析之外，防止中断间的串扰。具体用法参考OVERLAY文档。
    *********************************************************/
    num2str(stopwatch_10ms+stopwatch_sec*100,stopwatch_window_num);
}


