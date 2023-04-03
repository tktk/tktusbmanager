#ifndef TKTUSBREPEATER_CONFIG_H
#define TKTUSBREPEATER_CONFIG_H

#include "tktusbrepeater/endpoints.h"

typedef struct
{
    TktUsbEndpoints endpoints;
} TktUsbRepeaterConfig;

int loadConfigFile(
    TktUsbRepeaterConfig *
    , const char *
);

void unloadConfig(
    TktUsbRepeaterConfig *
);

#endif  // TKTUSBREPEATER_CONFIG_H
