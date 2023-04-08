#ifndef TKTUSBREPEATER_USB_ENDPOINTS_H
#define TKTUSBREPEATER_USB_ENDPOINTS_H

#include "tktusbrepeater/usb/endpoint.h"
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    SceUID              endpointsId;
    size_t              endpointsCount;
    TktUsbEndpoint *    endpoints;
} TktUsbEndpoints;

int allocTktUsbEndpoints(
    TktUsbEndpoints *
    , size_t
);

void freeTktUsbEndpoints(
    TktUsbEndpoints *
);

#endif  // TKTUSBREPEATER_USB_ENDPOINTS_H
