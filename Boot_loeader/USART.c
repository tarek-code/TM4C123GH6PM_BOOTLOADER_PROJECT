#include "USART.h"

// Helper function to get UART base pointer based on module
static UART0_Type* getUART(UART_Module uart_module) {
    switch (uart_module) {
        case UART_MODULE_0: return UART0;
        case UART_MODULE_1: return UART1;
        case UART_MODULE_2: return UART2;
        case UART_MODULE_3: return UART3;
        case UART_MODULE_4: return UART4;
        case UART_MODULE_5: return UART5;
        case UART_MODULE_6: return UART6;
        case UART_MODULE_7: return UART7;
        default: return UART0;  // Default to UART0 for safety
    }
}

// UART Initialization function
void UART_Init(UART_Module uart_module, uint32_t baud_rate) {
     UART0_Type *uart = getUART(uart_module);

    // Enable the clock for the UART module
    SYSCTL->RCGCUART |= (1U << uart_module); // Ensure clock for UART module is enabled
    Delay(1); // Small delay to stabilize the clock

    // Enable the clock for the corresponding GPIO port
    switch (uart_module) {
        case UART_MODULE_0:
            SYSCTL->RCGCGPIO |= (1U << 0); // Port A
            Delay(1);
            GPIOA->AFSEL |= (1U << 0) | (1U << 1); // Enable alternate functions on PA0, PA1
            GPIOA->PCTL = (GPIOA->PCTL & 0xFFFFFF00) | 0x00000011; // Set PA0, PA1 to UART
            GPIOA->DEN |= (1U << 0) | (1U << 1); // Digital enable
            break;
        case UART_MODULE_1:
            SYSCTL->RCGCGPIO |= (1U << 1); // Port B
            Delay(1);
            GPIOB->AFSEL |= (1U << 0) | (1U << 1);
            GPIOB->PCTL = (GPIOB->PCTL & 0xFFFFFF00) | 0x00000011;
            GPIOB->DEN |= (1U << 0) | (1U << 1);
            break;
        case UART_MODULE_2:
            SYSCTL->RCGCGPIO |= (1U << 3); // Port D
            Delay(1);
            GPIOD->AFSEL |= (1U << 6) | (1U << 7);
            GPIOD->PCTL = (GPIOD->PCTL & 0x00FFFFFF) | 0x11000000;
            GPIOD->DEN |= (1U << 6) | (1U << 7);
            break;
        case UART_MODULE_3:
            SYSCTL->RCGCGPIO |= (1U << 2); // Port C
            Delay(1);
            GPIOC->AFSEL |= (1U << 6) | (1U << 7);
            GPIOC->PCTL = (GPIOC->PCTL & 0x00FFFFFF) | 0x11000000;
            GPIOC->DEN |= (1U << 6) | (1U << 7);
            break;
        case UART_MODULE_4:
            SYSCTL->RCGCGPIO |= (1U << 2); // Port C
            Delay(1);
            GPIOC->AFSEL |= (1U << 4) | (1U << 5);
            GPIOC->PCTL = (GPIOC->PCTL & 0xFF00FFFF) | 0x00110000;
            GPIOC->DEN |= (1U << 4) | (1U << 5);
            break;
        case UART_MODULE_5:
            SYSCTL->RCGCGPIO |= (1U << 4); // Port E
            Delay(1);
            GPIOE->AFSEL |= (1U << 4) | (1U << 5);
            GPIOE->PCTL = (GPIOE->PCTL & 0xFF00FFFF) | 0x00110000;
            GPIOE->DEN |= (1U << 4) | (1U << 5);
            break;
        case UART_MODULE_6:
            SYSCTL->RCGCGPIO |= (1U << 3); // Port D
            Delay(1);
            GPIOD->AFSEL |= (1U << 4) | (1U << 5);
            GPIOD->PCTL = (GPIOD->PCTL & 0xFF00FFFF) | 0x00110000;
            GPIOD->DEN |= (1U << 4) | (1U << 5);
            break;
        case UART_MODULE_7:
            SYSCTL->RCGCGPIO |= (1U << 4); // Port E
            Delay(1);
            GPIOE->AFSEL |= (1U << 0) | (1U << 1);
            GPIOE->PCTL = (GPIOE->PCTL & 0xFFFFFF00) | 0x00000011;
            GPIOE->DEN |= (1U << 0) | (1U << 1);
            break;
    }

    // Disable UART for configuration
    uart->CTL &= ~0x01;

    // Calculate baud rate divisor for 16 MHz clock
    uint32_t brd = 16000000 / (16 * baud_rate);
    uart->IBRD = brd; // Integer part
    uart->FBRD = (16000000 % (16 * baud_rate)) * 64 / (16 * baud_rate); // Fractional part

    uart->LCRH = (0x3 << 5); // 8-bit data, no parity, 1 stop bit
    uart->CC = 0x0; // Use system clock

    // Enable UART, Tx, and Rx
    uart->CTL |= 0x301;
}

// Function to send a single character
void UART_WriteChar(UART_Module uart_module, char c) {
    UART0_Type *uart = getUART(uart_module);
    while (uart->FR & 0x20) {
        // Wait until the transmit FIFO is not full
    }
    uart->DR = c; // Transmit the character
}

// Function to send a string

void UART_WriteString2(UART_Module uart_module, const unsigned char *str) {
    while (*str) {
        UART_WriteChar(uart_module, *str++);
    }
}

void UART_WriteString(UART_Module uart_module, const unsigned char *str, uint8_t length) {
    for (uint32_t i = 0; i < length; i++) {
        UART_WriteChar(uart_module, str[i]);
    }
}



// Function to read a single character
char UART_ReadChar(UART_Module uart_module) {
    UART0_Type *uart = getUART(uart_module);
    while (uart->FR & 0x10) {
        // Wait until the receive FIFO is not empty
    }
    return (char)(uart->DR & 0xFF); // Read and return the received character
}

// Function to read a string until a termination character (e.g., '\n') or buffer limit
void UART_ReadString(UART_Module uart_module,unsigned char *buffer, uint32_t length) {
   for (uint32_t i = 0; i < length; i++) {
        buffer[i] = UART_ReadChar(uart_module); // Read each byte
    }
}



// Simple delay function
void Delay(unsigned long counter) {
    volatile unsigned long i = 0;
    for (i = 0; i < counter * 1000; i++); // Adjusted to approximate a delay
}
