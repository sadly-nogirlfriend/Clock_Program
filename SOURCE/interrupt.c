#include "interrupt.h"
#include "REG51.H"
#include "key.h"
#include "service_fun.h"


void Interrupt_init()
{
/**************************************
 * 中断优先级:INT0>T1>T0
 **************************************/

    EA = 1;    // 总开关
	// INT0中断初始化
    EX0 = 1;   // 中断开关
    IT0 = 1;   // 设置中断触发模式
    PX0 = 1;   // 设置优先级
	// T0定时器中断初始化
	ET0 = 1;
    PT0 = 0;
    // T1定时器中断初始化
    ET1 = 1;
    PT1 = 1;
}


void INT0_service() interrupt 0
{
    // 进入中断意味着有按键按下，此时查看按下的按键
}