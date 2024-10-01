#include <reg51.h>
#include "led.h"
#include "key.h"
#include "service_fun.h"
#include "interrupt.h"

void main()
{
	INT0_init();
	display_str("----",2000);  // 初始化提醒
	while(1)
	{
		display_windows();
	}
}

INT0_service();

