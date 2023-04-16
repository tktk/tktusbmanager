#ifndef TKTUSBMANAGER_CONFIG_CONFIG_H
#define TKTUSBMANAGER_CONFIG_CONFIG_H

#include "tktusbmanager/config/endpoint.h"
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    SceUID                          endpointsId;
    size_t                          endpointsCount;
    TktUsbManagerConfigEndpoint *   endpoints;
} TktUsbManagerConfig;

int loadConfigFile(
    TktUsbManagerConfig *
    , const char *
);

void unloadConfig(
    TktUsbManagerConfig *
);

#endif  // TKTUSBMANAGER_CONFIG_CONFIG_H
