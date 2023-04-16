#include "tktusbmanager/common/parson.h"
#include "tktusbmanager/common/memory.h"
#include "parson/parson.h"

void initializeParson(
)
{
    json_set_allocation_functions(
        allocMemory
        , freeMemory
        , getMemoryAddress
    );
}
