#ifndef TKTUSBREPEATER_COMMON_MEMORY_H
#define TKTUSBREPEATER_COMMON_MEMORY_H

#include <pspkerneltypes.h>

SceUID allocMemory(
    SceSize
);

void freeMemory(
    SceUID
);

void * getMemoryAddress(
    SceUID
);

#endif  // TKTUSBREPEATER_COMMON_MEMORY_H
