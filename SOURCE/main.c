#include <reg51.h>
#include "led.h"
#include "key.h"
#include "service_fun.h"

void main()
{
	display_str("----",2000);  // 初始化提醒
	while(1)
	{
		counting_machine();
	}
}

