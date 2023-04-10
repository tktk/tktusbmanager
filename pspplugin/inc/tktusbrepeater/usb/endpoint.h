#ifndef TKTUSBREPEATER_USB_ENDPOINT_H
#define TKTUSBREPEATER_USB_ENDPOINT_H

#include "tktusbrepeater/def/usb/endpoint.h"
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

#endif  // TKTUSBREPEATER_USB_ENDPOINT_H
