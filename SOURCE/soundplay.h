#ifndef __SOUNDPLAY_H
#define __SOUNDPLAY_H_

void InitialSound(void);
void BeepTimer0(void);
void Play(unsigned char *Sound,unsigned char Signature,unsigned Octachord,unsigned int Speed);

#endif