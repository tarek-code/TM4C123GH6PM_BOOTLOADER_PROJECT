
# TM4C123GH6PM Bootloader Project

## Overview
This project implements a bootloader for the TM4C123GH6PM microcontroller. The system comprises three main components:

1. **Bootloader**: Facilitates the update of application firmware via UART communication.
2. **Application**: Demonstrates functionality of the loaded firmware.
3. **Host**: A Python script-based tool to send firmware updates to the bootloader.

## Features
- **Firmware Update via UART**: Allows seamless application updates without JTAG.
- **CRC Validation**: Ensures data integrity during firmware transfer.
- **FreeRTOS Integration**: Provides multitasking capabilities.
- **Modular Codebase**: Separate modules for GPIO, USART, and Flash memory management.

## Project Structure
```
TM4C123GH6PM_BOOTLOADER_PROJECT/
├── Host/
│   ├── Host.py                # Python script to send firmware updates
│   ├── Application.bin        # Sample application binary
│   ├── Bugged_App/            # Example of an incorrect application binary
│   └── Updated_App/           # Example of an updated application binary
├── Application/
│   ├── app.c, app.h           # Application source code
│   ├── FreeRTOS/              # FreeRTOS kernel files
│   └── ...                    # Other application dependencies
├── Boot_loeader/
│   ├── bootloader.c, bootloader.h  # Bootloader source code
│   ├── USART.c, USART.h            # UART communication module
│   ├── CRC.c, CRC.h                # CRC validation module
│   └── ...                         # Other bootloader dependencies
```

## Setup Instructions
1. **Hardware Requirements**:
   - TM4C123GH6PM Launchpad
   - USB-to-UART adapter (for firmware updates)

2. **Software Requirements**:
   - Keil uVision (for compiling firmware)
   - Python 3.x (for Host script)

3. **Steps**:
   - Compile the bootloader and flash it onto the microcontroller.
   - Use the Host script to send `Application.bin` to the bootloader.
   - Observe the application running on the microcontroller.

## Challenges Faced
- **UART Communication Reliability**: Debugging timing issues in UART.
- **CRC Implementation**: Ensuring robust data integrity checks.
- **Flash Memory Management**: Safeguarding bootloader sections during updates.

## Lessons Learned
- Gained hands-on experience with firmware development and validation.
- Enhanced problem-solving skills through debugging complex systems.
- Developed a deeper understanding of microcontroller peripherals and FreeRTOS.

## Contact
For any queries or collaborations, feel free to reach out!
