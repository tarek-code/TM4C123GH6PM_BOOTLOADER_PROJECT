#include "flash.h"

// Unlock the Flash Memory Controller
void Flash_Unlock(void) {
    FLASH_FMC2 = FLASH_FMC_WRKEY;  // Flash unlock sequence
}

// Perform a Mass Erase
void Flash_MassErase(void) {
    // Unlock the flash memory first
    Flash_Unlock();

    // Initiate the mass erase operation
    FLASH_FMC = FLASH_FMC_WRKEY | FLASH_FMC_MERASE;  // Write the mass erase command

    // Wait until the erase operation completes
    while (FLASH_FMC & FLASH_FMC_BUSY); // Wait for the BUSY flag to clear
}

// Erase a specified number of sectors starting from a given sector
void Flash_EraseSectors(uint32_t startSector, uint32_t numSectors) {
    // Calculate the starting address of the first sector
    uint32_t startAddress = startSector * FLASH_SECTOR_SIZE;

    // Unlock the flash memory first
    Flash_Unlock();

    // Loop through the number of sectors to erase
    for (uint32_t i = 0; i < numSectors; i++) {
        // Set the address for the sector to erase
        FLASH_FMA = startAddress;

        // Issue the page erase command
        FLASH_FMC = FLASH_FMC_WRKEY | FLASH_FMC_ERASE;

        // Wait until the erase operation completes
        while (FLASH_FMC & FLASH_FMC_BUSY); // Wait for the BUSY flag to clear

        // Increment to the next sector address
        startAddress += FLASH_SECTOR_SIZE;
    }

    // Lock the flash memory after the operation
    Flash_Lock();
}

// Lock the Flash Memory Controller
void Flash_Lock(void) {
    FLASH_FMC2 = 0x00000000;  // Lock the flash memory
}




// Define flash constants
#define FLASH_BASE        0x00000000
#define FLASH_SIZE        (256 * 1024)  // 256 KB Flash memory

Flash_Status HAL_FLASH_Program(uint32_t Address, uint8_t Data) {
    // Check if the address is within valid flash range
    if (Address < FLASH_BASE || Address >= FLASH_BASE + FLASH_SIZE) {
        return FLASH_ERROR;
    }

    // Calculate the aligned word address
    uint32_t wordAddress = Address & ~0x3;  // Align to the nearest 32-bit boundary
    uint32_t wordData = *((volatile uint32_t *)wordAddress);  // Read the current word

    // Modify the specific byte in the word
    uint8_t byteOffset = Address % 4;  // Determine which byte to update
    uint32_t mask = 0xFF << (byteOffset * 8);
    wordData = (wordData & ~mask) | ((uint32_t)Data << (byteOffset * 8));

    // Unlock the flash for programming
    Flash_Unlock();

    // Program the word back to flash memory
    FLASH_FMA = wordAddress;           // Set the target address
    FLASH_FMD = wordData;              // Set the data to program
    FLASH_FMC = FLASH_FMC_WRKEY | FLASH_FMC_WRITE;  // Start the programming operation

    // Wait for the programming to complete
    while (FLASH_FMC & FLASH_FMC_BUSY);

    // Lock the flash controller
    Flash_Lock();

    return FLASH_OK;
}
