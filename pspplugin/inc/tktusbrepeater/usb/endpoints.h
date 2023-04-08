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

int allocEndpoints(
    TktUsbEndpoints *
    , size_t
);

void freeEndpoints(
    TktUsbEndpoints *
);

#endif  // TKTUSBREPEATER_USB_ENDPOINTS_H
