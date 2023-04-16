#ifndef TKTUSBMANAGER_USB_ENDPOINTS_H
#define TKTUSBMANAGER_USB_ENDPOINTS_H

#include "tktusbmanager/def/usb/endpoints.h"
#include "tktusbmanager/def/usb/endpoint.h"
#include "tktusbmanager/def/usb/driver.h"
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

TktUsbEndpoint * getEndpointTktUsbEndpoints(
    TktUsbEndpoints *
    , const char *
    , size_t
);

#endif  // TKTUSBMANAGER_USB_ENDPOINTS_H
