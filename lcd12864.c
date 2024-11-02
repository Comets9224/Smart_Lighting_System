#include "lcd12864.h"

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = ms; i > 0; i--)
        for (j = 110; j > 0; j--);
}

void LCD_SendByte(unsigned char dat)
{
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        LCD_CLK = 0;
        LCD_DATA = (dat & 0x80) ? 1 : 0;
        dat <<= 1;
        LCD_CLK = 1;
    }
}

void LCD_SendCmd(unsigned char cmd)
{
    LCD_SendByte(0xF8);
    LCD_SendByte(cmd & 0xF0);
    LCD_SendByte((cmd & 0x0F) << 4);
}

void LCD_SendData(unsigned char dat)
{
    LCD_SendByte(0xFA);
    LCD_SendByte(dat & 0xF0);
    LCD_SendByte((dat & 0x0F) << 4);
}

void LCD_Init(void)
{
    delay_ms(50);
    LCD_SendCmd(0x30);
    delay_ms(1);
    LCD_SendCmd(0x0C);
    delay_ms(1);
    LCD_SendCmd(0x01);
    delay_ms(10);
	  LCD_Clear();
}

void LCD_SetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    if (y == 0)
        addr = 0x80 + x;
    else if (y == 1)
        addr = 0x90 + x;
    else if (y == 2)
        addr = 0x88 + x;
    else if (y == 3)
        addr = 0x98 + x;
    LCD_SendCmd(addr);
}

void LCD_DisplayString(unsigned char x, unsigned char y, unsigned char *str)
{		 LCD_ClearLine(y);
    LCD_SetCursor(x, y);
    while (*str)
    {
        LCD_SendData(*str++);
    }
}


void LCD_UpdateChar(unsigned char x, unsigned char y, unsigned char ch)// 一次更新一个位置的字符  比如A和senddata 在功能上是相同的，前提是光标已经设置到 (0, 0) 位置。
{
    LCD_SetCursor(x, y);
    LCD_SendData(ch);
}

void LCD_ClearLine(unsigned char y)
{
    
		unsigned char i = 0;
		LCD_SetCursor(0, y);
    for (i; i < 16; i++)  // 假设每行有16个字符
    {
        LCD_SendData(' ');
    }
}

void LCD_Clear(void)
{
    LCD_SendCmd(0x01);  // 清屏命令
    delay_ms(10);       // 等待清屏完成
}