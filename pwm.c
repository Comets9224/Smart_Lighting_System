#include "pwm.h"

void PWM_Init(void) {
    // 设置PCA模块
    CMOD = 0x02;  // PCA时钟源为Fosc/2
    CCAP0H = 0x00; // 初始占空比为0
    CCAP0L = 0x00;
    CCAPM0 = 0x42; // 模块0为PWM模式
		CR = 1; // 启动PCA计数器
		PWM_SetDuty(0);//  初始占空比为0   也就是输出为0
}

void PWM_SetDuty(unsigned char duty) {
    // 反转占空比以适应低电平有效
    duty = 255 - duty;
    CCAP0H = duty;
    CCAP0L = duty;
}

