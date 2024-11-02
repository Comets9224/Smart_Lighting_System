#include "uart.h"
#include "lcd12864.h"
#include "timer.h"
#include "main.h"
#include "stdio.h"
bit BT_flag=0;
bit blue_timeflag;
char buffer[16];  // 在函数中定义 buffer
char BT_op='0';
void UART_Init(void)
{
    SCON = 0x50;   // 设置串口工作模式
    TMOD &= 0x0F;  // 清除高4位（定时器1的配置位）
    TMOD |= 0x20;  // 设置定时器1为模式2（8位自动重装）
    TH1 = 0xFD;    // 设置波特率为9600
    TL1 = 0xFD;
    TR1 = 1;       // 启动定时器1
    ES = 1;        // 使能串口中断
}

void UART_SendByte(unsigned char dat)
{
    SBUF = dat;
    while(!TI);
    TI = 0;
}

void UART_SendString(unsigned char *str)
{
    while(*str)
    {
        UART_SendByte(*str++);
    }
}
void UART_ISR(void) interrupt 4 {
    if (RI) {
        RI = 0;

        switch (SBUF) {
            case 'A':
								if(HP_flag == 1)
									LCD_DisplayString(0, 3, "On Time Adj");
                if (BT_op != '1') {
                    UART_SendString("Enter On Adj Mode\r\n");
                    BT_op = '1';
                } else {
                    UART_SendString("Exit On Adj Mode\r\n");
                    BT_op = '0';
                }
                break;
            case 'B':
								if(HP_flag == 1)
									LCD_DisplayString(0, 3, "Off Time Adj");
                if (BT_op != '2') {
                    UART_SendString("Enter Off Adj Mode\r\n");
                    BT_op = '2';
                } else {
                    UART_SendString("Exit Off Adj Mode\r\n");
                    BT_op = '0';
                }
                break;
            case 'C':
								if(HP_flag == 1)
									LCD_DisplayString(0, 3, "Now Time Adj");
                if (BT_op != '3') {
                    UART_SendString("Enter Current Adj Mode\r\n");
                    BT_op = '3';
                } else {
                    UART_SendString("Exit Current Adj Mode\r\n");
                    BT_op = '0';
                }
                break;
            case 'D':
                if (HP_flag == 1&&(BT_op!='0')) {
                    LCD_DisplayString(0, 3, "Hour add 1h");
                }
                if (BT_op == '1') {
                    on_hours = (on_hours + 1) % 24;
                    UART_SendString("On Time: ");
                    sprintf(buffer, "%d:%d:%d\r\n", on_hours, on_minutes, on_seconds);
                    UART_SendString(buffer);
                } else if (BT_op == '2') {
                    off_hours = (off_hours + 1) % 24;
                    UART_SendString("Off Time: ");
                    sprintf(buffer, "%d:%d:%d\r\n", off_hours, off_minutes, off_seconds);
                    UART_SendString(buffer);
                } else if (BT_op == '3') {
                    hours = (hours + 1) % 24;
                    UART_SendString("Current Time: ");
                    sprintf(buffer, "%d:%d:%d\r\n", hours, minutes, seconds);
                    UART_SendString(buffer);
                }
                break;
            case 'E':
                if (HP_flag == 1&&(BT_op!='0')) {
                    LCD_DisplayString(0, 3, "Min add 1min");
                }
                if (BT_op == '1') {
                    on_minutes = (on_minutes + 1) % 60;
                    UART_SendString("On Time: ");
                    sprintf(buffer, "%d:%d:%d\r\n", on_hours, on_minutes, on_seconds);
                    UART_SendString(buffer);
                } else if (BT_op == '2') {
                    off_minutes = (off_minutes + 1) % 60;
                    UART_SendString("Off Time: ");
                    sprintf(buffer, "%d:%d:%d\r\n", off_hours, off_minutes, off_seconds);
                    UART_SendString(buffer);
                } else if (BT_op == '3') {
                    minutes = (minutes + 1) % 60;
                    UART_SendString("Current Time: ");
                    sprintf(buffer, "%d:%d:%d\r\n", hours, minutes, seconds);
                    UART_SendString(buffer);
                }
                break;
								case 'I':
									if (HP_flag == 1) {
                     LCD_DisplayString(0, 3, "OnOffLight");
                }
								
								LED_flag = ~LED_flag;
								
								if(LED_flag==1)
								{UART_SendString("Waiting On...");
									blue_timeflag=1;
								}
								if(LED_flag==0)
								{UART_SendString("Waiting Off...");
								 blue_timeflag=0;}
                break;
								case 'F':
										if (HP_flag == 1) {
                   LCD_DisplayString(0, 3, "SysInfoReturn");
                }
                
                UartInfo();
                break;
        }
        BT_flag = 1;
    }
}


void UartInfo(void) {

    // 发送当前时间
    UART_SendString("Current Time: ");
    sprintf(buffer, "%d", hours);
    UART_SendString(buffer);
    UART_SendString(":");
    sprintf(buffer, "%d", minutes);
    UART_SendString(buffer);
    UART_SendString(":");
    sprintf(buffer, "%d", seconds);
    UART_SendString(buffer);
    UART_SendString("\r\n");

    // 发送开启时间
    UART_SendString("On Time: ");
    sprintf(buffer, "%d", on_hours);
    UART_SendString(buffer);
    UART_SendString(":");
    sprintf(buffer, "%d", on_minutes);
    UART_SendString(buffer);
    UART_SendString(":");
    sprintf(buffer, "%d", on_seconds);
    UART_SendString(buffer);
    UART_SendString("\r\n");

    // 发送关闭时间
    UART_SendString("Off Time: ");
    sprintf(buffer, "%d", off_hours);
    UART_SendString(buffer);
    UART_SendString(":");
    sprintf(buffer, "%d", off_minutes);
    UART_SendString(buffer);
    UART_SendString(":");
    sprintf(buffer, "%d", off_seconds);
    UART_SendString(buffer);
    UART_SendString("\r\n");
		
		//发送亮度信息
		UART_SendString("Light: ");
    sprintf(buffer, "%d per", percent);
    UART_SendString(buffer);
    UART_SendString("\r\n");
		//发送照度信息
		
		UART_SendString("Lux: ");
    sprintf(buffer, "%d",avg_lux);
    UART_SendString(buffer);
    UART_SendString("\r\n");

}