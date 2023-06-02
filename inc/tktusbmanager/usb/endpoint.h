#ifndef TKTUSBMANAGER_USB_ENDPOINT_H
#define TKTUSBMANAGER_USB_ENDPOINT_H

#include "tktusbmanager/def/usb/endpoint.h"
#include <pspusbbus.h>
#include <pspkerneltypes.h>
#include <stddef.h>

enum {
    MAX_ENDPOINT_NAME_SIZE = 20,
};

typedef struct TktUsbEndpoint
{
    size_t  nameSize;
    char    name[ MAX_ENDPOINT_NAME_SIZE ];

    char    endpoint;

    SceUID  eventFlagId;

    struct UsbEndpoint *    usbEndpoint;
} TktUsbEndpoint;

int initializeTktUsbEndpoint(
    TktUsbEndpoint *
    , size_t
    , const char *
    , char
);

void freeTktUsbEndpoint(
    TktUsbEndpoint *
);

void setUpTktUsbEndpoint(
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
    , const void *
    , int
);

#endif  // TKTUSBMANAGER_USB_ENDPOINT_H
