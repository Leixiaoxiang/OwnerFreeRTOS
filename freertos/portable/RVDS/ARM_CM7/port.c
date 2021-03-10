#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "ARMCM7.H"



#define portINITIAL_XPSR        (0x01000000UL)
#define portSTART_ADDRESS_MASK  ((StakType_t) 0xFFFFFFFEUL)

#define protNVIC_SYSPRI2_REG    (*((volatile uint32_t *)0xE000ED20))
#define portNVIC_PENDSV_PRI     (((uint32_t) configKERNEL_INTERRRUPT_PRIORITY) << 16UL)
#define portNVIC_SYSTICK_PRI    (((uint32_t) configKERNEL_INTERRRUPT_PRIORITY) << 24UL)

#define portNVIC_INT_CTRL_REG   (*((volatile unint32_t) 0xE000ED04))
#define portNVIC_PENDSVSET_BIT  (1UL << 28UL)
#define portSY_FULL_READ_WRITE  (15)

void portYIELD(void)
{
    portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
    __dsb(portSY_FULL_READ_WRITE);
    __isb(portSY_FULL_READ_WRITE);
}

static void prvTaskExitError(void)
{
    for(;;)
    {
        /* error*/
    }
}

StakType_t *pxPortInitialiseStack(StakType_t *pxTopOfStack,
                                    TaskFunction_t pxCode,
                                    void * pvParameters)
{
    pxTopOfStack--;     /*链表顶是包含数据的直接减去*/
    *pxTopOfStack = portINITIAL_XPSR;   /*xPSR*/
    pxTopOfStack--;
    *pxTopOfStack = ((StakType_t)pxCode) & portSTART_ADDRESS_MASK;/*R15 入口函数*/
    pxTopOfStack--;
    *pxTopOfStack = (StakType_t)prvTaskExitError;/*R14 任务的返回地址*/
    pxTopOfStack -= 5;  /*R12\R3\R2\R1\R0*/
    *pxTopOfStack = (StakType_t)pvParameters;/*R0 任务的参数*/
    pxTopOfStack -= 8;  /*R11~R4*/
    return pxTopOfStack;
}

BaseType_t xPortStartScheduler(void)
{
    /*
        PENSV 和 SYSTICK 中断优先级的设置
        这两项的中断和任务的切换有关  应该小于其他系统的中断优先级
        所以这只优先级为最小    
    */
    protNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    protNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;    

    prvStartFirstTask();

    return (BaseType_t)0UL; /*中断被触发  不会运行到这里  所以返回值无所谓*/
}

__asm void prvStartFirstTask(void)
{
    PRESERVE8
    ldr r0, =0xE000ED08 /*SCB_VTOP的地址，获取MSP*/
    ldr r0, [r0]
    ldr r0, [r0]

    msr msp, r0

    cpsie i
    cpsie f
    dsb
    isb
    svc 0   /*调用0号系统服务*/
    nop
    nop
}

__asm void xPortSVCHandler(void)
{
    extern pxCurrentTCB;

    PRESERVE8

    ldr r3, =pxCurrentTCB
    ldr r1, [r3]
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp,r0
    isb
    mov r0, #0
    msr basepri, r0 /*打开所有的中断*/
    orr r14, #0xd   /*退出中断 并切换到PSP，从r0开始恢复上下文，切换到task1*/
    bx r14
}

__asm void xPortPendSVHandler(void)
{
    extern pxCurrentTCB;
    extern vTaskSwitchContext;

    PRESERVE8

    mrs r0, psp
    isb

    ldr r3, =pxCurrentTCB
    ldr r2, [r3]

    stmdb r0!, {r4-r11}
    str r0, [r2]

    stmdb sp!, {r3,r14} /*中断中使用的是主堆栈，MSP r3 保存的是当前正在执行的值，r14保存的是返回值的值 | 0x0d 返回到PSP不然会返回到MSP*/

    mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
    msr basepri, r0 /*关闭中断序号11 之后的中断*/
    dsb
    isb
    bl vTaskSwitchContext
    mov r0, #0
    msr basepri,r0
    ldmia sp!, {r3,r14}
    ldr r1, [r3]    /*存储当前的正在运行的值*/
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp, r0
    isb
    bx r14
    nop
}






