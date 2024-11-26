
#include "sysclockcfg.h"

#define XTAL_FREQ_16MHZ 0x15    // Crystal frequency 16 MHz

void SysClockSet(uint32_t desiredMHz) {
    // Ensure the input frequency is within a safe range (1 to 400 MHz)
    if (desiredMHz < 1) {
        desiredMHz = 1;
    } else if (desiredMHz > 400) {
        desiredMHz = 80;  // Max supported system clock frequency is 80 MHz
    }

    // Bypass the PLL initially to configure the clock
    SYSCTL->RCC2 |= 0x80000000;      // USERCC2 bit set to use RCC2
    SYSCTL->RCC2 |= 0x00000800;      // Bypass PLL while configuring
    SYSCTL->RCC = (SYSCTL->RCC & ~0x7C0) + (XTAL_FREQ_16MHZ << 6); // Set crystal value and main oscillator
    SYSCTL->RCC2 &= ~0x70;           // Clear oscillator source bits
    SYSCTL->RCC2 |= 0x0;             // Use main oscillator as system clock

    // Configure based on the desired frequency
    if (desiredMHz < 16) {
        // For frequencies below 16 MHz, use the oscillator directly, bypassing the PLL
        SYSCTL->RCC2 |= 0x00000800;  // Bypass PLL
        uint32_t divisor = 400 / desiredMHz;  // Calculate divisor for direct frequencies
        SYSCTL->RCC2 = (SYSCTL->RCC2 & ~0x1FC00000) + ((divisor - 1) << 22);
    } else {
        // For frequencies 16 MHz and above, use the PLL
        SYSCTL->RCC2 &= ~0x00002000;   // Clear PWRDN2 to activate PLL
        SYSCTL->RCC2 |= 0x40000000;    // Set DIV400 to use 400 MHz PLL output

        // Calculate divisor to achieve the target frequency
        uint32_t pllDiv = (400 / desiredMHz) - 1;
        SYSCTL->RCC2 = (SYSCTL->RCC2 & ~0x1FC00000) + (pllDiv << 22);

        // Wait for PLL to stabilize
        while ((SYSCTL->RIS & 0x40) == 0) {};  // Wait for PLL to lock

        // Enable the PLL by clearing BYPASS
        SYSCTL->RCC2 &= ~0x00000800;
    }
}
