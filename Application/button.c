#include "button.h"

// Initialize button pin
void Button_Init(GPIOA_Type *PORT, uint8_t pin) {
    // Enable clock for the port if not already done
    SYSCTL->RCGCGPIO |= (1U << 5);  // Enable clock for PORTF
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0); // Wait for PORTF to be ready

    // Unlock PF0 if necessary
    if (PORT == GPIOF && pin == 0) {
        PORT->LOCK = 0x4C4F434B;  // Unlock GPIOCR register
        PORT->CR |= (1U << pin);   // Enable changes to PF0
    }

    // Set pin as digital input
    PORT->DIR &= ~(1U << pin);  // Set pin as input
    PORT->DEN |= (1U << pin);   // Enable digital function

    // Enable pull-up resistor
    PORT->PUR |= (1U << pin);   // Enable pull-up on button pin
}

// Read button state (returns 1 if pressed, 0 if not pressed)
uint8_t Button_Read(GPIOA_Type *PORT, uint8_t pin) {
    return (PORT->DATA & (1U << pin)) == 0 ? 1 : 0;  // Active low
}
