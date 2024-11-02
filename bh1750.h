#ifndef __BH1750_H__
#define __BH1750_H__

#include <STC12C5A60S2.h>

// BH1750��ַ
#define BH1750_ADDR 0x46  // ADDR���Žӵ�ʱ�ĵ�ַ

// BH1750ָ��
#define POWER_DOWN    0x00
#define POWER_ON      0x01
#define RESET         0x07
#define CONT_H_RES_MODE  0x10

// ��������
void BH1750_Init(unsigned char bus);
unsigned int BH1750_ReadLight(unsigned char bus);
void BH1750_init();
#endif
