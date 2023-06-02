#ifndef TKTUSBMANAGER_DEF_USB_DRIVER_H
#define TKTUSBMANAGER_DEF_USB_DRIVER_H

#include "tktusbmanager/def/usb/endpoints.h"

typedef struct TktUsbDriver TktUsbDriver;

enum {
    MAX_ENDPOINTS_COUNT = MAX_USER_ENDPOINTS_COUNT + 1,

    STRING_DESCRIPTOR_SIZE = 8 * sizeof( unsigned char ),
};

#endif  // TKTUSBMANAGER_DEF_USB_DRIVER_H
