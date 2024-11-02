#include "func.h"
#include "lcd12864.h"
#include "timer.h"
#include "stdio.h"
#include "key.h"
void LCD_Homepage(void)
{
// 显示初始信息
		char timeString[16];
		sprintf(timeString, "Time:%02u:%02u:%02u", hours, minutes, seconds);
		LCD_DisplayString(0, 0, "Lux: ");
    LCD_DisplayString(0, 1, timeString);  // 在第二行显示时间
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
                on_hours = (on_hours + 1) % 24; // 增加小时，并确保在 0-23 之间循环
                DisplayTime(on_hours, on_minutes, on_seconds, 1);
            } else if (key == '2') {
                on_minutes = (on_minutes + 1) % 60; // 增加分钟，并确保在 0-59 之间循环
                DisplayTime(on_hours, on_minutes, on_seconds, 1);
            } else if (key == '3') {
                on_seconds = (on_seconds + 1) % 60; // 增加秒，并确保在 0-59 之间循环
                DisplayTime(on_hours, on_minutes, on_seconds, 1);
            } else if (key == '0') {
                LCD_DisplayString(0, 2, "press 0 again");
                break; // 按下 '0' 键退出调整模式
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
                off_hours = (off_hours + 1) % 24; // 增加小时，并确保在 0-23 之间循环
                DisplayTime(off_hours, off_minutes, off_seconds, 1);
            } else if (key == '2') {
                off_minutes = (off_minutes + 1) % 60; // 增加分钟，并确保在 0-59 之间循环
                DisplayTime(off_hours, off_minutes, off_seconds, 1);
            } else if (key == '3') {
                off_seconds = (off_seconds + 1) % 60; // 增加秒，并确保在 0-59 之间循环
                DisplayTime(off_hours, off_minutes, off_seconds, 1);
            } else if (key == '0') {
                LCD_DisplayString(0, 2, "press 0 again");
                break; // 按下 '0' 键退出调整模式
            }
        }
    }
}

void Now_Adjust(void) {
    StopTimer();  // 停止定时器中断
    LCD_Clear();
    LCD_DisplayString(0, 0, "Now Adjust");
    DisplayTime(hours, minutes, seconds,1);

    while (1) {
        char key = Key_Scan();
        if (key != '\0') {
            if (key == '1') {
                hours = (hours + 1) % 24; // 增加小时，并确保在 0-23 之间循环
                DisplayTime(hours, minutes, seconds,1);
            } else if (key == '2') {
                minutes = (minutes + 1) % 60; // 增加分钟，并确保在 0-59 之间循环
                DisplayTime(hours, minutes, seconds,1);
            } else if (key == '3') {
                seconds = (seconds + 1) % 60; // 增加秒，并确保在 0-59 之间循环
                DisplayTime(hours, minutes, seconds,1);
            } else if (key == '0') {
              LCD_DisplayString(0, 2, "press 0 again");  
							break; // 按下 '0' 键退出调整模式
            }
        }
    }

    StartTimer();  // 重新启动定时器中断
}
int isWithinOperatingTime(unsigned int hours, unsigned int minutes, unsigned int seconds,
                          unsigned int on_hours, unsigned int on_minutes, unsigned int on_seconds,
                          unsigned int off_hours, unsigned int off_minutes, unsigned int off_seconds) {
    // 将时间转换为秒数以便比较
    unsigned long now_time = hours * 3600 + minutes * 60 + seconds;
    unsigned long on_time = on_hours * 3600 + on_minutes * 60 + on_seconds;
    unsigned long off_time = off_hours * 3600 + off_minutes * 60 + off_seconds;

    // 检查当前时间是否在开机和关机时间之间
    if (now_time >= on_time && now_time <= off_time) {
        return 1; // 在时间范围内
    } else {
        return 0; // 不在时间范围内
    }
}
