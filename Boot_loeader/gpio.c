#include "gpio.h"

// Function to initialize GPIO pin
void GPIO_Init(GPIOA_Type *PORT, uint8_t pin, GPIO_Mode mode) {
    // Enable clock for the corresponding GPIO port
    if (PORT == GPIOA) SYSCTL->RCGCGPIO |= (1U << 0);
    else if (PORT == GPIOB) SYSCTL->RCGCGPIO |= (1U << 1);
    else if (PORT == GPIOC) SYSCTL->RCGCGPIO |= (1U << 2);
    else if (PORT == GPIOD) SYSCTL->RCGCGPIO |= (1U << 3);
    else if (PORT == GPIOE) SYSCTL->RCGCGPIO |= (1U << 4);
    else if (PORT == GPIOF) SYSCTL->RCGCGPIO |= (1U << 5);

    // Wait until the GPIO port is ready
    while ((SYSCTL->PRGPIO & (1U << ((PORT == GPIOA) ? 0 :
                                     (PORT == GPIOB) ? 1 :
                                     (PORT == GPIOC) ? 2 :
                                     (PORT == GPIOD) ? 3 :
                                     (PORT == GPIOE) ? 4 : 5))) == 0);

    // Unlock GPIOF if necessary (e.g., for PF0)
    if (PORT == GPIOF && pin == 0) {
        PORT->LOCK = 0x4C4F434B;
        PORT->CR |= (1U << pin);
    }

    // Set the pin as digital
    PORT->DEN |= (1U << pin);

    // Configure pin as input or output
    if (mode == GPIO_MODE_OUTPUT) {
        PORT->DIR |= (1U << pin);  // Set pin as output
    } else {
        PORT->DIR &= ~(1U << pin); // Set pin as input
    }
}

// Function to write to GPIO pin
void GPIO_WritePin(GPIOA_Type *PORT, uint8_t pin, uint8_t state) {
    if (state) {
        PORT->DATA |= (1U << pin);  // Set pin
    } else {
        PORT->DATA &= ~(1U << pin); // Clear pin
    }
}

// Function to read from GPIO pin
uint8_t GPIO_ReadPin(GPIOA_Type *PORT, uint8_t pin) {
    return (PORT->DATA & (1U << pin)) ? 1 : 0;
}
