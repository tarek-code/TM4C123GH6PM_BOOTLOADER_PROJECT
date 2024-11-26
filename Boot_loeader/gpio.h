#ifndef GPIO_H
#define GPIO_H

#include "TM4C123GH6PM.h"  // Use the device-specific header for TM4C123GH6PM

// Define LED pins (Example: using PORTF pins)
#define LED_RED_PIN     (1U << 1)  // PF1
#define LED_BLUE_PIN    (1U << 2)  // PF2

// GPIO Modes
typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT
} GPIO_Mode;

// Function prototypes
void GPIO_Init(GPIOA_Type *PORT, uint8_t pin, GPIO_Mode mode);
void GPIO_WritePin(GPIOA_Type *PORT, uint8_t pin, uint8_t state);
uint8_t GPIO_ReadPin(GPIOA_Type *PORT, uint8_t pin);

#endif // GPIO_H
