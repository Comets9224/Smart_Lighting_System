#include "bh1750.h"
#include "i2c.h"
#include "lcd12864.h"
// 初始化BH1750
void BH1750_Init(unsigned char bus)
{
    I2C_Init(bus);
    
    I2C_Start(bus);
    I2C_SendByte(bus, BH1750_ADDR);
    I2C_SendByte(bus, POWER_ON);
    I2C_Stop(bus);
    
    I2C_Start(bus);
    I2C_SendByte(bus, BH1750_ADDR);
    I2C_SendByte(bus, CONT_H_RES_MODE);
    I2C_Stop(bus);

}

// 读取光照强度
unsigned int BH1750_ReadLight(unsigned char bus)
{
    unsigned int temp;
    unsigned char msb, lsb;
    
    I2C_Start(bus);
    I2C_SendByte(bus, BH1750_ADDR | 0x01);

    msb = I2C_RecvByte(bus);
    I2C_SendAck(bus, 0);
    
    lsb = I2C_RecvByte(bus);
    I2C_SendAck(bus, 1);
    
    I2C_Stop(bus);
    
    temp = (msb << 8) | lsb;
    temp = (unsigned int)(((float)temp) / 1.2);
    
    return temp;
}
void BH1750_init()
{
    // 初始化4个BH1750传感器
    BH1750_Init(1);
    BH1750_Init(2);
    BH1750_Init(3);
    BH1750_Init(4);
		delay_ms(120);

}
