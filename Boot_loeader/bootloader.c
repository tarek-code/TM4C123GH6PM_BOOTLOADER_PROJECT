#include "bootloader.h"

static uint8_t BL_Host_Buffer[BL_MAX_HOST_BUFFER];
/* ----------------- Static Functions Decleration -----------------*/
static void Bootloader_Get_Version(uint8_t *Host_Buffer);
static void Bootloader_Get_Help(uint8_t *Host_Buffer);
static void Bootloader_Get_Chip_Identification_Number(uint8_t *Host_Buffer);
static void Bootloader_Jump_To_Address(uint8_t *Host_Buffer);
static void Bootloader_Erase_Flash(uint8_t *Host_Buffer);
static void Bootloader_Memory_Write(uint8_t *Host_Buffer);



/* Helper functions */
static uint8_t Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_Len, uint32_t Host_CRC);
static void Bootloader_Send_ACK(uint8_t Replay_Len);
static void Bootloader_Send_NACK(void);
static void Bootloader_Send_Data_To_Host(uint8_t *Host_Buffer, uint32_t Data_Len);
static uint8_t Host_Address_Verification(uint32_t Jump_Address);
static uint8_t Perform_Flash_Erase(uint8_t Sector_Numebr, uint8_t Number_Of_Sectors);
static void jump_to_user_app(void);
static uint8_t Flash_Memory_Write_Payload(uint8_t *Host_Payload, uint32_t Payload_Start_Address, uint16_t Payload_Len);



/* static local variables  */
static uint8_t Bootloader_Supported_CMDs[6] = {
    CBL_GET_VER_CMD,
    CBL_GET_HELP_CMD,
    CBL_GET_CID_CMD,
    CBL_GO_TO_ADDR_CMD,
    CBL_FLASH_ERASE_CMD,
    CBL_MEM_WRITE_CMD,
};







/************************************************************************************************/
BL_Statuse BL_UART_Fetch_Host_Command(void){
BL_Statuse state=BL_NOK;
	uint8_t Data_length=0;
	memset(BL_Host_Buffer,0,BL_MAX_HOST_BUFFER);
	UART_ReadString(BL_HOST_COMMUNICATION_UART,(uint8_t *)BL_Host_Buffer,1);
	Data_length=BL_Host_Buffer[0];
	UART_ReadString(BL_HOST_COMMUNICATION_UART,(uint8_t *)&BL_Host_Buffer[1],Data_length);
	switch(BL_Host_Buffer[1]){
		case CBL_GET_VER_CMD:
			BL_Print_Messege("CBL_GET_VER_CMD \n");
		Bootloader_Get_Version(BL_Host_Buffer);
		break;
		case CBL_GET_HELP_CMD:
			BL_Print_Messege("CBL_GET_HELP_CMD \n");
		Bootloader_Get_Help(BL_Host_Buffer);
		break;
		case CBL_GET_CID_CMD:
			BL_Print_Messege("CBL_GET_CID_CMD \n");
		Bootloader_Get_Chip_Identification_Number(BL_Host_Buffer);
		break;
		
		case CBL_GO_TO_ADDR_CMD:
			BL_Print_Messege("CBL_GO_TO_ADDR_CMD \n");
		Bootloader_Jump_To_Address(BL_Host_Buffer);
		break;
		case CBL_FLASH_ERASE_CMD:
			BL_Print_Messege("CBL_FLASH_ERASE_CMD \n");
		Bootloader_Erase_Flash(BL_Host_Buffer);
		break;
		case CBL_MEM_WRITE_CMD:
			BL_Print_Messege("CBL_MEM_WRITE_CMD \n");
		Bootloader_Memory_Write(BL_Host_Buffer);
		break;		
		default:
			
		BL_Print_Messege("Invalid Command selected \n");
		break;
	}
	return state;
}


