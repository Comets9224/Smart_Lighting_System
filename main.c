#include <STC12C5A60S2.h>
#include "lcd12864.h"
#include "uart.h"
#include "timer.h"
#include "stdio.h"
#include "key.h"
#include "main.h"
#include "func.h"
#include "bh1750.h"
#include "pwm.h"

bit HP_flag = 1;
bit MU_flag = 0;
bit Third_flag = 0;
bit LUX_flag = 0;
data unsigned int lux[4];
bit LED_flag = 0;
bit Time_flag=0;
bit is_duty_initialized = 0; 
bit adjustment_needed = 0; //表示是否需要调整
data unsigned int base_lux=0;
data unsigned int percent = 0;
data unsigned int avg_lux;
void main(void)
{		
    data char lux_str[20];
    data float total_lux;
    
		unsigned int duty;

    LCD_BL = 0;  // 打开背光（低电平有效）
    Key_Init();
    Timer0_Init();
    LCD_Init();
    UART_Init();
    BH1750_init(); // 初始化四个1750
    PWM_Init();  // 初始化PWM

    EA = 1;
    LCD_Homepage();
    
    UART_SendString("Bluetooth already linked\r\n");
        
    while (1)
    {		
        char key;
        
        // 读取4个BH1750传感器的数据
        lux[0] = BH1750_ReadLight(1);
        lux[1] = BH1750_ReadLight(2);
        lux[2] = BH1750_ReadLight(3);
        lux[3] = BH1750_ReadLight(4);
        delay_ms(120);//读取需要时间

        if (isWithinOperatingTime(hours, minutes, seconds, on_hours, on_minutes, on_seconds, off_hours, off_minutes, off_seconds)) 
					{
						Time_flag = 1;
					}
					else {
						if(blue_timeflag==1)
						{	Time_flag = 1;}
						else
						{Time_flag = 0;}
							}
        
        if (HP_flag == 1)
        {    
            int i = 0;	
            bit error_flag = 0;
            DisplayTime(hours, minutes, seconds, 1);
                    
            // 检查是否有错误
            for (i = 0; i < 4; i++) {
                if (lux[i] == -10924) {
                    sprintf(lux_str, "ERROR: Lux[%d]", i + 1);
                    LCD_DisplayString(0, 0, lux_str);
                    error_flag = 1;
                    break;
                }
            }

            if (!error_flag) {
                // 计算平均照度
                total_lux = (float)(lux[0] + lux[1] + lux[2] + lux[3]);
                avg_lux = (unsigned int)(total_lux / 4);

                // 显示平均照度
                sprintf(lux_str, "lux: %4d", avg_lux);
								
                LCD_DisplayString(0, 0, lux_str);

                // 设置lux_flag为1
                LUX_flag = 1;
            }
						
					sprintf(lux_str, "Light: %2d per", percent);
					LCD_DisplayString(0, 2, lux_str);
        }	

        if ((BT_flag == 1) && (HP_flag == 1))//蓝牙准备好了的返回信息
					{
            delay_ms(15000);
            LCD_DisplayString(0, 3, "BtReady");
            BT_flag = 0;
        }
        
        key = Key_Scan();
        if (key != '\0') 
        {
            // 处理按键
            if (key == '0') 
            {
                if (Third_flag == 0)
                {
                    MU_flag = ~MU_flag;
                    if (MU_flag)
                    {
                        HP_flag = 0; 
                        Timemenu();
                    }
                    else
                    {
                        HP_flag = 1; 
                        LCD_Homepage();
                    }
                }
                if (Third_flag == 1)
                {
                    Third_flag = 0;
                    LCD_Clear();
                    Timemenu();
                    MU_flag = 1;
                }
            }

            if ((key == '1') && (MU_flag == 1) && (Third_flag == 0))
            {
                On_Adjust();
                Third_flag = 1;
            }

            if ((key == '2') && (MU_flag == 1) && (Third_flag == 0))
            {
                Off_Adjust();
                Third_flag = 1;
            }

            if ((key == '3') && (MU_flag == 1) && (Third_flag == 0))
            {
                Now_Adjust();
                Third_flag = 1;
            }
                        
            if ((key == '4'))
            {
                LED_flag = ~LED_flag;
             
            }
        }
if ((LED_flag == 1) && (Time_flag == 1))
    {
        if (!is_duty_initialized) {
            duty = 125; // 从50%亮度开始
            PWM_SetDuty(duty);
            delay_ms(5000); // 等待5秒让环境光稳定

            // 读取照度值
            lux[0] = BH1750_ReadLight(1);
            lux[1] = BH1750_ReadLight(2);
            lux[2] = BH1750_ReadLight(3);
            lux[3] = BH1750_ReadLight(4);
            total_lux = lux[0] + lux[1] + lux[2] + lux[3];
            avg_lux = total_lux / 4;

            base_lux = avg_lux; // 设置基准值
            is_duty_initialized = 1; // 标记已初始化
        }

        if (LUX_flag == 1 && is_duty_initialized == 1)
        {
            // 检查是否需要调整
            if (avg_lux < base_lux * 92 / 100 || avg_lux > base_lux * 108 / 100) {
                adjustment_needed = 1;
            }

            if (adjustment_needed) {
                int target_low = base_lux * 97 / 100;
                int target_high = base_lux * 103 / 100;
                int adjustment = 0;

                if (avg_lux < target_low) {
                    adjustment = (target_low - avg_lux) / 10;
                    adjustment = (adjustment > 10) ? 10 : (adjustment < 1) ? 1 : adjustment;
                    if (duty + adjustment <= 255) {
                        duty += adjustment;
                    } else {
                        duty = 255; // 最大限制
                    }
                } else if (avg_lux > target_high) {
                    adjustment = (avg_lux - target_high) / 10;
                    adjustment = (adjustment > 10) ? 10 : (adjustment < 1) ? 1 : adjustment;
                    if (duty >= adjustment) {
                        duty -= adjustment;
                    } else {
                        duty = 0; // 最小限制
                    }
                } else {
                    // 在目标范围内，停止调整
                    adjustment_needed = 0;
                }
                PWM_SetDuty(duty);
            }
        }
        
        percent = (duty * 100) / 255;
    }

    if ((LED_flag == 1) && (Time_flag == 0))
    {
        duty = 125; // 设置为50%亮度
        PWM_SetDuty(duty);
        percent = (duty * 100) / 255;
        delay_ms(5000);
        LED_flag = 0;
        is_duty_initialized = 0; // 重置标志位
        adjustment_needed = 0; // 重置调整标志
        base_lux = 0; // 清除基准值
    }

    if (LED_flag == 0)
    {		
				duty = 0;
				PWM_SetDuty(duty);
        base_lux = 0; // 清除基准值
        percent = 0;
        is_duty_initialized = 0; // 重置标志位
        adjustment_needed = 0; // 重置调整标志
    }

        delay_ms(1000);  // 防止屏幕刷新太快闪烁
    }
}
