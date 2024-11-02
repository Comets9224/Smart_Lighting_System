#include "pwm.h"

void PWM_Init(void) {
    // ����PCAģ��
    CMOD = 0x02;  // PCAʱ��ԴΪFosc/2
    CCAP0H = 0x00; // ��ʼռ�ձ�Ϊ0
    CCAP0L = 0x00;
    CCAPM0 = 0x42; // ģ��0ΪPWMģʽ
		CR = 1; // ����PCA������
		PWM_SetDuty(0);//  ��ʼռ�ձ�Ϊ0   Ҳ�������Ϊ0
}

void PWM_SetDuty(unsigned char duty) {
    // ��תռ�ձ�����Ӧ�͵�ƽ��Ч
    duty = 255 - duty;
    CCAP0H = duty;
    CCAP0L = duty;
}

