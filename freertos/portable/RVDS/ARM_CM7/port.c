#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "ARMCM7.H"



#define portINITIAL_XPSR        (0x01000000UL)
#define portSTART_ADDRESS_MASK  ((StakType_t) 0xFFFFFFFEUL)

#define protNVIC_SYSPRI2_REG    (*((volatile uint32_t *)0xE000ED20))
#define portNVIC_PENDSV_PRI     (((uint32_t) configKERNEL_INTERRRUPT_PRIORITY) << 16UL)
#define portNVIC_SYSTICK_PRI    (((uint32_t) configKERNEL_INTERRRUPT_PRIORITY) << 24UL)

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
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ((StakType_t)pxCode) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = (StakType_t)prvTaskExitError;
    pxTopOfStack -= 5;  /*R12\R3\R2\R1\R0*/
    *pxTopOfStack = (StakType_t)pvParameters;
    pxTopOfStack -= 8;
    return pxTopOfStack;
}

BaseType_t xPortStartScheduler(void)
{
    protNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
    protNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI;    

    prvStartFirstTask();

    return (BaseType_t)0UL;
}

__asm void prvStartFirstTask(void)
{
    PRESERVE8
    ldr r0, =0xE000ED08
    ldr r0, [r0]
    ldr r0, [r0]

    msr msp, r0

    cpsie i
    cpsie f
    dsb
    isb
    svc 0
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
    msr basepri, r0
    orr r14, #0xd
    bx r14
}

__asm void xPortPendSVHandler(void)
{
    extern pxCurrentTCB;
    extern vTaskSwitchContext;

    PRESERVE8

    mrs r0, psp
    isb

    ldr r3, =pxCurrentTCB;
    ldr r2, [r3]

    stmdb r0!, {r4-r11}
    str r0, [r2]

    stmdb sp!, {r3,r14}

    mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
    msr basepri, r0
    dsb
    isb
    bl vTaskSwitchContext
    mov r0, #0
    ldmia sp!, {r3,r14}
    ldr r1, [r3]
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp, r0
    isb
    bx r14
    nop
}






