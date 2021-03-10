#ifndef PORTABLE_H
#define PORTABLE_H

#include "portmacro.h"
#include "task.h"

StakType_t *pxPortInitialiseStack(StakType_t *pxTopOfStack,
                                    TaskFunction_t pxCode,
                                    void * pvParameters);
void portYIELD(void);

#endif
