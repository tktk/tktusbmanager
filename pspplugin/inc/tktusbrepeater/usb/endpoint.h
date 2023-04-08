#ifndef TKTUSBREPEATER_USB_ENDPOINT_H
#define TKTUSBREPEATER_USB_ENDPOINT_H

#include <pspusbbus.h>
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    SceUID  nameId;
    size_t  nameSize;
    char *  name;

    char    endpoint;

    struct UsbEndpoint *    usbEndpoint;
} TktUsbEndpoint;

int allocEndpoint(
    TktUsbEndpoint *
    , size_t
    , const char *
    , char
);

void freeEndpoint(
    TktUsbEndpoint *
);

#endif  // TKTUSBREPEATER_USB_ENDPOINT_H