static void Bootloader_Get_Version(uint8_t *Host_Buffer){
	uint8_t BL_Version[CRC_TYPE_SIZE_BYTE]={CBL_VENDOR_ID,CBL_SW_MAJOR_VERSION,CBL_SW_MINOR_VERSION,CBL_SW_PATCH_VERSION};
	uint16_t Host_CMD_Packet_Len=0;
	uint32_t Host_CRC32=0;
	Host_CMD_Packet_Len=Host_Buffer[0]+1;
	Host_CRC32=*((uint32_t *)((Host_Buffer+Host_CMD_Packet_Len)-CRC_TYPE_SIZE_BYTE));
	
	if(CRC_VERIFICATION_PASSED==Bootloader_CRC_Verify((uint8_t *)(&Host_Buffer[0]),Host_CMD_Packet_Len-CRC_TYPE_SIZE_BYTE,Host_CRC32)){
	
		Bootloader_Send_ACK(4);
		UART_WriteString(BL_HOST_COMMUNICATION_UART,BL_Version,4);
	}
	else{
	Bootloader_Send_NACK();
	}
}
static void Bootloader_Get_Help(uint8_t *Host_Buffer){
uint16_t Host_CMD_Packet_Len=0;
	uint32_t Host_CRC32=0;
	Host_CMD_Packet_Len=Host_Buffer[0]+1;
	Host_CRC32=*((uint32_t *)((Host_Buffer+Host_CMD_Packet_Len)-CRC_TYPE_SIZE_BYTE));
	if(CRC_VERIFICATION_PASSED==Bootloader_CRC_Verify((uint8_t *)(&Host_Buffer[0]),Host_CMD_Packet_Len-CRC_TYPE_SIZE_BYTE,Host_CRC32)){
		Bootloader_Send_ACK(6);
		UART_WriteString(BL_HOST_COMMUNICATION_UART,Bootloader_Supported_CMDs,6);
	}
	else{
	Bootloader_Send_NACK();
	}
	
}
static void Bootloader_Get_Chip_Identification_Number(uint8_t *Host_Buffer){
  uint16_t Host_CMD_Packet_Len=0;
	uint32_t Host_CRC32=0;
	Host_CMD_Packet_Len=Host_Buffer[0]+1;
	uint16_t Fam=0;
	Host_CRC32=*((uint32_t *)((Host_Buffer+Host_CMD_Packet_Len)-CRC_TYPE_SIZE_BYTE));
	if(CRC_VERIFICATION_PASSED==Bootloader_CRC_Verify((uint8_t *)(&Host_Buffer[0]),Host_CMD_Packet_Len-CRC_TYPE_SIZE_BYTE,Host_CRC32)){
		Fam=SYSCTL->DID1;
		Bootloader_Send_ACK(2);
		UART_WriteString(BL_HOST_COMMUNICATION_UART,(uint8_t *)(&Fam),2);
		//jump_to_user_app();	
	}
	else{
	Bootloader_Send_NACK();
	}
}
static void Bootloader_Jump_To_Address(uint8_t *Host_Buffer){
	uint16_t Host_CMD_Packet_Len=0;
	uint32_t Host_CRC32=0;
	uint8_t add_state=ADDRESS_IS_INVALID;
	uint32_t adrees=0;
	Host_CMD_Packet_Len=Host_Buffer[0]+1;
	Host_CRC32=*((uint32_t *)((Host_Buffer+Host_CMD_Packet_Len)-CRC_TYPE_SIZE_BYTE));
	
	if(CRC_VERIFICATION_PASSED==Bootloader_CRC_Verify((uint8_t *)(&Host_Buffer[0]),Host_CMD_Packet_Len-CRC_TYPE_SIZE_BYTE,Host_CRC32)){
		Bootloader_Send_ACK(1);
		adrees=*((uint32_t *)&Host_Buffer[2]);
		add_state=Host_Address_Verification(adrees);
		if(add_state==ADDRESS_IS_VALID){
		UART_WriteString(BL_HOST_COMMUNICATION_UART,(uint8_t *)&add_state,1);
			Jump_Ptr jumping=(Jump_Ptr) (adrees+1);
			jumping();
			
		}
		else if (add_state==ADDRESS_IS_SPECTISL){
		UART_WriteString(BL_HOST_COMMUNICATION_UART,(uint8_t *)&add_state,1);
			jump_to_user_app();
		}
		else{
		UART_WriteString(BL_HOST_COMMUNICATION_UART,(uint8_t *)&add_state,1);
		}
	}
	else{
	Bootloader_Send_NACK();
	}
}






