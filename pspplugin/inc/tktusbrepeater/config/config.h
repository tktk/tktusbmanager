#ifndef TKTUSBREPEATER_CONFIG_CONFIG_H
#define TKTUSBREPEATER_CONFIG_CONFIG_H

#include "tktusbrepeater/config/endpoint.h"
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    SceUID                          endpointsId;
    size_t                          endpointsCount;
    TktUsbRepeaterConfigEndpoint *  endpoints;
} TktUsbRepeaterConfig;

int loadConfigFile(
    TktUsbRepeaterConfig *
    , const char *
);

void unloadConfig(
    TktUsbRepeaterConfig *
);

#endif  // TKTUSBREPEATER_CONFIG_CONFIG_H
