#include "sysclockcfg.h"
#include "button.h"
#include "gpio.h"

#define LED_RED_PIN 1    // PF1 is Red LED
#define BUTTON_PIN 0     // PF0 is Button
#include <stdint.h>


void GPIOF_Handler(void); // Interrupt handler prototype
int main(void) {
    // Initialize UART5 with a baud rate of 9600
	SysClockSet(16);
// Initialize the Red LED (PF1) as output
    GPIO_Init(GPIOF, LED_RED_PIN, GPIO_MODE_OUTPUT);
    GPIO_WritePin(GPIOF, LED_RED_PIN, 0); // Start with LED off

    // Initialize the button (PF0) as input with pull-up resistor
    Button_Init(GPIOF, BUTTON_PIN);

    // Enable interrupts for PF0 (button)
    GPIOF->IS &= ~(1U << BUTTON_PIN);  // Edge-sensitive
    GPIOF->IBE &= ~(1U << BUTTON_PIN); // Interrupt controlled by IEV
    GPIOF->IEV &= ~(1U << BUTTON_PIN); // Falling edge trigger
    GPIOF->IM |= (1U << BUTTON_PIN);   // Enable interrupt for PF0
    NVIC_EnableIRQ(GPIOF_IRQn);        // Enable interrupt in NVIC
    // Small delay to ensure UART5 is fully initialized

    while (1) {

    }
}
// GPIO Port F interrupt handler
void GPIOF_Handler(void) {
    if (GPIOF->MIS & (1U << BUTTON_PIN)) { // Check if interrupt occurred on PF0
        GPIOF->ICR |= (1U << BUTTON_PIN); // Clear the interrupt flag
        GPIOF->DATA ^= (1U << LED_RED_PIN); // Toggle the Red LED
    }
}
