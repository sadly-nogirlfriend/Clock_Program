/*******************************************************
file.name : led.c
version : 1.1
description : 该文件存放了一些用于显示led和led数码管的
相关函数。
*******************************************************/

#include "led.h"
#include <reg51.h>
#include "timer.h"

// 宏定义数字管的段码io口和位选io口
#define				GPIO_DIG				P1       // 段码
#define				GPIO_PLACE				P2		 // 位选

// led段码表,存放在rom中
unsigned char code leddata[]={ 
                0xC0,  //"0"
                0xF9,  //"1"
                0xA4,  //"2"
                0xB0,  //"3"
                0x99,  //"4"
                0x92,  //"5"
                0x82,  //"6"
                0xF8,  //"7"
                0x80,  //"8"
                0x90,  //"9"
                0x88,  //"A"
                0x83,  //"B"
                0xC6,  //"C"
                0xA1,  //"D"
                0x86,  //"E"
                0x8E,  //"F"
                0x89,  //"H"
                0xC7,  //"L"
                0xC8,  //"n"
                0xC1,  //"u"
                0x8C,  //"P"
                0xA3,  //"o"
                0xBF,  //"-"
                0xFF,  //熄灭
                0x91   //"y"
                         };

// 用于让数码管静态显示
// display_screen(input,signature)
// input为需要输入的字符，signature为选位
// 该函数的显示过程已经进行了消影处理
void display_screen(unsigned char input,unsigned char signature)
{
	unsigned char temp = 0x01;
	// 设置选位
	if(signature>=0&&signature<=3)
		GPIO_PLACE = (GPIO_PLACE & 0xf0) + (temp << signature);  // 保留高四位,变更低四位
	
	if((input>='0')&&(input<='9'))
		GPIO_DIG = leddata[input-'0'];
	else if((input>='A')&&(input<='F'))
		GPIO_DIG = leddata[input-'A'+10];
	else if(input=='H')
		GPIO_DIG = leddata[16];
	else if(input=='L')
		GPIO_DIG = leddata[17];
	else if(input=='n')
		GPIO_DIG = leddata[18];
	else if(input=='u')
		GPIO_DIG = leddata[19];
	else if(input=='p')
		GPIO_DIG = leddata[20];
	else if(input=='o')
		GPIO_DIG = leddata[21];
	else if(input=='-')
		GPIO_DIG = leddata[22];
	else if(input=='*') // 表示熄灭
		GPIO_DIG = leddata[23];
	else if(input=='y')
		GPIO_DIG = leddata[24];
	short_delay_5ms();
	
	// 消影
	GPIO_DIG = 0xff;
	short_delay_5ms();
}

// 用于让数码管在xms内动态显示一组数组
void display_str(unsigned char *input,unsigned int xms)
{
	unsigned char i = 0;
	for(i=0;i<xms/40;i++)
	{
		display_screen(input[0],0);
		display_screen(input[1],1);
		display_screen(input[2],2);
		display_screen(input[3],3);
	}
}

// 用于让数码管动态显示一次数组，一次的显示时长约为40ms
void display_str_once(unsigned char* input)
{
//	input[0] = '0' ;
	display_screen(input[0],0);
	display_screen(input[1],1);
	display_screen(input[2],2);
	display_screen(input[3],3);
}