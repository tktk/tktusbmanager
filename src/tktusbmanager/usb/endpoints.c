#include "tktusbmanager/usb/endpoints.h"
#include "tktusbmanager/usb/endpoint.h"
#include "tktusbmanager/usb/driver.h"
#include <stddef.h>

void initializeTktUsbEndpoints(
    TktUsbEndpoints *   _endpoints
    , TktUsbDriver *    _driver
)
{
    const size_t    ENDPOINT_COUNT = _endpoints->endpointsCount;

    struct UsbEndpoint *    userUsbEndpoints = _driver->usbDriver.endp + 1;

    size_t  i;
    for( i = 0 ; i < ENDPOINT_COUNT ; i++ ) {
        TktUsbEndpoint *        endpoint = _endpoints->endpoints + i;
        struct UsbEndpoint *    userUsbEndpoint = userUsbEndpoints + i;

        setUpTktUsbEndpoint(
            endpoint
            , userUsbEndpoint
        );
    }
}

TktUsbEndpoint * getEndpointTktUsbEndpoints(
    TktUsbEndpoints *   _endpoints
    , const char *      _NAME
    , size_t            _NAME_SIZE
)
{
    size_t  i;
    for( i = 0 ; i < _endpoints->endpointsCount ; i++ ) {
        TktUsbEndpoint *    endpoint = _endpoints->endpoints + i;

        if( equalsNameTktUsbEndpoint(
            endpoint
            , _NAME
            , _NAME_SIZE
        ) == 0 ) {
            return endpoint;
        }
    }

    return NULL;
}