static void Bootloader_Erase_Flash(uint8_t *Host_Buffer){
uint16_t Host_CMD_Packet_Len=0;
	uint32_t Host_CRC32=0;
	uint8_t erease_statuse=UNSUCCESSFUL_ERASE;
	Host_CMD_Packet_Len=Host_Buffer[0]+1;
	Host_CRC32=*((uint32_t *)((Host_Buffer+Host_CMD_Packet_Len)-CRC_TYPE_SIZE_BYTE));
	if(CRC_VERIFICATION_PASSED==Bootloader_CRC_Verify((uint8_t *)(&Host_Buffer[0]),Host_CMD_Packet_Len-CRC_TYPE_SIZE_BYTE,Host_CRC32)){
		Bootloader_Send_ACK(1);
	//	Flash_EraseSectors(12, 2);
		erease_statuse=Perform_Flash_Erase(Host_Buffer[2],Host_Buffer[3]);
		if(erease_statuse==SUCCESSFUL_ERASE){
		UART_WriteString(BL_HOST_COMMUNICATION_UART,&erease_statuse,1);
		}
		else{
		UART_WriteString(BL_HOST_COMMUNICATION_UART,&erease_statuse,1);
		}
		
		
	}
	else{
	Bootloader_Send_NACK();
	}
}
static void Bootloader_Memory_Write(uint8_t *Host_Buffer){
	uint16_t Host_CMD_Packet_Len=0;
	uint8_t write_state=FLASH_PAYLOAD_WRITE_FAILED;
	uint32_t Host_CRC32=0;
	uint8_t add_verefication=ADDRESS_IS_INVALID;
	Host_CMD_Packet_Len=Host_Buffer[0]+1;
	Host_CRC32=*((uint32_t *)((Host_Buffer+Host_CMD_Packet_Len)-CRC_TYPE_SIZE_BYTE));
	
	if(CRC_VERIFICATION_PASSED==Bootloader_CRC_Verify((uint8_t *)(&Host_Buffer[0]),Host_CMD_Packet_Len-CRC_TYPE_SIZE_BYTE,Host_CRC32)){
		Bootloader_Send_ACK(1);
		if(ADDRESS_IS_VALID==Host_Address_Verification(*((uint32_t *)&Host_Buffer[2])) || ADDRESS_IS_SPECTISL==Host_Address_Verification(*((uint32_t *)&Host_Buffer[2]))){
		write_state=Flash_Memory_Write_Payload(&Host_Buffer[7],*((uint32_t *)&Host_Buffer[2]),Host_Buffer[6]);
			if(write_state==FLASH_PAYLOAD_WRITE_PASSED){
		UART_WriteString(BL_HOST_COMMUNICATION_UART,&write_state,1);
		}
		else{
			write_state=FLASH_PAYLOAD_WRITE_FAILED;
		UART_WriteString(BL_HOST_COMMUNICATION_UART,&write_state,1);
		}
		}
		else{
		write_state=FLASH_PAYLOAD_WRITE_FAILED;
			UART_WriteString(BL_HOST_COMMUNICATION_UART,&write_state,1);
		}
		
		
	}
	else{
	Bootloader_Send_NACK();
	}
}
static uint8_t Flash_Memory_Write_Payload(uint8_t *Host_Payload, uint32_t Payload_Start_Address, uint16_t Payload_Len){
Flash_Status HAL_Status = FLASH_ERROR;
	uint8_t Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
	uint16_t Payload_Counter = 0;
	
	Flash_Unlock();

		for(Payload_Counter = 0; Payload_Counter < Payload_Len; Payload_Counter++){
			/* Program a byte at a specified address */
			HAL_Status = HAL_FLASH_Program(Payload_Start_Address + Payload_Counter, Host_Payload[Payload_Counter]);
			if(HAL_Status==FLASH_ERROR){
			Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_FAILED;
			break;		
			}
			else{
						Flash_Payload_Write_Status = FLASH_PAYLOAD_WRITE_PASSED;
			}
		}
	
	
	
		/* Locks the FLASH control register access */
		 Flash_Lock();
		
			
		
		
	
	
	return Flash_Payload_Write_Status;
}


