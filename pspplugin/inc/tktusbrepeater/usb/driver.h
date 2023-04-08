#ifndef TKTUSBREPEATER_USB_DRIVER_H
#define TKTUSBREPEATER_USB_DRIVER_H

#include "tktusbrepeater/usb/endpoints.h"
#include <pspusbbus.h>
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    //TODO
} TktUsbDriver;

int allocTktUsbDriver(
    TktUsbDriver *
    , const char *
    , const TktUsbEndpoints *
);

void freeTktUsbDriver(
    TktUsbDriver *
);

int registerTktUsbDriver(
    TktUsbDriver *
);

int startTktUsbDriver(
    TktUsbDriver *
);

#endif  // TKTUSBREPEATER_USB_DRIVER_H
