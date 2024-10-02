#include "timer.h"
#include "REG51.H"

void T0_Init()
{
    TMOD = 0x01;    // 初始化，16位计数模式。
    TR0 = 1;
}

void short_delay_5ms()
{
	TF0 = 0;                  // 在计数0点将标志位置1
    TL0 = (65536-5000)%256;  // 低八位初始化
    TH0 = (65536-5000)/256;  // 高八位初始化
    while(TF0 == 0);
}

void Timer_delay_50ms()
{
	TF0 = 0;                  // 在计数0点将标志位置1
    TL0 = (65536-50000)%256;  // 低八位初始化
    TH0 = (65536-50000)/256;  // 高八位初始化
    while(TF0 == 0);
}

// 最小延时时间位50ms，延时毫秒为50ms的整数倍
void Timer_long_delay_Xms(unsigned int xms)
{
    unsigned int i;
    for(i = 0;i < xms/50;i++)
    {
        Timer_delay_50ms();
    }
}
