#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware
 * and application requirements.
 *----------------------------------------------------------*/

/* Core configurations */
#define configCPU_CLOCK_HZ                        (16000000UL)   // 16 MHz system clock
#define configTICK_RATE_HZ                        (1000)         // Tick frequency (1 ms tick)
#define configMAX_PRIORITIES                      (5)            // Number of priorities (including idle)
#define configMINIMAL_STACK_SIZE                  (128)          // Minimal stack size in words
#define configTOTAL_HEAP_SIZE                     (24 * 1024)     // 8 KB heap for RTOS dynamic allocations
#define configMAX_TASK_NAME_LEN                   (16)           // Task name max length

/* System configurations */
#define configUSE_PREEMPTION                      1              // Enable preemptive scheduling
#define configUSE_IDLE_HOOK                       0              // Disable idle hook
#define configUSE_TICK_HOOK                       0              // Disable tick hook
#define configUSE_16_BIT_TICKS                    0              // Use 32-bit tick counters
#define configIDLE_SHOULD_YIELD                   1              // Idle task yields processor

/* Memory allocation */
#define configSUPPORT_STATIC_ALLOCATION           1              // Allow static memory allocation
#define configSUPPORT_DYNAMIC_ALLOCATION          1              // Allow dynamic memory allocation

/* Debugging and runtime stats */
#define configCHECK_FOR_STACK_OVERFLOW            2              // Enable stack overflow checking
#define configUSE_MALLOC_FAILED_HOOK              1              // Enable malloc failed hook
#define configGENERATE_RUN_TIME_STATS             0              // Disable runtime stats
#define configUSE_TRACE_FACILITY                  1              // Enable tracing (if needed)
#define configUSE_STATS_FORMATTING_FUNCTIONS      1              // Enable vTaskList and other debug functions

/* Interrupt and priority configurations */
#define configKERNEL_INTERRUPT_PRIORITY           (2)          // Lowest NVIC priority (0xFF)
#define configMAX_SYSCALL_INTERRUPT_PRIORITY      (5)          // Priority mask for FreeRTOS APIs (0xC0)
#define configPRIO_BITS                           3              // 3 priority bits in TM4C123GH6PM

/* Software timer configurations */
#define configUSE_TIMERS                          1              // Enable software timers
#define configTIMER_TASK_PRIORITY                 (3)            // Priority of the timer task
#define configTIMER_QUEUE_LENGTH                  (10)           // Timer queue length
#define configTIMER_TASK_STACK_DEPTH              (128)          // Timer task stack size

/* Task utilities */
#define INCLUDE_vTaskPrioritySet                  1
#define INCLUDE_uxTaskPriorityGet                 1
#define INCLUDE_vTaskDelete                       1
#define INCLUDE_vTaskSuspend                      1
#define INCLUDE_vTaskDelayUntil                   1
#define INCLUDE_vTaskDelay                        1
#define INCLUDE_xTaskGetSchedulerState            1

/* Hook functions */
#define vAssertCalled()    for( ;; )  // Infinite loop for assertion failures

/* Cortex-M4F specific settings */
#define configASSERT( x )  if( ( x ) == 0 ) { vAssertCalled(); }
#define configUSE_PORT_OPTIMISED_TASK_SELECTION   0

#define configUSE_MALLOC_FAILED_HOOK           1

#define configCHECK_FOR_STACK_OVERFLOW        2   // 1 or 2 for stack overflow detection
#define configUSE_TIMERS                       1

#define configUSE_MALLOC_FAILED_HOOK   1
#define configUSE_TIMERS               1
#define configUSE_TIMERS_HOOK          1

#define vPortSVCHandler    SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler



#endif /* FREERTOS_CONFIG_H */
