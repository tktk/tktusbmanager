#include "tktusbrepeater/usb/driver.h"
#include "tktusbrepeater/usb/endpoints.h"

int allocTktUsbDriver(
    TktUsbDriver *              _driver
    , const char *              _DRIVER_NAME
    , const TktUsbEndpoints *   _ENDPOINTS
)
{
    //TODO
    return 0;
}

void freeTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    //TODO
}

int registerTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    return sceUsbbdRegister( &( _driver->usbDriver ) );
}

void unregisterTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    sceUsbbdUnregister( &( _driver->usbDriver ) );
}

int startTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    //TODO
    return 0;
}

void stopTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    //TODO
}
