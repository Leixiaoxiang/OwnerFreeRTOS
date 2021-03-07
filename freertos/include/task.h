#ifndef TASK_H
#define TASK_H

#include "FreeRTOS.h"

#define taskYIELD()       portYIELD()


typedef void * TaskHandle_t;

typedef struct tskTaskControlBlock
{
    volatile StakType_t *pxTopOfStack;
    ListItem xStateListItem;
    StakType_t *pxStack;
    char pcTaskName[configMAX_TASK_NAME_LEN];
}tskTCB;

typedef tskTCB TCB_t;

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
                                StakType_t * const puxStackBuffer,
                                TCB_t * const pxTaskBuffer
                            );

void prvInitialiseTaskLists(void);
void vTaskSwitchContext(void);

#endif
