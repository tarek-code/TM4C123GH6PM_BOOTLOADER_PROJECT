#include "button.h"

static void (*SW1_Callback)(void) = 0x00;
static void (*SW2_Callback)(void) = 0x00;


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



void Button_InitInterrupt(GPIOA_Type *PORT, uint8_t pin, void (*callback)(void)) {
    // Enable the clock for the GPIO port
    SYSCTL->RCGCGPIO |= (1U << 5);  // Enable clock for PORTF
    while ((SYSCTL->PRGPIO & (1U << 5)) == 0); // Wait for PORTF to be ready

    // Unlock PF0 if necessary
    if (PORT == GPIOF && pin == 0) {
        PORT->LOCK = 0x4C4F434B;  // Unlock GPIOCR register
        PORT->CR |= (1U << pin);   // Enable changes to PF0
    }

    // Configure the pin as input with pull-up resistor
    PORT->DIR &= ~(1U << pin);  // Set pin as input
    PORT->DEN |= (1U << pin);   // Enable digital function
    PORT->PUR |= (1U << pin);   // Enable pull-up resistor

    // Configure edge-triggered interrupts
    PORT->IS &= ~(1U << pin);  // Edge-sensitive
    PORT->IBE &= ~(1U << pin); // Interrupt controlled by IEV
    PORT->IEV &= ~(1U << pin); // Falling edge trigger
    PORT->IM |= (1U << pin);   // Unmask the interrupt

    // Assign the callback function
    if (pin == 4) {  // SW1
        SW1_Callback = callback;
    } else if (pin == 0) {  // SW2
        SW2_Callback = callback;
    }

    // Enable interrupts in NVIC
    NVIC_EnableIRQ(GPIOF_IRQn);
}

// GPIO Port F Interrupt Handler
void GPIOF_Handler(void) {
    if ((GPIOF->MIS & BUTTON1_PIN) && SW1_Callback) {
        GPIOF->ICR |= BUTTON1_PIN;  // Clear the interrupt flag
        SW1_Callback();         // Call SW1 callback
    }
    if ((GPIOF->MIS & BUTTON2_PIN) && SW2_Callback) {
        GPIOF->ICR |= BUTTON2_PIN;  // Clear the interrupt flag
        SW2_Callback();         // Call SW2 callback
    }
}