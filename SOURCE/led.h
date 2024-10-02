#ifndef __LED_H
#define __LED_H

// 宏定义段码，用于快捷调试程序
#define       ZERO             0xC0    
#define       ONE              0xF9    
#define       TWO              0xA4    
#define       THREE        	   0xB0    
#define       FOUR      	   0x99    
#define       FIVE             0x92    
#define       SIX              0x82    
#define       SEVEN            0xF8    
#define       EIGHT            0x80    
#define       NIGHT            0x90    
#define       LETTER_A         0x88    
#define       LETTER_B         0x83    
#define       LETTER_C         0xC6    
#define       LETTER_D         0xA1     
#define       LETTER_E         0x86     
#define       LETTER_F         0x8E     
#define       LETTER_H         0x89     
#define       LETTER_L         0xC7     
#define       LETTER_n         0xC8     
#define       LETTER_u         0xC1     
#define       LETTER_P         0x8C     
#define       LETTER_o         0xA3     
#define       LETTER_-         0xBF     
#define       TURN_OFF         0xFF    
#define       CUSTOM           0xFF    

// 宏定义数字管的段码io口和位选io口
#define				GPIO_DIG				P1       // 段码
#define				GPIO_PLACE				P2		 // 位选

void display_screen(unsigned char input,unsigned char signature);
void display_str(unsigned char *input,unsigned int xms);
void display_str_once(unsigned char *input);
	
#endif