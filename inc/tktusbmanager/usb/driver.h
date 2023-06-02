#ifndef TKTUSBMANAGER_USB_DRIVER_H
#define TKTUSBMANAGER_USB_DRIVER_H

#include "tktusbmanager/def/usb/driver.h"
#include "tktusbmanager/def/usb/endpoints.h"
#include <pspusbbus.h>
#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    unsigned char   devdesc[ 20 ];

    struct Config   config;
    struct ConfDesc confdesc;

    unsigned char   pad1[ 8 ];

    struct Interfaces   interfaces;
    struct InterDesc    interdesc;
} __attribute__( ( packed ) ) UsbData_;

typedef struct TktUsbDriver
{
    struct UsbDriver    usbDriver;

    struct UsbEndpoint  usbEndpoints[ MAX_ENDPOINTS_COUNT ];
    struct UsbInterface usbInterface;
    UsbData_            usbDataHi;
    struct Endp         usbEndpointDescriptorsHi[ MAX_ENDPOINTS_COUNT ];
    UsbData_            usbDataFull;
    struct Endp         usbEndpointDescriptorsFull[ MAX_ENDPOINTS_COUNT ];
    unsigned char       stringDescriptor[ STRING_DESCRIPTOR_SIZE ];
} TktUsbDriver;

int initializeTktUsbDriver(
    TktUsbDriver *
    , const char *
    , const TktUsbEndpoints *
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
