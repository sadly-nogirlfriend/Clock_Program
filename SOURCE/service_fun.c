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
#include "buzzer.h"

// 宏定义
#define 			WINDOW1				      2       // 分秒窗口
#define 			WINDOW2				      3       // 月日窗口
#define 			WINDOW3				      4       // 年份窗口
#define				STOPWATCH_WINDOW	  5       // 秒表窗口
#define  			CLOCK_MOD			      6
#define 			STOPWATCH_MOD		    7
#define       ALARM_SET_MOD       8
#define       CLOCK_FIXED_MOD     9

#define 	LED_ON 		0
#define 	LED_OFF 	1

sbit LED1 = P2^4;
sbit LED2 = P2^5;

// 时间变量区
unsigned int mins = 34;
unsigned int hours = 12;
unsigned int day = 7;
unsigned int mounth = 11;
unsigned int year = 2024;
int min_time = 0; 
unsigned int stopwatch_sec = 0;  // 秒表计数
unsigned int stopwatch_10ms = 0; // 秒表计毫秒
unsigned int alarm_mins = 35;     // 闹钟分
unsigned int alarm_hours = 12;    // 闹钟秒

// 窗口字符变量区
unsigned char window1_num[] = "0000";  // 用于计数，窗口1
unsigned char window2_num[] = "0000";  // 用于计数，窗口2
unsigned char window3_num[] = "0000";  // 用于计数，窗口3
unsigned char stopwatch_window_num[] = "0000";	// 用于计数，秒表窗口
unsigned char alarm_clock_window_num[] = "1235";

// 标记变量区
unsigned char windows = WINDOW1;  	        // 窗口标记，默认为窗口1
unsigned char Mod = CLOCK_MOD;	  	        // 模式标记，默认为时钟模式
unsigned char stopwatch_state = 0;          // 秒表开关标记
unsigned char blinked_bit = 1;              // 屏幕闪烁位数标记
unsigned char alarm_set_bit = 1;            // 闹钟修正闪烁位数标记
unsigned int blinked_mark = 0;              // 屏幕闪烁定时标记
unsigned char blinked_screen_State = ~0;    // 屏幕闪烁状态标记
unsigned char alarm_clock_switch = 0;       // 闹钟开关
unsigned char alarm_mod_buffer = 0;         // 闹钟缓冲暂存变量
unsigned char clock_fixed_buffer = 0;       // 时钟矫正缓冲暂存变量
unsigned char alarm_stop_buzzer = 0x0;			// 用于关闭正在响的闹钟


