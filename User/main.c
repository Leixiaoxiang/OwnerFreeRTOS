#include "FreeRTOS.h"
#include "task.h"

extern List_t pxReadyTaskLists[configMAX_PRIORITIES];


TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE      ((UBaseType_t)128) 
StakType_t Task1Stack[TASK1_STACK_SIZE];
TCB_t Task1TCB;

TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE      ((UBaseType_t)128) 
StakType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t Task2TCB;

portCHAR flag1;
portCHAR flag2;

void delay (uint32_t count)
{
	for(; count!=0; count--);
}

void Task1_Entry(void *p_arg)
{
    for(;;)
    {
        flag1 = 0;
        delay(100);
        flag1 = 1;
        delay(100);
        portYIELD();
    }
}

void Task2_Entry(void *p_arg)
{
    for(;;)
    {
        flag2 = 0;
        delay(100);
        flag2 = 1;
        delay(100);
        portYIELD();
    }
}

int main(void)
{
    /*初始化链表的数组*/
    prvInitialiseTaskLists();
    /*
        设置堆栈的位置 TCB与TCB的链表项连接起来 并将入口函数进栈
    */
    Task1_Handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry,
                                        (char *)"Task1",
                                        (uint32_t)TASK1_STACK_SIZE,
                                        (void *)NULL,
                                        Task1Stack,
                                        &Task1TCB);
    /*判断TCB创建是否成功*/
    /*将TCB中的链表项插入到链表数组中 */
    vListInsertEnd(&(pxReadyTaskLists[1]),&(Task1TCB.xStateListItem));

    Task2_Handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry,
                                        (char *)"Task2",
                                        (uint32_t)TASK2_STACK_SIZE,
                                        (void *)NULL,
                                        Task2Stack,
                                        &Task2TCB);

    vListInsertEnd(&(pxReadyTaskLists[2]),&(Task2TCB.xStateListItem));

    vTaskStartScheduler();

    for (;;)
    {
        /* code */
    }
    
}
