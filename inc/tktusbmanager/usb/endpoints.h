#ifndef TKTUSBMANAGER_USB_ENDPOINTS_H
#define TKTUSBMANAGER_USB_ENDPOINTS_H

#include "tktusbmanager/def/usb/endpoints.h"
#include "tktusbmanager/usb/endpoint.h"
#include "tktusbmanager/def/usb/driver.h"
#include <stddef.h>

enum {
    MAX_USER_ENDPOINTS_COUNT = 8,
};

typedef struct TktUsbEndpoints
{
    size_t          endpointsCount;
    TktUsbEndpoint  endpoints[ MAX_USER_ENDPOINTS_COUNT ];
} TktUsbEndpoints;

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
