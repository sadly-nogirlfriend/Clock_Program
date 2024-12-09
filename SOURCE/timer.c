#include "timer.h"
#include "REG51.H"

void T0_Init()
{
    TMOD = (TMOD & 0xF0) + 0x01;    // 将TMOD后四位初始化，16位计数模式。
    TR0 = 1;
    // 标志位清零
    TF0 = 0;
    // 定时60ms
    TL0 = (65536 - 60000)%256;
    TH0 = (65536 - 60000)/256;
}

/*******************************************************************
注意，定时器T1用于秒表，由开关控制闭合，因此初始化时设定关闭定时器开关TR1
*******************************************************************/
void T1_Init()
{
    TMOD = (TMOD & 0x0F) + 0x10;    // 将TMOD前四位初始化，16位计数模式。
    TR1 = 0;
    // 标志位清零
    TF1 = 0;
    // 定时1ms
    TL1 = (65536 - 1000)%256;
    TH1 = (65536 - 1000)/256;
}

void short_delay_5ms()
{
	unsigned char data i,j;
	for(j=0;j<14;j++)
		{for(i=0;i<50;i++);}
}

// 最小延时时间位50ms，延时毫秒为5ms的整数倍
void Timer_long_delay_Xms(unsigned int xms)
{
    unsigned int i;
    for(i = 0;i < xms/5;i++)
    {
        short_delay_5ms();
    }
}
