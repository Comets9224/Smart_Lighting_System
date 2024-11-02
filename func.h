#ifndef FUNC_H
#define FUNC_H
void LCD_Homepage(void);
void Timemenu(void) ;
void DisplayTime(unsigned int hours, unsigned int minutes, unsigned int seconds,unsigned int i);
void On_Adjust(void);
void Off_Adjust(void);
void Now_Adjust(void);
int isWithinOperatingTime(unsigned int hours, unsigned int minutes, unsigned int seconds,
                          unsigned int on_hours, unsigned int on_minutes, unsigned int on_seconds,
                          unsigned int off_hours, unsigned int off_minutes, unsigned int off_seconds);
#endif // MAIN_H