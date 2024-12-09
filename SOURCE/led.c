/*******************************************************
file.name : led.c
version : 1.1
description : 该文件存放了一些用于显示led和led数码管的
相关函数。
*******************************************************/

#include "led.h"
#include <reg51.h>
#include "timer.h"

unsigned int delay_count = 0;
unsigned int delay_flag = 0;

// 宏定义数字管的段码io口和位选io口
#define				GPIO_DIG				P1       // 段码
#define				GPIO_PLACE				P2		 // 位选

// led段码表,存放在rom中
unsigned char code leddata[]={ 
 
                0x12,  //"0"
                0x77,  //"1"
                0x2A,  //"2"
                0x26,  //"3"
                0x47,  //"4"
                0x86,  //"5"
                0x82,  //"6"
                0x37,  //"7"
                0x02,  //"8"
                0x06,  //"9"
                0x03,  //"A"
                0xC2,  //"B"
                0x9A,  //"C"
                0x62,  //"D"
                0x8A,  //"E"
                0x8B,  //"F"
                0x43,  //"H"
                0xDA,  //"L"
                0x13,  //"n"
                0x52,  //"u"
                0x0B,  //"P"
                0xE2,  //"o"
                0xEF,  //"-"
                0xFF,  //熄灭
                0x46   //"y"
                         };

												 
void display_off()
{
	GPIO_PLACE = GPIO_PLACE & 0xF0 + 0x0F;
}

// 只让选中的位数亮，用于让数码管静态显示
// display_screen(input,signature)
// input为需要输入的字符，signature为选位
void display_screen(unsigned char input,unsigned char signature)
{
	unsigned char temp = 0x01;
	// 设置选位
	if(signature>=0&&signature<=3)
		GPIO_PLACE = (GPIO_PLACE & 0xf0) + (~(temp << signature) & 0x0F);  // 保留高四位,变更低四位
	
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
}

// 用于让数码管在xms内动态显示一组数组
void display_str(unsigned char* input,unsigned int xms)
{
	unsigned int i = 0;
	for(i=0;i<8*xms;i++)
	{
		display_str_once(input);
	}
}

// 用于让数码管动态显示一次数组，不占用cpu运行时间
void display_str_once(unsigned char* input)
{
	delay_count++;
	if(delay_count == 10)
	{
		delay_flag++;
		delay_count = 0;
	}
	switch (delay_flag)
	{
	case 0:
		display_screen(input[0],0);
		break;
	case 2:
		display_screen(input[1],1);
		break;
	case 4:
		display_screen(input[2],2);
		break;
	case 6:
		display_screen(input[3],3);
		break;
	case 1:
	case 3:
	case 5:
		display_off();
		break;
	case 7:
		display_off();
		delay_flag = 0;
		break;
	default:
		break;
	}
}

// 用于让数码管动态显示一次数组，且指定其中的一个位数不亮
// 该函数只是用于闪烁
// 不占用cpu运行时间
void blinked_display_str_once(unsigned char* input, unsigned char blinked_bit)
{
	switch (blinked_bit)
	{
	case 1:
		delay_count++;
		if(delay_count == 10)
		{
			delay_flag++;
			delay_count = 0;
		}
		switch (delay_flag)
		{
		case 0:
			display_screen(input[0],0);
			break;
		case 2:
			display_screen(input[1],1);
			break;
		case 4:
			display_screen(input[2],2);
			break;
		case 6:
			display_off();
			break;
		case 1:
		case 3:
		case 5:
			display_off();
			break;
		case 7:
			display_off();
			delay_flag = 0;
			break;
		default:
			break;
		}
		break;
	case 2:
		delay_count++;
		if(delay_count == 10)
		{
			delay_flag++;
			delay_count = 0;
		}
		switch (delay_flag)
		{
		case 0:
			display_screen(input[0],0);
			break;
		case 2:
			display_screen(input[1],1);
			break;
		case 4:
			display_off();
			break;
		case 6:
			display_screen(input[3],3);
			break;
		case 1:
		case 3:
		case 5:
			display_off();
			break;
		case 7:
			display_off();
			delay_flag = 0;
			break;
		default:
			break;
		}
		break;
	case 3:
		delay_count++;
		if(delay_count == 10)
		{
			delay_flag++;
			delay_count = 0;
		}
		switch (delay_flag)
		{
		case 0:
			display_screen(input[0],0);
			break;
		case 2:
			display_off();
			break;
		case 4:
			display_screen(input[2],2);
			break;
		case 6:
			display_screen(input[3],3);
			break;
		case 1:
		case 3:
		case 5:
			display_off();
			break;
		case 7:
			display_off();
			delay_flag = 0;
			break;
		default:
			break;
		}
		break;
	case 4:
		delay_count++;
		if(delay_count == 10)
		{
			delay_flag++;
			delay_count = 0;
		}
		switch (delay_flag)
		{
		case 0:
			display_off();
			break;
		case 2:
			display_screen(input[1],1);
			break;
		case 4:
			display_screen(input[2],2);
			break;
		case 6:
			display_screen(input[3],3);
			break;
		case 1:
		case 3:
		case 5:
			display_off();
			break;
		case 7:
			display_off();
			delay_flag = 0;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}