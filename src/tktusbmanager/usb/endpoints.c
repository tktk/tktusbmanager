#include "tktusbmanager/usb/endpoints.h"
#include "tktusbmanager/usb/endpoint.h"
#include "tktusbmanager/usb/driver.h"
#include "tktusbmanager/common/memory.h"
#include <pspkerneltypes.h>
#include <stddef.h>

int allocTktUsbEndpoints(
    TktUsbEndpoints *   _endpoints
    , size_t            _ENDPOINTS_COUNT
)
{
    SceUID  endpointsId = allocMemory( _ENDPOINTS_COUNT * sizeof( *( _endpoints->endpoints ) ) );
    if( endpointsId < 0 ) {
        return 1;
    }

    TktUsbEndpoint *    endpoints = ( TktUsbEndpoint * )getMemoryAddress( endpointsId );

    _endpoints->endpointsId = endpointsId;
    _endpoints->endpointsCount = _ENDPOINTS_COUNT;
    _endpoints->endpoints = endpoints;

    return 0;
}

void freeTktUsbEndpoints(
    TktUsbEndpoints *   _endpoints
)
{
    size_t  i;
    for( i = 0 ; i < _endpoints->endpointsCount ; i++ ) {
        TktUsbEndpoint *    endpoint = _endpoints->endpoints + i;

        freeTktUsbEndpoint( endpoint );
    }

    _endpoints->endpointsCount = 0;

    freeMemory( _endpoints->endpointsId );

    _endpoints->endpointsId = 0;
}

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
