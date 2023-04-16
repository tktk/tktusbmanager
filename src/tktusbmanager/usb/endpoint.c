#include "tktusbmanager/usb/endpoint.h"
#include "tktusbmanager/common/memory.h"
#include <pspthreadman.h>
#include <pspkerneltypes.h>
#include <string.h>
#include <stddef.h>

enum {
    USB_ENDPOINT_IN = 0x80,
};

int allocTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
    , size_t            _NAME_SIZE
    , const char *      _NAME
    , char              _ENDPOINT
)
{
    SceUID  eventFlagId = sceKernelCreateEventFlag(
        "tktusbmanager"
        , PSP_EVENT_WAITMULTIPLE
        , 0
        , NULL
    );
    if( eventFlagId < 0 ) {
        return 1;
    }

    SceUID  nameId = allocMemory( _NAME_SIZE * sizeof( *( _endpoint->name ) ) );
    if( nameId < 0 ) {
        sceKernelDeleteEventFlag( eventFlagId );

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
    _endpoint->eventFlagId = eventFlagId;

    return 0;
}

void freeTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
)
{
    freeMemory( _endpoint->nameId );
    _endpoint->nameId = 0;

    sceKernelDeleteEventFlag( _endpoint->eventFlagId );
    _endpoint->eventFlagId = 0;
}

void initializeTktUsbEndpoint(
    TktUsbEndpoint *        _endpoint
    , struct UsbEndpoint *  _usbEndpoint
)
{
    _endpoint->usbEndpoint = _usbEndpoint;
}

int equalsNameTktUsbEndpoint(
    const TktUsbEndpoint *  _ENDPOINT
    , const char *          _NAME
    , size_t                _NAME_SIZE
)
{
    if( _ENDPOINT->nameSize != _NAME_SIZE ) {
        return 1;
    }

    return memcmp(
        _ENDPOINT->name
        , _NAME
        , _NAME_SIZE
    );
}

int isReadableTktUsbEndpoint(
    const TktUsbEndpoint *  _ENDPOINT
)
{
    return ( _ENDPOINT->endpoint & USB_ENDPOINT_IN ) == 0;
}

int isWritableTktUsbEndpoint(
    const TktUsbEndpoint *  _ENDPOINT
)
{
    return ( _ENDPOINT->endpoint & USB_ENDPOINT_IN ) == USB_ENDPOINT_IN;
}

enum {
    USB_WAITING = 0x1,
};

int readRequestDone(
    struct UsbdDeviceReq *  _request
    , int                   _arg2
    , int                   _arg3
)
{
    TktUsbEndpoint *    endpoint = ( TktUsbEndpoint * )( _request->arg );

    sceKernelSetEventFlag(
        endpoint->eventFlagId
        , USB_WAITING
    );

    return 0;
}

int readTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
    , void *            _buffer
    , int               _BUFFER_SIZE
)
{
    struct UsbdDeviceReq    request;

    u32 result;

    memset(
        &request
        , 0
        , sizeof( request )
    );

    request.endp = _endpoint->usbEndpoint;
    request.data = _buffer;
    request.size = _BUFFER_SIZE;
    request.func = readRequestDone;
    request.arg = _endpoint;

    sceUsbbdReqRecv( &request );

    sceKernelWaitEventFlag(
        _endpoint->eventFlagId
        , USB_WAITING
        , PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR
        , &result
        , NULL
    );

    int returnCode = request.retcode;

    if( returnCode != 0 ) {
        return returnCode;
    }

    return request.recvsize;
}

int writeTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
    , void *            _data
    , int               _DATA_SIZE
)
{
    //TODO
    return -1;
}
