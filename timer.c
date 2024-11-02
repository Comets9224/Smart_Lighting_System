#include <STC12C5A60S2.h>
#include "timer.h"

unsigned int hours = 18;
unsigned int minutes = 0;
unsigned int seconds = 0;
unsigned int on_hours = 18;
unsigned int on_minutes = 30;
unsigned int on_seconds = 0;
unsigned int off_hours = 23;
unsigned int off_minutes = 0;
unsigned int off_seconds = 0;

void Timer0_Init(void)
{
    TMOD &= 0xF0;  // 清除低4位（定时器0的配置位）
    TMOD |= 0x01;  // 设置定时器0为模式1（16位定时器）
    TH0 = 0xFC;    // 加载初值
    TL0 = 0x66;
    ET0 = 1;       // 使能定时器0中断
    TR0 = 1;       // 启动定时器0
}

void Timer0_ISR(void) interrupt 1
{
    static unsigned int count = 0;

    TH0 = 0xFC;  // 重新加载定时器初值
    TL0 = 0x66;

    count++;
    if (count >= 1000) // 每1秒更新一次时间
    {
        count = 0;
        seconds++;
        if (seconds >= 60)
        {
            seconds = 0;
            minutes++;
            if (minutes >= 60)
            {
                minutes = 0;
                hours++;
                if (hours >= 24)
                {
                    hours = 0;
                }
            }
        }
    }
}
void StopTimer(void) {
    ET0 = 0;  // 禁用定时器 0 中断
    TR0 = 0;  // 停止定时器 0
}
void StartTimer(void) {
    TR0 = 1;  // 启动定时器 0
    ET0 = 1;  // 启用定时器 0 中断
}
