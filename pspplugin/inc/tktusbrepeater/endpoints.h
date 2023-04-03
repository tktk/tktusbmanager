#ifndef TKTUSBREPEATER_ENDPOINTS_H
#define TKTUSBREPEATER_ENDPOINTS_H

#include "tktusbrepeater/endpoint.h"
#include <stddef.h>
#include <pspkerneltypes.h>

typedef struct
{
    SceUID              endpointsId;
    size_t              endpointsCount;
    TktUsbEndpoint *    endpoints;
} TktUsbEndpoints;

#endif  // TKTUSBREPEATER_ENDPOINTS_H
