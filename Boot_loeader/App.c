#include "App.h"
int main(void) {
	SysClockSet(16);    
	 UART_Init(UART_MODULE_5, 9600);
	 UART_Init(UART_MODULE_3, 9600);

    while (1) {

        BL_UART_Fetch_Host_Command();
    }
}
