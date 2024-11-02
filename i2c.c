#include "i2c.h"

// I2C延时函数
void I2C_Delay(void)
{
    unsigned char i = 10;
    while (i--);
}

// 初始化I2C
void I2C_Init(unsigned char bus)
{
    switch(bus) {
        case 1: SCL1 = 1; SDA1 = 1; break;
        case 2: SCL2 = 1; SDA2 = 1; break;
        case 3: SCL3 = 1; SDA3 = 1; break;
        case 4: SCL4 = 1; SDA4 = 1; break;
    }
}

// 产生I2C起始信号
void I2C_Start(unsigned char bus)
{
    switch(bus) {
        case 1: SDA1 = 1; SCL1 = 1; I2C_Delay(); SDA1 = 0; I2C_Delay(); SCL1 = 0; break;
        case 2: SDA2 = 1; SCL2 = 1; I2C_Delay(); SDA2 = 0; I2C_Delay(); SCL2 = 0; break;
        case 3: SDA3 = 1; SCL3 = 1; I2C_Delay(); SDA3 = 0; I2C_Delay(); SCL3 = 0; break;
        case 4: SDA4 = 1; SCL4 = 1; I2C_Delay(); SDA4 = 0; I2C_Delay(); SCL4 = 0; break;
    }
}

// 产生I2C停止信号
void I2C_Stop(unsigned char bus)
{
    switch(bus) {
        case 1: SDA1 = 0; SCL1 = 1; I2C_Delay(); SDA1 = 1; I2C_Delay(); break;
        case 2: SDA2 = 0; SCL2 = 1; I2C_Delay(); SDA2 = 1; I2C_Delay(); break;
        case 3: SDA3 = 0; SCL3 = 1; I2C_Delay(); SDA3 = 1; I2C_Delay(); break;
        case 4: SDA4 = 0; SCL4 = 1; I2C_Delay(); SDA4 = 1; I2C_Delay(); break;
    }
}

// 发送应答信号
void I2C_SendAck(unsigned char bus, bit ack)
{
    switch(bus) {
        case 1: SDA1 = ack; SCL1 = 1; I2C_Delay(); SCL1 = 0; I2C_Delay(); break;
        case 2: SDA2 = ack; SCL2 = 1; I2C_Delay(); SCL2 = 0; I2C_Delay(); break;
        case 3: SDA3 = ack; SCL3 = 1; I2C_Delay(); SCL3 = 0; I2C_Delay(); break;
        case 4: SDA4 = ack; SCL4 = 1; I2C_Delay(); SCL4 = 0; I2C_Delay(); break;
    }
}

// 接收应答信号
bit I2C_RecvAck(unsigned char bus)
{
    bit ack;
    switch(bus) {
        case 1: SDA1 = 1; I2C_Delay(); SCL1 = 1; I2C_Delay(); ack = SDA1; SCL1 = 0; I2C_Delay(); break;
        case 2: SDA2 = 1; I2C_Delay(); SCL2 = 1; I2C_Delay(); ack = SDA2; SCL2 = 0; I2C_Delay(); break;
        case 3: SDA3 = 1; I2C_Delay(); SCL3 = 1; I2C_Delay(); ack = SDA3; SCL3 = 0; I2C_Delay(); break;
        case 4: SDA4 = 1; I2C_Delay(); SCL4 = 1; I2C_Delay(); ack = SDA4; SCL4 = 0; I2C_Delay(); break;
    }
    return ack;
}

// 发送一个字节
void I2C_SendByte(unsigned char bus, unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++) {
        switch(bus) {
            case 1: SDA1 = (dat & 0x80) ? 1 : 0; break;
            case 2: SDA2 = (dat & 0x80) ? 1 : 0; break;
            case 3: SDA3 = (dat & 0x80) ? 1 : 0; break;
            case 4: SDA4 = (dat & 0x80) ? 1 : 0; break;
        }
        dat <<= 1;
        switch(bus) {
            case 1: SCL1 = 1; I2C_Delay(); SCL1 = 0; I2C_Delay(); break;
            case 2: SCL2 = 1; I2C_Delay(); SCL2 = 0; I2C_Delay(); break;
            case 3: SCL3 = 1; I2C_Delay(); SCL3 = 0; I2C_Delay(); break;
            case 4: SCL4 = 1; I2C_Delay(); SCL4 = 0; I2C_Delay(); break;
        }
    }
    I2C_RecvAck(bus);
}

// 接收一个字节
unsigned char I2C_RecvByte(unsigned char bus)
{
    unsigned char i, dat = 0;
    switch(bus) {
        case 1: SDA1 = 1; break;
        case 2: SDA2 = 1; break;
        case 3: SDA3 = 1; break;
        case 4: SDA4 = 1; break;
    }
    for (i = 0; i < 8; i++) {
        dat <<= 1;
        switch(bus) {
            case 1: SCL1 = 1; I2C_Delay(); if (SDA1) dat |= 0x01; SCL1 = 0; I2C_Delay(); break;
            case 2: SCL2 = 1; I2C_Delay(); if (SDA2) dat |= 0x01; SCL2 = 0; I2C_Delay(); break;
            case 3: SCL3 = 1; I2C_Delay(); if (SDA3) dat |= 0x01; SCL3 = 0; I2C_Delay(); break;
            case 4: SCL4 = 1; I2C_Delay(); if (SDA4) dat |= 0x01; SCL4 = 0; I2C_Delay(); break;
        }
    }
    return dat;
}
