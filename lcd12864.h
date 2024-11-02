#ifndef LCD_H
#define LCD_H

#include <STC12C5A60S2.h>

sbit LCD_DATA = P3^7;
sbit LCD_CLK = P3^6;
sbit LCD_BL = P2^0;
void LCD_Clear(void);
void LCD_Init(void);
void LCD_SetCursor(unsigned char x, unsigned char y);
void LCD_DisplayString(unsigned char x, unsigned char y, unsigned char *str);
void LCD_SendData(unsigned char dat); 
void delay_ms(unsigned int ms);

void LCD_UpdateChar(unsigned char x, unsigned char y, unsigned char ch);
void LCD_ClearLine(unsigned char y);
#endif
