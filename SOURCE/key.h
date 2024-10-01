#ifndef __KEY_H
#define __KEY_H

// 按键检测模式
#define				MODEL1 			1
#define				MODEL2 			2
#define				WAITING_RELEASE     3
unsigned char Key_State_Scan(unsigned char model,unsigned char wait_mod);

#endif