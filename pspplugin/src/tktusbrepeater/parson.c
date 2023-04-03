#include "tktusbrepeater/parson.h"
#include "parson/parson.h"
#include <pspsysmem.h>
#include <pspkerneltypes.h>

#define PARTITION_NAME_PARSON "parson"

enum {
    PARTITION_TYPE_USER = 2,
};

static SceUID alloc(
    const SceSize   _SIZE
)
{
    return sceKernelAllocPartitionMemory(
        PARTITION_TYPE_USER
        , PARTITION_NAME_PARSON
        , PSP_SMEM_Low
        , _SIZE
        , NULL
    );
}

void initializeParson(
)
{
    json_set_allocation_functions(
        alloc
        , sceKernelFreePartitionMemory
        , sceKernelGetBlockHeadAddr
    );
}
