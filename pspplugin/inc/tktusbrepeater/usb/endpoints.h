#ifndef TKTUSBREPEATER_USB_ENDPOINTS_H
#define TKTUSBREPEATER_USB_ENDPOINTS_H

#include "tktusbrepeater/def/usb/endpoints.h"
#include "tktusbrepeater/def/usb/endpoint.h"
#include "tktusbrepeater/def/usb/driver.h"
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct TktUsbEndpoints
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

void initializeTktUsbEndpoints(
    TktUsbEndpoints *
    , TktUsbDriver *
);

#endif  // TKTUSBREPEATER_USB_ENDPOINTS_H