// 该函数用于日历的进位计算
void time_carry()
{
	if(min_time == 1000)
	{
		mins++;
		min_time = 0;
	}
	if(mins >= 60)   //60分钟进1小时
	{
		hours++;
		mins = mins-60;
	}
	if(hours >= 24)	 //24小时进1天
	{
		day++;
		hours = hours - 24;
	}
    if(year >= 10000) //不超过一万年
    {
        year = year - 10000;
    }
	if(mounth > 12)
    {
        mounth = 1;
    }
    // 日进位
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
                day = day-31;
            }
            break;
        case 12:
            if(day > 31)
            {
                year++;
                mounth = 1;
                day = day - 31;
            }
            break;
        case 2:
            if(day > 29)
            {
                mounth++;
                day = day - 29;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(day > 30)
            {
                mounth++;
                day = day - 30;
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
                day = day - 31;
            }
            break;
        case 12:
            if(day > 31)
            {
                year++;
                mounth = 1;
                day = day - 31;
            }
            break;
        case 2:
            if(day > 28)
            {
                mounth++;
                day = day - 28;
            }
            break;
        case 4:
        case 6:
        case 9:
        case 11:
            if(day > 30)
            {
                mounth++;
                day = day - 30;
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
    // 屏幕闪烁处理
    blinked_mark++;
    if(blinked_mark == 1500)
    {
        blinked_screen_State = ~blinked_screen_State;
        blinked_mark = 0;
    }
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
    case ALARM_SET_MOD:
        if(blinked_screen_State)
        {
            display_str_once(alarm_clock_window_num);
            if(alarm_clock_switch)
            {
              LED1 = LED_ON;
              LED2 = LED_ON;
            }
            else
            {
              LED1 = LED_OFF;
              LED2 = LED_OFF;
            }
        }
        else
        {
            blinked_display_str_once(alarm_clock_window_num,alarm_set_bit);
            LED1 = LED_OFF;
            LED2 = LED_OFF;
        }
        break;
    case CLOCK_FIXED_MOD:
        // 屏幕闪烁显示
        switch (windows)
        {
        case WINDOW1:
            if(blinked_screen_State)
            {
                display_str_once(window1_num);
            }
            else
            {
                blinked_display_str_once(window1_num,blinked_bit);
            }
            break;
        case WINDOW2:
            if(blinked_screen_State)
            {
                display_str_once(window2_num);
            }
            else
            {
                blinked_display_str_once(window2_num,blinked_bit);
            }
            break;
        case WINDOW3:
            if(blinked_screen_State)
            {
                display_str_once(window3_num);
            }
            else
            {
                blinked_display_str_once(window3_num,blinked_bit);
            }
            break;
        
        default:
            break;
        }
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
	
	// 如果闹钟正在响，按键1可以关闭闹钟
	if(((key_state>>0) & 1) && ((mins == alarm_mins) && (hours == alarm_hours) && (alarm_clock_switch)))
	{
		alarm_stop_buzzer = 0xFF;
	}
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

	// 当模式为闹钟设置模式的时候，按键1的作用是设置闹钟的开关
	if(((key_state>>0) & 1 )&&(Mod == ALARM_SET_MOD))
  {
    alarm_clock_switch = ~alarm_clock_switch;
  }

    // 当模式为秒表模式时，按下按键2，清零秒表
  if(((key_state>>1) & 1 )&&(Mod == STOPWATCH_MOD))
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
    
    // 当模式为闹钟设置模式时，按下按键2，切换选位
  if(((key_state>>1) & 1 )&&(Mod == ALARM_SET_MOD))
    {
      alarm_set_bit++;
      if(alarm_set_bit >= 5)
      {
        alarm_set_bit = 1;
      }
    }

    // 当按键3被按下的时候，切换当前模式
  if(((key_state>>2) & 1 )&&(Mod != ALARM_SET_MOD))
	{
		Mod++;
		if(Mod >= STOPWATCH_MOD+1)
		{
			Mod = CLOCK_MOD;
		}
	}
    
    // 当处在闹钟设置模式的时候，按下按键3，当前位加1
  if(((key_state>>2) & 1 )&&(Mod == ALARM_SET_MOD))
  {
    switch (alarm_set_bit)
    {
    case 1:
      if(alarm_mins%10 != 9)
        alarm_mins++;
      else
        alarm_mins = alarm_mins - 9;
      break;
    case 2:
      if(alarm_mins/10 < 6)
        alarm_mins = alarm_mins + 10;
      else
        alarm_mins = alarm_mins - 50;
      break;
    case 3:
      if(alarm_hours+1 >= 24)
        alarm_hours = 0;
      else
        alarm_hours = alarm_hours + 1;
      break;
    case 4:
      if(alarm_hours+10 >= 24)
        alarm_hours = alarm_hours % 10;
      else
        alarm_hours = alarm_hours + 10;
      break;
    default:
      break;
    }
    num2str(alarm_hours*100+alarm_mins,alarm_clock_window_num);
  }
    
    // 当处在闹钟设置模式的时候，按下按键4，当前位减1
  if(((key_state>>3) & 1 )&&(Mod == ALARM_SET_MOD))
  {
    switch (alarm_set_bit)
    {
    case 1:
      if(alarm_mins%10 != 0)
        alarm_mins = alarm_mins - 1;
      break;
    case 2:
      if(!(alarm_mins < 10))
        alarm_mins = alarm_mins - 10;
      break;
    case 3:
      if(!(alarm_hours < 1))
        alarm_hours = alarm_hours - 1;
      break;
    case 4:
      if(!(alarm_hours < 10))
        alarm_hours = alarm_hours - 10; 
      break;
    default:
      break;
    }
    num2str(alarm_hours*100+alarm_mins,alarm_clock_window_num);
  }
    
    // 当按键5被按下的时候，打开中断开关
  if((key_state>>4) & 1)
    {
      EX0 = 1;
    }

  // 当按键6被按下的时候，切换到闹钟设置窗口
  if((key_state>>5) & 1)
    {
        if(alarm_mod_buffer == 0)
        {
            alarm_mod_buffer = Mod;
            Mod = ALARM_SET_MOD;
        }
        else
        {
            Mod = alarm_mod_buffer;
            alarm_mod_buffer = 0;
            LED1 = LED_ON;
            LED2 = LED_ON;
        }
    }
}

void alarm_clock()
{
  if((mins == alarm_mins) && (hours == alarm_hours) && (alarm_clock_switch))
  {
		if(alarm_stop_buzzer)
		{
			buzzer_off();
		}
    else
		{
			buzzer_blinked(3);
		}
  }
	else
	{
		alarm_stop_buzzer = 0x0;
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

// 按下按键5进入中断,开始校正时钟
void clock_fixed() interrupt 0
{
  unsigned char fixed__state = 1;
  extern unsigned char mark;
  mark = 1;
  blinked_bit = 1;
  windows = WINDOW1;
  clock_fixed_buffer = Mod;
  Mod = CLOCK_FIXED_MOD;
  // 关闭中断开关，防止二次进入中断
  EX0 = 0;
  while(fixed__state)
  {
    unsigned char fixed_key_state = 0x0;
    // 屏幕闪烁显示
    display_windows();
    // 进行一次按键扫描
    fixed_key_state = Key_State_Scan(MODEL1);
    // 按键1切换时间窗口
    if((fixed_key_state >> 0) & 1)
    {
        windows++;
        if(windows == WINDOW3 + 1)
        {
            windows = WINDOW1;
        }
    }
    // 按键2切换位数
    if((fixed_key_state >> 1) & 1)
    {
        blinked_bit++;
        if(blinked_bit == 5)
        {
            blinked_bit = 1;
        }
    }
    // 按键3加
    if((fixed_key_state >> 2) & 1)
    {
        if(windows == WINDOW3)
        {
            switch (blinked_bit)
            {
            case 1:
                year++;
                break;
            case 2:
                year = year + 10;
                break;
            case 3:
                year = year + 100;
                break;
            case 4:
                year = year + 1000;
                break;                                        
            default:
                break;
            }

        }
        else if(windows == WINDOW2)
        {
            switch (blinked_bit)
            {
            case 1:
                day++;
                break;
            case 2:
                day = day + 10;
                break;
            case 3:
                mounth++;
                break;
            case 4:
                mounth = mounth + 10;
                break;                                        
            default:
                break;
            }
        }
        else if (windows == WINDOW1)
        {
            switch (blinked_bit)
            {
            case 1:
                mins++;
                break;
            case 2:
                mins = mins + 10;
                break;
            case 3:
                hours++;
                break;
            case 4:
                hours = hours + 10;
                break;                                        
            default:
                break;
            }
        }
        time_carry();
        num2str(mins+hours*100,window1_num);
        num2str(day+mounth*100,window2_num);
        num2str(year,window3_num);
    }
    // 按键4减
    if((fixed_key_state >> 3) & 1)
    {
        if(windows == WINDOW3)
        {
            switch (blinked_bit)
            {
            case 1:
                if(!(year<1))
                    year--;
                else
                    year = 0;
                break;
            case 2:
                if(!(year<10))
                    year = year - 10;
                else
                    year = 0;
                break;
            case 3:
                if(!(year<100))
                    year = year - 100;
                else
                    year = 0;
                break;
            case 4:
                if(!(year<1000))
                    year = year - 1000;
                else
                    year = 0;
                break;                                        
            default:
                break;
            }

        }
        else if(windows == WINDOW2)
        {
            switch (blinked_bit)
            {
            case 1:
                if(!(day<1))
                    day--;
                else
                    day = 0;
                break;
            case 2:
                if(!(day<10))
                    day = day - 10;
                else
                    day = 0;                     
                break;
            case 3:
                if(!(mounth<1))
                    mounth--;
                else
                    mounth = 0;
                break;
            case 4:
                if(!(mounth<10))
                    mounth = mounth - 10;
                else
                    mounth = 0;
                break;                                        
            default:
                break;
            }
        }
        else if (windows == WINDOW1)
        {
            switch (blinked_bit)
            {
            case 1:
                if(!(mins<1))
                    mins--;
                else
                    mins = 0;
                break;
            case 2:
                if(!(mins<10))
                    mins = mins - 10;
                else
                    mins = 0;                        
                break;
            case 3:
                if(!(hours<1))
                    hours--;
                else
                    hours = 0;
                break;
            case 4:
                if(!(hours<10))
                    hours = hours - 10;
                else
                    hours = 0;
                break;                                        
            default:
                break;
            }
        }
        time_carry();
        num2str(mins+hours*100,window1_num);
        num2str(day+mounth*100,window2_num);
        num2str(year,window3_num);
    }

    // 再次按下按键5退出循环
    if((fixed_key_state >> 4) & 1)
    {
        fixed__state = 0;
        Mod = clock_fixed_buffer;
        clock_fixed_buffer = 0;
    }
  }
}



