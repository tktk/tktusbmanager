#ifndef TKTUSBMANAGER_USB_DRIVER_H
#define TKTUSBMANAGER_USB_DRIVER_H

#include "tktusbmanager/def/usb/driver.h"
#include "tktusbmanager/def/usb/endpoints.h"
#include <pspusbbus.h>
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct TktUsbDriver
{
    struct UsbDriver    usbDriver;

    SceUID  memoryId;
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

void unregisterTktUsbDriver(
    TktUsbDriver *
);

int startTktUsbDriver(
    TktUsbDriver *
);

void stopTktUsbDriver(
    TktUsbDriver *
);

#endif  // TKTUSBMANAGER_USB_DRIVER_H
