#ifndef PWM_H
#define PWM_H

#include <STC12C5A60S2.h>

void PWM_Init(void);
void PWM_SetDuty(unsigned char duty);

#endif // PWM_H
