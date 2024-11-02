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
bit adjustment_needed = 0; //��ʾ�Ƿ���Ҫ����
data unsigned int base_lux=0;
data unsigned int percent = 0;
data unsigned int avg_lux;
void main(void)
{		
    data char lux_str[20];
    data float total_lux;
    
		unsigned int duty;

    LCD_BL = 0;  // �򿪱��⣨�͵�ƽ��Ч��
    Key_Init();
    Timer0_Init();
    LCD_Init();
    UART_Init();
    BH1750_init(); // ��ʼ���ĸ�1750
    PWM_Init();  // ��ʼ��PWM

    EA = 1;
    LCD_Homepage();
    
    UART_SendString("Bluetooth already linked\r\n");
        
    while (1)
    {		
        char key;
        
        // ��ȡ4��BH1750������������
        lux[0] = BH1750_ReadLight(1);
        lux[1] = BH1750_ReadLight(2);
        lux[2] = BH1750_ReadLight(3);
        lux[3] = BH1750_ReadLight(4);
        delay_ms(120);//��ȡ��Ҫʱ��

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
                    
            // ����Ƿ��д���
            for (i = 0; i < 4; i++) {
                if (lux[i] == -10924) {
                    sprintf(lux_str, "ERROR: Lux[%d]", i + 1);
                    LCD_DisplayString(0, 0, lux_str);
                    error_flag = 1;
                    break;
                }
            }

            if (!error_flag) {
                // ����ƽ���ն�
                total_lux = (float)(lux[0] + lux[1] + lux[2] + lux[3]);
                avg_lux = (unsigned int)(total_lux / 4);

                // ��ʾƽ���ն�
                sprintf(lux_str, "lux: %4d", avg_lux);
								
                LCD_DisplayString(0, 0, lux_str);

                // ����lux_flagΪ1
                LUX_flag = 1;
            }
						
					sprintf(lux_str, "Light: %2d per", percent);
					LCD_DisplayString(0, 2, lux_str);
        }	

        if ((BT_flag == 1) && (HP_flag == 1))//����׼�����˵ķ�����Ϣ
					{
            delay_ms(15000);
            LCD_DisplayString(0, 3, "BtReady");
            BT_flag = 0;
        }
        
        key = Key_Scan();
        if (key != '\0') 
        {
            // ������
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
            duty = 125; // ��50%���ȿ�ʼ
            PWM_SetDuty(duty);
            delay_ms(5000); // �ȴ�5���û������ȶ�

            // ��ȡ�ն�ֵ
            lux[0] = BH1750_ReadLight(1);
            lux[1] = BH1750_ReadLight(2);
            lux[2] = BH1750_ReadLight(3);
            lux[3] = BH1750_ReadLight(4);
            total_lux = lux[0] + lux[1] + lux[2] + lux[3];
            avg_lux = total_lux / 4;

            base_lux = avg_lux; // ���û�׼ֵ
            is_duty_initialized = 1; // ����ѳ�ʼ��
        }

        if (LUX_flag == 1 && is_duty_initialized == 1)
        {
            // ����Ƿ���Ҫ����
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
                        duty = 255; // �������
                    }
                } else if (avg_lux > target_high) {
                    adjustment = (avg_lux - target_high) / 10;
                    adjustment = (adjustment > 10) ? 10 : (adjustment < 1) ? 1 : adjustment;
                    if (duty >= adjustment) {
                        duty -= adjustment;
                    } else {
                        duty = 0; // ��С����
                    }
                } else {
                    // ��Ŀ�귶Χ�ڣ�ֹͣ����
                    adjustment_needed = 0;
                }
                PWM_SetDuty(duty);
            }
        }
        
        percent = (duty * 100) / 255;
    }

    if ((LED_flag == 1) && (Time_flag == 0))
    {
        duty = 125; // ����Ϊ50%����
        PWM_SetDuty(duty);
        percent = (duty * 100) / 255;
        delay_ms(5000);
        LED_flag = 0;
        is_duty_initialized = 0; // ���ñ�־λ
        adjustment_needed = 0; // ���õ�����־
        base_lux = 0; // �����׼ֵ
    }

    if (LED_flag == 0)
    {		
				duty = 0;
				PWM_SetDuty(duty);
        base_lux = 0; // �����׼ֵ
        percent = 0;
        is_duty_initialized = 0; // ���ñ�־λ
        adjustment_needed = 0; // ���õ�����־
    }

        delay_ms(1000);  // ��ֹ��Ļˢ��̫����˸
    }
}
