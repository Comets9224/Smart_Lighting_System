#ifndef __I2C_H__
#define __I2C_H__

#include <STC12C5A60S2.h>

// 定义4组I2C引脚
sbit SCL1 = P1^0;
sbit SDA1 = P1^1;
sbit SCL2 = P1^2;
sbit SDA2 = P1^5;
sbit SCL3 = P1^6;
sbit SDA3 = P1^7;
sbit SCL4 = P2^1;
sbit SDA4 = P2^2;

// 函数声明
void I2C_Init(unsigned char bus);
void I2C_Start(unsigned char bus);
void I2C_Stop(unsigned char bus);
void I2C_SendAck(unsigned char bus, bit ack);
bit I2C_RecvAck(unsigned char bus);
void I2C_SendByte(unsigned char bus, unsigned char dat);
unsigned char I2C_RecvByte(unsigned char bus);

#endif
