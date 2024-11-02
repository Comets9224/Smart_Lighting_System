#include "func.h"
#include "lcd12864.h"
#include "timer.h"
#include "stdio.h"
#include "key.h"
void LCD_Homepage(void)
{
// ��ʾ��ʼ��Ϣ
		char timeString[16];
		sprintf(timeString, "Time:%02u:%02u:%02u", hours, minutes, seconds);
		LCD_DisplayString(0, 0, "Lux: ");
    LCD_DisplayString(0, 1, timeString);  // �ڵڶ�����ʾʱ��
		LCD_DisplayString(0, 2, "Light: ");
		LCD_DisplayString(0, 3, "BtReady");
}
void DisplayTime(unsigned int hours, unsigned int minutes, unsigned int seconds,unsigned int i)
{
    char time[16];
		sprintf(time, "Time:%02u:%02u:%02u", hours, minutes, seconds);
		LCD_DisplayString(0, i, time);  

}

void Timemenu(void) {

    LCD_DisplayString(0, 0, "Time Adjust Menu");
		LCD_DisplayString(0, 1, "key1:On Adjust");
    LCD_DisplayString(0, 2, "key2:Off Adjust");
    LCD_DisplayString(0, 3, "key3:Now Adjust");
}
void On_Adjust(void) {
    LCD_Clear();
    LCD_DisplayString(0, 0, "On Adjust");
    DisplayTime(on_hours, on_minutes, on_seconds, 1);

    while (1) {
        char key = Key_Scan();
        if (key != '\0') {
            if (key == '1') {
                on_hours = (on_hours + 1) % 24; // ����Сʱ����ȷ���� 0-23 ֮��ѭ��
                DisplayTime(on_hours, on_minutes, on_seconds, 1);
            } else if (key == '2') {
                on_minutes = (on_minutes + 1) % 60; // ���ӷ��ӣ���ȷ���� 0-59 ֮��ѭ��
                DisplayTime(on_hours, on_minutes, on_seconds, 1);
            } else if (key == '3') {
                on_seconds = (on_seconds + 1) % 60; // �����룬��ȷ���� 0-59 ֮��ѭ��
                DisplayTime(on_hours, on_minutes, on_seconds, 1);
            } else if (key == '0') {
                LCD_DisplayString(0, 2, "press 0 again");
                break; // ���� '0' ���˳�����ģʽ
            }
        }
    }
}


void Off_Adjust(void) {
    LCD_Clear();
    LCD_DisplayString(0, 0, "Off Adjust");
    DisplayTime(off_hours, off_minutes, off_seconds, 1);

    while (1) {
        char key = Key_Scan();
        if (key != '\0') {
            if (key == '1') {
                off_hours = (off_hours + 1) % 24; // ����Сʱ����ȷ���� 0-23 ֮��ѭ��
                DisplayTime(off_hours, off_minutes, off_seconds, 1);
            } else if (key == '2') {
                off_minutes = (off_minutes + 1) % 60; // ���ӷ��ӣ���ȷ���� 0-59 ֮��ѭ��
                DisplayTime(off_hours, off_minutes, off_seconds, 1);
            } else if (key == '3') {
                off_seconds = (off_seconds + 1) % 60; // �����룬��ȷ���� 0-59 ֮��ѭ��
                DisplayTime(off_hours, off_minutes, off_seconds, 1);
            } else if (key == '0') {
                LCD_DisplayString(0, 2, "press 0 again");
                break; // ���� '0' ���˳�����ģʽ
            }
        }
    }
}

void Now_Adjust(void) {
    StopTimer();  // ֹͣ��ʱ���ж�
    LCD_Clear();
    LCD_DisplayString(0, 0, "Now Adjust");
    DisplayTime(hours, minutes, seconds,1);

    while (1) {
        char key = Key_Scan();
        if (key != '\0') {
            if (key == '1') {
                hours = (hours + 1) % 24; // ����Сʱ����ȷ���� 0-23 ֮��ѭ��
                DisplayTime(hours, minutes, seconds,1);
            } else if (key == '2') {
                minutes = (minutes + 1) % 60; // ���ӷ��ӣ���ȷ���� 0-59 ֮��ѭ��
                DisplayTime(hours, minutes, seconds,1);
            } else if (key == '3') {
                seconds = (seconds + 1) % 60; // �����룬��ȷ���� 0-59 ֮��ѭ��
                DisplayTime(hours, minutes, seconds,1);
            } else if (key == '0') {
              LCD_DisplayString(0, 2, "press 0 again");  
							break; // ���� '0' ���˳�����ģʽ
            }
        }
    }

    StartTimer();  // ����������ʱ���ж�
}
int isWithinOperatingTime(unsigned int hours, unsigned int minutes, unsigned int seconds,
                          unsigned int on_hours, unsigned int on_minutes, unsigned int on_seconds,
                          unsigned int off_hours, unsigned int off_minutes, unsigned int off_seconds) {
    // ��ʱ��ת��Ϊ�����Ա�Ƚ�
    unsigned long now_time = hours * 3600 + minutes * 60 + seconds;
    unsigned long on_time = on_hours * 3600 + on_minutes * 60 + on_seconds;
    unsigned long off_time = off_hours * 3600 + off_minutes * 60 + off_seconds;

    // ��鵱ǰʱ���Ƿ��ڿ����͹ػ�ʱ��֮��
    if (now_time >= on_time && now_time <= off_time) {
        return 1; // ��ʱ�䷶Χ��
    } else {
        return 0; // ����ʱ�䷶Χ��
    }
}
