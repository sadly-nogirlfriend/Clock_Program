#include <reg51.h>
#include "led.h"
#include "key.h"
#include "service_fun.h"
#include "interrupt.h"
#include "timer.h"
#include "buzzer.h"

#define 	LED_ON 		0
#define 	LED_OFF 	1

sbit LED1 = P2^4;
sbit LED2 = P2^5;

void main()
{
	Interrupt_init();	// 中断初始化
	display_str("----",2000);  // 开机提醒
	LED1 = LED_ON;
	LED2 = LED_ON;
	T1_Init();		// 用作秒表的T1定时器初始化
	T0_Init();		// 用作时钟的T2定时器开始计时
	while(1)
	{
		key_fucntions();
		stopwatch_function();
		display_windows();
		alarm_clock();
	}
}

