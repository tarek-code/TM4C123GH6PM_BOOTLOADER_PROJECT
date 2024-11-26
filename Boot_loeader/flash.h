#ifndef FLASH_H
#define FLASH_H

#include "TM4C123.h"

// Flash Memory Controller Registers
#define FLASH_FMA      (*((volatile uint32_t *)0x400FD000))  // Flash Memory Address
#define FLASH_FMD      (*((volatile uint32_t *)0x400FD004))  // Flash Memory Data
#define FLASH_FMC      (*((volatile uint32_t *)0x400FD008))  // Flash Memory Control
#define FLASH_FMC2     (*((volatile uint32_t *)0x400FD020))  // Flash Memory Control 2
#define FLASH_FCRIS    (*((volatile uint32_t *)0x400FD00C))  // Flash Raw Interrupt Status

// Flash Memory Control Register Bits
#define FLASH_FMC_WRKEY    0xA4420000  // Write Key
#define FLASH_FMC_ERASE    0x00000002  // Page Erase
#define FLASH_FMC_MERASE   0x00000004  // Mass Erase
#define FLASH_FMC_BUSY     0x00000001  // Busy Flag
#define FLASH_FMC_WRITE  0x00000001  // Write operation

// Flash Sector Size (1 KB)
#define FLASH_SECTOR_SIZE  0x400

typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR
} Flash_Status;
                                                   // Start Address=Start Sector×Sector Size
// Function prototypes
void Flash_Unlock(void);
void Flash_MassErase(void);
void Flash_EraseSectors(uint32_t startSector, uint32_t numSectors);
void Flash_Lock(void);
Flash_Status HAL_FLASH_Program(uint32_t Address, uint8_t Data);
#endif // FLASH_H
