#include "sysclockcfg.h"
#include "button.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LONG_TIME         0xffff
#define LED_RED_PIN       1  // PF1 is Red LED

SemaphoreHandle_t xSemaphore = NULL;

// Task to toggle the LED
void vTask1(void *pvParameters) {
    while (1) {
        // Wait for the semaphore indefinitely
        if (xSemaphoreTake(xSemaphore, LONG_TIME) == pdTRUE) {
            // Execute when semaphore is taken
            GPIO_WritePin(GPIOF, LED_RED_PIN, 1);  // Turn LED on
            vTaskDelay(pdMS_TO_TICKS(100));       // Delay for 100 ms
            GPIO_WritePin(GPIOF, LED_RED_PIN, 0);  // Turn LED off
            vTaskDelay(pdMS_TO_TICKS(100));       // Delay for 100 ms
        }
    }
}

// Interrupt handler for SW1
void SW1_Handler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Give the semaphore from ISR
    xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);

    // Clear the interrupt flag
    GPIOF->ICR |= (1U << 4);

    // Request context switch if needed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

int main(void) {
    // Set up the system clock
    SysClockSet(16);

    // Initialize GPIO for LED
    GPIO_Init(GPIOF, LED_RED_PIN, GPIO_MODE_OUTPUT);
    GPIO_WritePin(GPIOF, LED_RED_PIN, 0);  // Start with LED off

    // Create the semaphore
    xSemaphore = xSemaphoreCreateBinary();
    if (xSemaphore == NULL) {
        // Handle error if semaphore creation failed
        while (1);
    }

    // Create the task
    xTaskCreate(
        vTask1,        // Task function
        "LED_Task",    // Task name
        configMINIMAL_STACK_SIZE,  // Stack size
        NULL,          // Task parameters
        1,             // Priority
        NULL           // Task handle
    );

    // Initialize SW1 interrupt
    Button_InitInterrupt(GPIOF, SW1_PIN, SW1_Handler);

    // Start the FreeRTOS scheduler
    vTaskStartScheduler();

    // Should never reach here
    while (1);
}
