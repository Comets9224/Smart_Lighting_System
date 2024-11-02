#ifndef TIMER_H
#define TIMER_H
extern unsigned int hours;
extern unsigned int minutes;
extern unsigned int seconds;
extern unsigned int on_hours;
extern unsigned int on_minutes;
extern unsigned int on_seconds;

extern unsigned int off_hours;
extern unsigned int off_minutes;
extern unsigned int off_seconds;
void Timer0_Init(void);
void Timer0_ISR(void);
void StopTimer(void);
void StartTimer(void);

#endif // TIMER_H
