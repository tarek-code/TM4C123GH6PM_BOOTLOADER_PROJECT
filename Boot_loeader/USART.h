#ifndef USART_H
#define USART_H

#include "TM4C123.h"
#include <stdint.h>
// Assuming you are using a Tiva C series or a similar ARM Cortex-M4 MCU.


// UART module enumeration
typedef enum {
    UART_MODULE_0,
    UART_MODULE_1,
    UART_MODULE_2,
    UART_MODULE_3,
    UART_MODULE_4,
    UART_MODULE_5,
    UART_MODULE_6,
    UART_MODULE_7
} UART_Module;

// Function prototypes
void UART_Init(UART_Module uart_module, uint32_t baud_rate);
void UART_WriteChar(UART_Module uart_module, char c);
void UART_WriteString2(UART_Module uart_module, const unsigned char *str);
void UART_WriteString(UART_Module uart_module, const unsigned char *str, uint8_t length);
void UART_ReadString(UART_Module uart_module, unsigned char  *buffer, uint32_t length);
char UART_ReadChar(UART_Module uart_module);
void Delay(unsigned long counter);









/* Exambels 

//////////////////////// recive char and send it //////////////////////////////////// 
char received_char = UART_ReadChar(UART_MODULE_0);
UART_WriteChar(UART_MODULE_0, received_char); // Echo the received character back
////////////////////////////////////////////////////////////////////////////////////
************************************************************************************************
/////////////////////////recive string and send it again////////////////////////////
char buffer[100];
UART_ReadString(UART_MODULE_0, buffer, 100, '\n');
UART_WriteString(UART_MODULE_0, buffer); // Echo the received string back
////////////////////////////////////////////////////////////////////////////////////


*/
#endif // USART_H
