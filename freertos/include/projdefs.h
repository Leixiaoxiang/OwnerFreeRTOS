#ifndef PROJDEFS_H
#define PROJDEFS_H

#include "portable.h"

typedef void (*TaskFunction_t)(void *);

#define pdFALSE     ((UBaseType_t)0UL)
#define pdTRUE      ((UBaseType_t)0UL)

#define pdPASS      (pdFALSE)
#define pdFAIL      (pdTRUE)

#endif
