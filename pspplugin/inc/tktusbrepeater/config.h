#ifndef TKTUSBREPEATER_CONFIG_H
#define TKTUSBREPEATER_CONFIG_H

#include "tktusbrepeater/endpoints.h"
#include <pspkerneltypes.h>

typedef struct
{
    TktUsbEndpoints endpoints;
} TktUsbRepeaterConfig;

int loadConfigFile(
    SceUID *
    , TktUsbRepeaterConfig **
    , const char *
);

void unloadConfig(
    SceUID *
    , TktUsbRepeaterConfig **
);

#endif  // TKTUSBREPEATER_CONFIG_H
