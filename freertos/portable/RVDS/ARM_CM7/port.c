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
    extern 
}








