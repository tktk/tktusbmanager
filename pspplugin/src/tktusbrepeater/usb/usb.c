#include "tktusbrepeater/usb/usb.h"
#include <psptypes.h>
#include <pspusb.h>

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
    //TODO
}

int activateUsb(
)
{
    //TODO
    return 0;
}

void deactivateUsb(
)
{
    //TODO
}
