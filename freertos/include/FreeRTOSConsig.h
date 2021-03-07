#ifndef FREERTOSCONSIG_H
#define FREERTOSCONSIG_H

#define configUSE_16_BIT_TICKS          0UL
#define configMAX_TASK_NAME_LEN         16UL
#define configSUPPORT_STATIC_ALLOCATION 1UL
#define configMAX_PRIORITIES            5UL
#define configKERNEL_INTERRRUPT_PRIORITY 255UL
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191UL

#define xPortPendSVHandler              PendSV_Handler
#define xPortSysTickHandler             SysTick_Handler
#define xPortSVCHandler                 SVC_Handler

#endif
