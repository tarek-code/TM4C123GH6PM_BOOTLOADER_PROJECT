// sysclockcfg.h
#ifndef SYSCLOCKCFG_H
#define SYSCLOCKCFG_H

#include <stdint.h>
#include "TM4C123GH6PM.h"
// Define system control register addresses if not defined in TM4C123GH6PM.h
#ifndef SYSCTL_RCC_R
#define SYSCTL_RCC_R           (*((volatile uint32_t *)0x400FE060))
#endif

#ifndef SYSCTL_RCC2_R
#define SYSCTL_RCC2_R          (*((volatile uint32_t *)0x400FE070))
#endif

#ifndef SYSCTL_RIS_R
#define SYSCTL_RIS_R           (*((volatile uint32_t *)0x400FE050))
#define SYSCTL_RIS_PLLLRIS     0x00000040  // PLL Lock Raw Interrupt Status
#endif

// Rest of your sysclockcfg.c code follows
// Function prototypes
void SysClockSet(uint32_t desiredMHz);
uint32_t SysClockGet(void);

#endif // SYSCLOCKCFG_H
