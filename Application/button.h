#ifndef BUTTON_H
#define BUTTON_H

#include "TM4C123GH6PM.h"

// Define Button Pins (Example: assuming buttons on PF4 and PF0)
#define BUTTON1_PIN (1U << 4)  // PF4
#define BUTTON2_PIN (1U << 0)  // PF0

#define SW1_PIN ( 4)  // PF4
#define SW2_PIN ( 0)  // PF0

// Function prototypes
void Button_Init(GPIOA_Type *PORT, uint8_t pin);
uint8_t Button_Read(GPIOA_Type *PORT, uint8_t pin);
void Button_InitInterrupt(GPIOA_Type *PORT, uint8_t pin, void (*callback)(void));
 void GPIOF_Handler(void); // Interrupt handler for Port F
#endif // BUTTON_H
