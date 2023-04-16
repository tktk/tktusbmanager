#ifndef TKTUSBMANAGER_COMMON_MEMORY_H
#define TKTUSBMANAGER_COMMON_MEMORY_H

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

#endif  // TKTUSBMANAGER_COMMON_MEMORY_H
