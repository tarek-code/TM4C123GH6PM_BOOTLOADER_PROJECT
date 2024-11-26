#ifndef BOOTLADER_H
#define BOOTLADER_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "USART.h"
#include "CRC.h"
#include "gpio.h"
#include "flash.h"

#define BL_DEBUG_UART 											UART_MODULE_3
#define BL_HOST_COMMUNICATION_UART 					UART_MODULE_5

#define BL_ENABLE_UART_DEBUG_MESSAGE

#define BL_MAX_HOST_BUFFER 									200

#define CBL_GET_VER_CMD											0X10
#define CBL_GET_HELP_CMD										0X11
#define CBL_GET_CID_CMD											0X12
#define CBL_GO_TO_ADDR_CMD									0X14
#define CBL_FLASH_ERASE_CMD									0X15
#define CBL_MEM_WRITE_CMD										0X16


#define CBL_VENDOR_ID                100
#define CBL_SW_MAJOR_VERSION         1
#define CBL_SW_MINOR_VERSION         1
#define CBL_SW_PATCH_VERSION         0

#define CRC_TYPE_SIZE_BYTE           4

#define CRC_VERIFICATION_FAILED      0x00
#define CRC_VERIFICATION_PASSED      0x01

#define CBL_SEND_NACK                0xAB
#define CBL_SEND_ACK                 0xCD

#define FLASH_SECTOR12_BASE_ADDRESS   0x3000U

#define TM4C123GH6PM_SRAM_SIZE       	 (32 * 1024)
#define TM4C123GH6PM_FLASH_SIZE        (256 * 1024)
#define TM4C123GH6PM_EEPROM_SIZE       (2 * 1024)
#define FLASH_BASE 										 (0x00000000)
#define SRAM_BASE 										 (0x20000000)
#define EEPROM_BASE_END 							 (0x400AF000)
#define TM4C123GH6PM_SRAM_END          ( SRAM_BASE+ TM4C123GH6PM_SRAM_SIZE)
#define TM4C123GH6PM_FLASH_END         (FLASH_BASE + TM4C123GH6PM_FLASH_SIZE)
#define TM4C123GH6PM_EEPROM_END        (EEPROM_BASE + TM4C123GH6PM_EEPROM_SIZE)

#define ADDRESS_IS_INVALID           0x00
#define ADDRESS_IS_VALID             0x01
#define ADDRESS_IS_SPECTISL					 0x02

#define CBL_FLASH_MAX_SECTOR_NUMBER  255              /*Total Sectors: 256
																											Maximum Sector Number: 255
																										Sector Size: 1 KB*/
#define CBL_FLASH_MASS_ERASE         0xFF   

#define INVALID_SECTOR_NUMBER        0x00
#define VALID_SECTOR_NUMBER          0x01
#define UNSUCCESSFUL_ERASE           0x02
#define SUCCESSFUL_ERASE             0x03

#define FLASH_PAYLOAD_WRITE_FAILED   0x00
#define FLASH_PAYLOAD_WRITE_PASSED   0x01



typedef enum{
BL_NOK =0,
	BL_OK
}BL_Statuse;
typedef void (*pMainApp)(void);
typedef void (*Jump_Ptr)(void);
BL_Statuse BL_UART_Fetch_Host_Command(void);
 void BL_Print_Messege(char *format, ...); // variadic function 

#endif // BOOTLADER_H