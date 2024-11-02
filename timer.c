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
    TMOD &= 0xF0;  // �����4λ����ʱ��0������λ��
    TMOD |= 0x01;  // ���ö�ʱ��0Ϊģʽ1��16λ��ʱ����
    TH0 = 0xFC;    // ���س�ֵ
    TL0 = 0x66;
    ET0 = 1;       // ʹ�ܶ�ʱ��0�ж�
    TR0 = 1;       // ������ʱ��0
}

void Timer0_ISR(void) interrupt 1
{
    static unsigned int count = 0;

    TH0 = 0xFC;  // ���¼��ض�ʱ����ֵ
    TL0 = 0x66;

    count++;
    if (count >= 1000) // ÿ1�����һ��ʱ��
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
    ET0 = 0;  // ���ö�ʱ�� 0 �ж�
    TR0 = 0;  // ֹͣ��ʱ�� 0
}
void StartTimer(void) {
    TR0 = 1;  // ������ʱ�� 0
    ET0 = 1;  // ���ö�ʱ�� 0 �ж�
}
