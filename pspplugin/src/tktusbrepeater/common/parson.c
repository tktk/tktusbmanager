#include "tktusbrepeater/common/parson.h"
#include "tktusbrepeater/common/memory.h"
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