static uint8_t Bootloader_CRC_Verify(uint8_t *pData, uint32_t Data_Len, uint32_t Host_CRC){
uint8_t crc_statuse=CRC_VERIFICATION_FAILED;
	uint32_t crc_calculated=0xFFFFFFFF;
	uint8_t Data_Counter=0;
	uint32_t Data_Buffer=0;
	
	for(Data_Counter=0;Data_Counter<Data_Len;Data_Counter++){
	Data_Buffer=(uint32_t)pData[Data_Counter];
	crc_calculated=Crc32(crc_calculated,&Data_Buffer,1);
	}
	
	if (crc_calculated==Host_CRC){
		crc_statuse=CRC_VERIFICATION_PASSED;
	}
	else{
	crc_statuse=CRC_VERIFICATION_FAILED;
	}
	return crc_statuse;
}




static void Bootloader_Send_ACK(uint8_t Replay_Len){
uint8_t ACK_Value[2]={0};
ACK_Value[0]=CBL_SEND_ACK;
ACK_Value[1]=Replay_Len;
UART_WriteString(BL_HOST_COMMUNICATION_UART,ACK_Value,2);
}
static void Bootloader_Send_NACK(void){
uint8_t ACK_Value=CBL_SEND_NACK;
	UART_WriteString(BL_HOST_COMMUNICATION_UART,&ACK_Value,1);
}



static uint8_t Host_Address_Verification(uint32_t Jump_Address){
	uint8_t add_state=ADDRESS_IS_INVALID;
	if(FLASH_SECTOR12_BASE_ADDRESS==Jump_Address){
		add_state=ADDRESS_IS_SPECTISL;
	}
	else if((Jump_Address>=FLASH_BASE && Jump_Address<=TM4C123GH6PM_FLASH_END)||(Jump_Address>=SRAM_BASE && Jump_Address<=TM4C123GH6PM_SRAM_END)||(Jump_Address>=EEPROM_BASE_END && Jump_Address<=TM4C123GH6PM_EEPROM_END)){
	add_state=ADDRESS_IS_VALID;
	}
	else{
	add_state=ADDRESS_IS_INVALID;
	}
return add_state;
}


static uint8_t Perform_Flash_Erase(uint8_t Sector_Numebr, uint8_t Number_Of_Sectors){
uint8_t earase_stase=UNSUCCESSFUL_ERASE;
	
if(Sector_Numebr == CBL_FLASH_MASS_ERASE){
	Flash_Unlock();
	Flash_MassErase();
	Flash_Lock();
	earase_stase=SUCCESSFUL_ERASE;
}
else{
		if((Sector_Numebr <=CBL_FLASH_MAX_SECTOR_NUMBER)&&((Sector_Numebr+Number_Of_Sectors)<=CBL_FLASH_MAX_SECTOR_NUMBER)){
		Flash_EraseSectors(Sector_Numebr,Number_Of_Sectors);
			earase_stase=SUCCESSFUL_ERASE;
		}
		else{
		earase_stase=UNSUCCESSFUL_ERASE;
		}
}
	return earase_stase;
}

 


void BL_Print_Messege(char *format, ...){
	 char Messege[100]={0};
	va_list args;
	va_start(args,format);
	vsprintf(Messege,format,args);
	#ifdef BL_ENABLE_UART_DEBUG_MESSAGE
	UART_WriteString(BL_DEBUG_UART, (uint8_t *)Messege,sizeof(Messege));
	#endif
	va_end(args);

}


static void jump_to_user_app(void){

uint32_t msp = *((volatile uint32_t *)FLASH_SECTOR12_BASE_ADDRESS);
    uint32_t reset_handler_add = *((volatile uint32_t *)(FLASH_SECTOR12_BASE_ADDRESS + 4));
    pMainApp reset_app = (pMainApp)reset_handler_add;

    __disable_irq();                  // Disable all interrupts
    SCB->VTOR = FLASH_SECTOR12_BASE_ADDRESS; // Set vector table to application
    __set_MSP(msp);                   // Set the MSP to the application's stack pointer
    __enable_irq();                   // Re-enable interrupts
    reset_app();
}