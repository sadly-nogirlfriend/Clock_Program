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
	// 关闭INT1
		EX1 = 0;
	// T0定时器中断初始化
	ET0 = 1;
    PT0 = 0;
    // T1定时器中断初始化
    ET1 = 1;
    PT1 = 1;
}
