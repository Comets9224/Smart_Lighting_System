#ifndef UART_H
#define UART_H

#include <STC12C5A60S2.h>
extern bit BT_flag;
extern data unsigned int avg_lux;
extern data unsigned int percent;
extern bit is_duty_initialized; 
extern data unsigned int base_lux;
extern data unsigned int avg_lux;
extern bit LED_flag;
extern bit Time_flag;
void UART_Init(void);
void UART_SendByte(unsigned char dat);
void UART_SendString(unsigned char *str);
void UART_ISR(void);
void UartInfo(void);

#endif
