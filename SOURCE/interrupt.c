#include "interrupt.h"
#include "REG51.H"
#include "key.h"
#include "service_fun.h"


void INT0_init()
{
    EA = 1;    // 总开关
    EX0 = 1;   // 中断开关
    IT0 = 1;   // 设置中断触发模式
    PX0 = 0;   // 设置优先级
}

void INT0_service() interrupt 0
{
    // 进入中断意味着有按键按下，此时查看按下的按键
    counting_machine();
}