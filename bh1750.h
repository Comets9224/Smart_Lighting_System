#ifndef __BH1750_H__
#define __BH1750_H__

#include <STC12C5A60S2.h>

// BH1750地址
#define BH1750_ADDR 0x46  // ADDR引脚接地时的地址

// BH1750指令
#define POWER_DOWN    0x00
#define POWER_ON      0x01
#define RESET         0x07
#define CONT_H_RES_MODE  0x10

// 函数声明
void BH1750_Init(unsigned char bus);
unsigned int BH1750_ReadLight(unsigned char bus);
void BH1750_init();
#endif
