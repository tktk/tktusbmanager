#include "tktusbrepeater/usb/endpoints.h"
#include "tktusbrepeater/usb/endpoint.h"
#include "tktusbrepeater/common/memory.h"
#include <pspkerneltypes.h>
#include <stddef.h>

int allocEndpoints(
    TktUsbEndpoints *   _endpoints
    , size_t            _ENDPOINTS_COUNT
)
{
    SceUID  endpointsId = allocMemory( _ENDPOINTS_COUNT * sizeof( *( _endpoints->endpoints ) ) );
    if( endpointsId < 0 ) {
        return 1;
    }

    TktUsbEndpoint *    endpoints = ( TktUsbEndpoint * )getMemoryAddress( endpointsId );

    size_t  i;
    for( i = 0 ; i < _ENDPOINTS_COUNT ; i++ ) {
        TktUsbEndpoint *    endpoint = endpoints + i;

        endpoint->nameId = 0;
    }

    _endpoints->endpointsId = endpointsId;
    _endpoints->endpointsCount = _ENDPOINTS_COUNT;
    _endpoints->endpoints = endpoints;

    return 0;
}

void freeEndpoints(
    TktUsbEndpoints *   _endpoints
)
{
    size_t  i;
    for( i = 0 ; i < _endpoints->endpointsCount ; i++ ) {
        TktUsbEndpoint *    endpoint = _endpoints->endpoints + i;

        freeEndpoint( endpoint );
    }

    _endpoints->endpointsCount = 0;

    freeMemory( _endpoints->endpointsId );

    _endpoints->endpointsId = 0;
}
