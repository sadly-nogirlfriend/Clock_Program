#include "buzzer.h"
#include <REG51.H>
sbit Buzzer_Port = P2^6;

unsigned char button = 0;
unsigned int count = 0;
unsigned int blinked_count = 0;


void buzzer_beep(unsigned int hz)
{
	count++;
    if(count == hz)
    {
        // 电平反转
        Buzzer_Port = ~Buzzer_Port;
        count = 0;
    }
}

void buzzer_blinked(unsigned int hz)
{
    blinked_count++;
    if(blinked_count == 1876)
    {
        button = ~button;
			blinked_count = 0;
    }
	if(button)
    {
        buzzer_beep(hz);
    }
}

void buzzer_off()
{
	Buzzer_Port = 1;
}