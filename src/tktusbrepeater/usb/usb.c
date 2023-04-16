#include "tktusbrepeater/usb/usb.h"
#include <psptypes.h>
#include <pspusb.h>

enum {
    USB_DRIVER_PID = 0x1c9,
};

int startUsbBus(
)
{
    return sceUsbStart(
        PSP_USBBUS_DRIVERNAME
        , 0
        , 0
    );
}

void stopUsbBus(
)
{
    sceUsbStop(
        PSP_USBBUS_DRIVERNAME
        , 0
        , 0
    );
}

int activateUsb(
)
{
    return sceUsbActivate( USB_DRIVER_PID );
}

void deactivateUsb(
)
{
    sceUsbDeactivate( USB_DRIVER_PID );
}
