#ifndef CRC_H
#define CRC_H

#include <stdint.h>

#define CRC32_POLYNOMIAL 0x04C11DB7

// Initializes the CRC module
void CRC_Init(void);

// Calculates the CRC-32 for a given data buffer
//uint32_t CRC_Calculate(uint32_t *data, uint32_t length);
uint32_t Crc32(uint32_t ui32Crc, const uint32_t *pui8Data, uint32_t ui32Count);
#endif  // CRC_H
