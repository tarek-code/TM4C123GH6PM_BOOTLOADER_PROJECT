#ifndef BUTTON_H
#define BUTTON_H

#include "TM4C123GH6PM.h"

// Define Button Pins (Example: assuming buttons on PF4 and PF0)
#define BUTTON1_PIN (1U << 4)  // PF4
#define BUTTON2_PIN (1U << 0)  // PF0

// Function prototypes
void Button_Init(GPIOA_Type *PORT, uint8_t pin);
uint8_t Button_Read(GPIOA_Type *PORT, uint8_t pin);

#endif // BUTTON_H
