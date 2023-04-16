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

    SceUID  eventFlagId;

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

int equalsNameTktUsbEndpoint(
    const TktUsbEndpoint *
    , const char *
    , size_t
);

int isReadableTktUsbEndpoint(
    const TktUsbEndpoint *
);

int isWritableTktUsbEndpoint(
    const TktUsbEndpoint *
);

int readTktUsbEndpoint(
    TktUsbEndpoint *
    , void *
    , int
);

int writeTktUsbEndpoint(
    TktUsbEndpoint *
    , void *
    , int
);

#endif  // TKTUSBMANAGER_USB_ENDPOINT_H
