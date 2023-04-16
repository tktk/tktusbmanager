#include "tktusbmanager/usb/endpoint.h"
#include "tktusbmanager/common/memory.h"
#include <pspkerneltypes.h>
#include <string.h>
#include <stddef.h>

int allocTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
    , size_t            _NAME_SIZE
    , const char *      _NAME
    , char              _ENDPOINT
)
{
    SceUID  nameId = allocMemory( _NAME_SIZE * sizeof( *( _endpoint->name ) ) );
    if( nameId < 0 ) {
        return 1;
    }

    char *  name = ( char * )getMemoryAddress( nameId );

    memcpy(
        name
        , _NAME
        , _NAME_SIZE
    );

    _endpoint->nameId = nameId;
    _endpoint->nameSize = _NAME_SIZE;
    _endpoint->name = name;
    _endpoint->endpoint = _ENDPOINT;

    return 0;
}

void freeTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
)
{
    freeMemory( _endpoint->nameId );

    _endpoint->nameId = 0;
}

void initializeTktUsbEndpoint(
    TktUsbEndpoint *        _endpoint
    , struct UsbEndpoint *  _usbEndpoint
)
{
    _endpoint->usbEndpoint = _usbEndpoint;
}
