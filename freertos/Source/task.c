#include "task.h"

#if (configSUPPORT_STATIC_ALLOCATION == 1UL)

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                                const char * const pcName,
                                const uint32_t ulStackDepth,
                                void * const pvParameters,
                                StakType_t * const puxStackBuffer,
                                TCB_t * const pxTaskBuffer
                            )
{
    
}



#endif
