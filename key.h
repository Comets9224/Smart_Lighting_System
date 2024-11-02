#ifndef KEY_H
#define KEY_H

#include <STC12C5A60S2.h>

sbit KEY0 = P0^0;
sbit KEY1 = P0^1;
sbit KEY2 = P0^2;
sbit KEY3 = P0^3;
sbit KEY4 = P0^4;

void Key_Init(void);
char Key_Scan(void);
#endif 
