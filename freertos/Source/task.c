#include "task.h"

extern TCB_t Task1TCB;
extern TCB_t Task2TCB;

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
        pxNewTCB->pxStack = (StakType_t * )puxStackBuffer;/*将设置的stack区域赋值到TCB中*/

        prvInitialiseNewTask(pxTaskCode,
                            pcName,
                            ulStackDepth,
                            pvParameters,
                            &xReturn,   /*句柄*/
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

    pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);    /*设置到栈顶*/
    /*浮点运算和移植需要考虑到8Byte*/
    pxTopOfStack = (StakType_t *)((uint32_t)pxTopOfStack & (~((uint32_t)0x07)));

    for (x = (UBaseType_t)0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; x++)/*最长的任务名长度为configMAX_TASK_NAME_LEN*/
    {
        pxNewTCB->pcTaskName[x] = pcName[x];
        if(pcName[x] == 0x00)
        {
            break;
        }
    }

    pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN - 1] = '\0';

    vListInitialiseItem(&(pxNewTCB->xStateListItem));   /*未插入链表之前赋值为NULL*/

    listSET_LIST_ITEM_OWNER(&(pxNewTCB->xStateListItem),pxNewTCB);  /*将TCB于他自身包含的链表项包含起来，在后面就可从链表项知道是哪个TCB*/

    pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack,pxTaskCode,pvParameters);

    if((void *)pxCreateTask != NULL)
    {
        *pxCreateTask = (TaskHandle_t)pxNewTCB; /*将TCB变成句柄返回*/
    }
    
}

void prvInitialiseTaskLists(void)
{
    UBaseType_t uxPriority;
    for(uxPriority = (UBaseType_t)0UL;
        uxPriority < (UBaseType_t)configMAX_PRIORITIES; /*每个优先级都有对应点的链表*/
        uxPriority++)
    {
        vListInitialise(&(pxReadyTaskLists[uxPriority]));   /*对每个链表都进行初始化*/
    }
}



void vTaskStartScheduler(void)
{
    pxCurrentTCB = &Task1TCB;

    if(xPortStartScheduler() != pdFAIL)
    {

    }
}

void vTaskSwitchContext(void)
{
    if(pxCurrentTCB == &Task1TCB)
    {
        pxCurrentTCB = &Task2TCB;
    }
    else
    {
        pxCurrentTCB = &Task1TCB;
    }
}
