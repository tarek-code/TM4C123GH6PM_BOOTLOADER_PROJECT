#ifndef APP_H
#define APP_H

#include "sysclockcfg.h"
#include "button.h"
#include "gpio.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LONG_TIME         0xffff


SemaphoreHandle_t xSemaphore = NULL;

#endif // APP_H
