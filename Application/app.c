#include "sysclockcfg.h"
#include "button.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"

#define LED_RED_PIN_ 1  // PF1 is Red LED
#define LED_BLUE_PIN_ 2  // PF1 is Red LED
#define LED_GREEN_PIN 3  // PF1 is Red LED

void vTask1(void *pvParameters) {
    while (1) {
        GPIO_WritePin(GPIOF, LED_RED_PIN_, 1);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(10000));       // Delay for 1 second
        GPIO_WritePin(GPIOF, LED_RED_PIN_, 0);  // Turn LED off
        vTaskDelay(pdMS_TO_TICKS(10000));       // Delay for 1 second
    }
}

void vTask2(void *pvParameters) {
    while (1) {
        GPIO_WritePin(GPIOF, LED_GREEN_PIN, 1);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(1000));       // Delay for 1 second
        GPIO_WritePin(GPIOF, LED_GREEN_PIN, 0);  // Turn LED off
        vTaskDelay(pdMS_TO_TICKS(1000));       // Delay for 1 second
    }
}


void vTask3(void *pvParameters) {
    while (1) {
        GPIO_WritePin(GPIOF, LED_BLUE_PIN_, 1);  // Turn LED on
        vTaskDelay(pdMS_TO_TICKS(100));       // Delay for 1 second
        GPIO_WritePin(GPIOF, LED_BLUE_PIN_, 0);  // Turn LED off
        vTaskDelay(pdMS_TO_TICKS(100));       // Delay for 1 second
    }
}


int main(void) {
    SysClockSet(16);  // Initialize system clock to 16 MHz
    GPIO_Init(GPIOF, LED_RED_PIN_, GPIO_MODE_OUTPUT);
	GPIO_Init(GPIOF, LED_GREEN_PIN, GPIO_MODE_OUTPUT);
	GPIO_Init(GPIOF, LED_BLUE_PIN_, GPIO_MODE_OUTPUT);
    GPIO_WritePin(GPIOF, LED_RED_PIN_, 0);  // Start with LED off
	GPIO_WritePin(GPIOF, LED_GREEN_PIN, 0);  // Start with LED off
	GPIO_WritePin(GPIOF, LED_BLUE_PIN_, 0);  // Start with LED off

    xTaskCreate(
        vTask1,       /* Task function. */
        "LED_Task",   /* Task name. */
        configMINIMAL_STACK_SIZE, /* Stack size. */
        NULL,         /* Task parameters. */
        1,            /* Task priority. */
        NULL          /* Task handle. */
    );
	
	 xTaskCreate(
        vTask2,       /* Task function. */
        "GREEN_Task",   /* Task name. */
        configMINIMAL_STACK_SIZE, /* Stack size. */
        NULL,         /* Task parameters. */
        2,            /* Task priority. */
        NULL          /* Task handle. */
    );
		
		 xTaskCreate(
        vTask3,       /* Task function. */
        "BLUE_Task",   /* Task name. */
        configMINIMAL_STACK_SIZE, /* Stack size. */
        NULL,         /* Task parameters. */
        3,            /* Task priority. */
        NULL          /* Task handle. */
    );

    vTaskStartScheduler();  // Start the FreeRTOS scheduler

    while (1) {
        // Main loop
    }
}
