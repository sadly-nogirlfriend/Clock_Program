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
	INT0_init();	// 中断初始化
	T0_Init();		// 定时器初始化
	display_str("----",2000);  // 初始化提醒
	LED1 = ~LED1;
	LED2 = ~LED2;
	while(1)
	{
		
	}
}

// INT0_service();

