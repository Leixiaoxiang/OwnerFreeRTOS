#include "task.h"

TCB_t *pxCurrentTCB = NULL;

static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
                                TaskHandle_t * const pxCreateTask,
                                TCB_t * pxNewTCB);

List_t pxReadyTaskLists[configMAX_PRIORITIES];



#if (configSUPPORT_STATIC_ALLOCATION == 1UL)

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
                                StakType_t * const puxStackBuffer,
                                TCB_t * const pxTaskBuffer
                            )
{
    TCB_t * pxNewTCB;
    TaskHandle_t xReturn;

    if((NULL != puxStackBuffer) && (NULL != pxTaskBuffer))
    {
        pxNewTCB = (TCB_t *)pxTaskBuffer;
        pxNewTCB->pxStack = (StakType_t * )puxStackBuffer;

        prvInitialiseNewTask(pxTaskCode,
                            pcName,
                            ulStackDepth,
                            pvParameters,
                            &xReturn,
                            pxNewTCB);

    }
    else
    {
        xReturn = NULL;
    }

    return xReturn;
}
#endif

static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
                                TaskHandle_t * const pxCreateTask,
                                TCB_t * pxNewTCB)
{
    StakType_t *pxTopOfStack;
    UBaseType_t x;

    pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);
    /*向下�?8字节对齐，现在是栈顶*/
    pxTopOfStack = (StakType_t *)((uint32_t)pxTopOfStack & (~((uint32_t)0x07)));

    for (x = (UBaseType_t)0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; x++)
    {
        pxNewTCB->pcTaskName[x] = pcName[x];
        if(pcName[x] == 0x00)
        {
            break;
        }
    }

    pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN - 1] = '\0';

    vListInitialiseItem(&(pxNewTCB->xStateListItem));

    listSET_LIST_ITEM_OWNER(&(pxNewTCB->xStateListItem),pxNewTCB);

    pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack,pxTaskCode,pvParameters);

    if((void *)pxCreateTask != NULL)
    {
        *pxCreateTask = (TaskHandle_t)pxNewTCB;
    }
    
}

void prvInitialiseTaskLists(void)
{
    UBaseType_t uxPriority;
    for(uxPriority = (UBaseType_t)0UL;
        uxPriority < (UBaseType_t)configMAX_PRIORITIES;
        uxPriority++)
    {
        vListInitialise(&(pxReadyTaskLists[uxPriority]));
    }
}



void vTaskStartScheduler(void)
{
    pxCurrentTCB = &Task1TCB;

    if(xPortStartScheduler() != pdFAIL)
    {

    }
}

