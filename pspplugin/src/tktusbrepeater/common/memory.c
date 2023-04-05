#include "tktusbrepeater/common/memory.h"
#include <pspsysmem.h>
#include <pspkerneltypes.h>

#define PARTITION_NAME "tktusbrepeater"

enum {
    USER_PARTITION = 2,
};

SceUID allocMemory(
    SceSize _SIZE
)
{
    return sceKernelAllocPartitionMemory(
        USER_PARTITION
        , PARTITION_NAME
        , PSP_SMEM_Low
        , _SIZE
        , NULL
    );
}

void freeMemory(
    SceUID  _ID
)
{
    sceKernelFreePartitionMemory( _ID );
}

void * getMemoryAddress(
    SceUID  _ID
)
{
    return sceKernelGetBlockHeadAddr( _ID );
}
