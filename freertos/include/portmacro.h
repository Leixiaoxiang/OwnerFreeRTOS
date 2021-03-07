#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"
#include "stddef.h"

/*数据类型的重定义*/
#define portCHAR    char
#define portFLOAT   float
#define portDOUBLE  double
#define portLONG    long
#define portSHORT   short
#define portSTACK_TYPE  uint32_t
#define portBASE_TYPE   long

typedef portSTACK_TYPE StakType_t;
typedef portBASE_TYPE    BaseType_t;
typedef unsigned portBASE_TYPE UBaseType_t;

#if ( configUSE_16_BIT_TICKS == 1)
    typedef uint16_t TickType_t;
    #define portMAX_DELAY (TickType_t)0xFFFF
#else
    typedef uint32_t TickType_t;
    #define portMAX_DELAY (TickType_t)0xFFFFFFFFUL
#endif

#define portNVIC_INT_CTRL_REG   (*((volatile unint32_t) 0xE000ED04))
#define portNVIC_PENDSVSET_BIT  (1UL << 28UL)
#define portSY_FULL_READ_WRITE  (15)
#define portYIELD() \
{\
    portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;\
    _dsb(portSY_FULL_READ_WRITE);   \
    _isb(portSY_FULL_READ_WRITE);   \
}\

#endif
