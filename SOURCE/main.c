#include <reg51.h>
#include "led.h"
#include "key.h"
#include "service_fun.h"
#include "interrupt.h"
#include "timer.h"

sbit LED1 = P2^4;
sbit LED2 = P2^5;

void main()
{
	Interrupt_init();	// 中断初始化
	T1_Init();		// 定时器初始化
	display_str("----",2000);  // 初始化提醒
	LED1 = ~LED1;
	LED2 = ~LED2;
	T0_Init();		// 开始计时
	while(1)
	{
		key1_switch_windows();
		display_windows();
	}
}

