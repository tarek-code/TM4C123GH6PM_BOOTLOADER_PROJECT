#include "crc.h"

// CRC-32 (Ethernet, ZIP, etc.) polynomial: 0x04C11DB7


void CRC_Init(void) {
    // Initialize the CRC settings if needed
}
/*
uint32_t CRC_Calculate(uint32_t *data, uint32_t length) {
     uint32_t crc = 0xFFFFFFFF;

    for (uint32_t i = 0; i < length; i++) {
        crc ^= data[i]; // XOR directly with the 32-bit word
        
        for (uint8_t j = 0; j < 32; j++) { // Process all 32 bits
            if (crc & 0x80000000) {
                crc = (crc << 1) ^ CRC32_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }

    return ~crc;
}
*/



uint32_t Crc32(uint32_t ui32Crc, const uint32_t *pui8Data, uint32_t ui32Count) {
    // Process each 32-bit word in the data buffer
    for (uint32_t i = 0; i < ui32Count; i++) {
        ui32Crc ^= pui8Data[i];  // XOR the 32-bit word into the CRC value
        
        // Process each bit in the 32-bit word
        for (uint8_t j = 0; j < 32; j++) {
            if (ui32Crc & 0x80000000) {          // Check if the MSB is set
                ui32Crc = (ui32Crc << 1) ^ CRC32_POLYNOMIAL; // Apply the polynomial
            } else {
                ui32Crc <<= 1;                  // Shift left if no MSB
            }
        }
    }

    return ui32Crc; // Return the updated CRC value
}
