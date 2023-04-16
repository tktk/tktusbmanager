#ifndef TKTUSBMANAGER_USB_ENDPOINT_H
#define TKTUSBMANAGER_USB_ENDPOINT_H

#include "tktusbmanager/def/usb/endpoint.h"
#include <pspusbbus.h>
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct TktUsbEndpoint
{
    SceUID  nameId;
    size_t  nameSize;
    char *  name;

    char    endpoint;

    struct UsbEndpoint *    usbEndpoint;
} TktUsbEndpoint;

int allocTktUsbEndpoint(
    TktUsbEndpoint *
    , size_t
    , const char *
    , char
);

void freeTktUsbEndpoint(
    TktUsbEndpoint *
);

void initializeTktUsbEndpoint(
    TktUsbEndpoint *
    , struct UsbEndpoint *
);

#endif  // TKTUSBMANAGER_USB_ENDPOINT_H
