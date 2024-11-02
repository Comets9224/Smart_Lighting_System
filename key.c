#include "lcd12864.h"
#include "key.h"
#define DEBOUNCE_DELAY 20

void Key_Init(void) {
    P0 = 0xFF; // Set P0 as input
}
char Key_Scan(void) {
    if (!KEY0) { // Check if KEY0 is pressed
        delay_ms(DEBOUNCE_DELAY); // Debounce delay
        if (!KEY0) {
            while (!KEY0); // Wait for key release
            return '0';
        }
    }
    if (!KEY1) { // Check if KEY1 is pressed
        delay_ms(DEBOUNCE_DELAY);
        if (!KEY1) {
            while (!KEY1);
            return '1';
        }
    }
    if (!KEY2) { // Check if KEY2 is pressed
        delay_ms(DEBOUNCE_DELAY);
        if (!KEY2) {
            while (!KEY2);
            return '2';
        }
    }
    if (!KEY3) { // Check if KEY3 is pressed
        delay_ms(DEBOUNCE_DELAY);
        if (!KEY3) {
            while (!KEY3);
            return '3';
        }
    }
		  if (!KEY4) { // Check if KEY3 is pressed
        delay_ms(DEBOUNCE_DELAY);
        if (!KEY4) {
            while (!KEY4);
            return '4';
        }
    }
    return '\0'; // No key pressed
}