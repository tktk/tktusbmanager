#include "tktusbmanager/usb/endpoint.h"
#include "tktusbmanager/common/memory.h"
#include <pspthreadman.h>
#include <pspkerneltypes.h>
#include <string.h>
#include <stddef.h>

#define EVENT_FLAG_NAME "tktusbmanager"

enum {
    USB_ENDPOINT_IN = 0x80,

    USB_REQUEST_DONE = 0x1,
};

int allocTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
    , size_t            _NAME_SIZE
    , const char *      _NAME
    , char              _ENDPOINT
)
{
    SceUID  eventFlagId = sceKernelCreateEventFlag(
        EVENT_FLAG_NAME
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

void setUpTktUsbEndpoint(
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

static int usbRequestDone(
    struct UsbdDeviceReq *  _request
    , int                   _arg2
    , int                   _arg3
)
{
    TktUsbEndpoint *    endpoint = ( TktUsbEndpoint * )( _request->arg );

    sceKernelSetEventFlag(
        endpoint->eventFlagId
        , USB_REQUEST_DONE
    );

    return 0;
}

typedef int ( * SceUsbbdRequest )(
    struct UsbdDeviceReq *
);

static int requestTktUsbEndpoint(
    SceUsbbdRequest             _SCE_USBBD_REQUEST
    , struct UsbdDeviceReq *    _request
    , TktUsbEndpoint *          _endpoint
    , void *                    _buffer
    , int                       _BUFFER_SIZE
)
{
    u32 outFlag;

    memset(
        _request
        , 0
        , sizeof( *_request )
    );

    _request->endp = _endpoint->usbEndpoint;
    _request->data = _buffer;
    _request->size = _BUFFER_SIZE;
    _request->func = usbRequestDone;
    _request->arg = _endpoint;

    const int   RESULT = _SCE_USBBD_REQUEST( _request );
    if( RESULT != 0 ) {
        return RESULT;
    }

    sceKernelWaitEventFlag(
        _endpoint->eventFlagId
        , USB_REQUEST_DONE
        , PSP_EVENT_WAITOR | PSP_EVENT_WAITCLEAR
        , &outFlag
        , NULL
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

    const int   RESULT = requestTktUsbEndpoint(
        sceUsbbdReqRecv
        , &request
        , _endpoint
        , _buffer
        , _BUFFER_SIZE
    );
    if( RESULT != 0 ) {
        return RESULT;
    }

    const int   RETURN_CODE = request.retcode;

    if( RETURN_CODE != 0 ) {
        return RETURN_CODE;
    }

    return request.recvsize;
}

int writeTktUsbEndpoint(
    TktUsbEndpoint *    _endpoint
    , const void *      _DATA
    , int               _DATA_SIZE
)
{
    struct UsbdDeviceReq    request;

    const int   RESULT = requestTktUsbEndpoint(
        sceUsbbdReqSend
        , &request
        , _endpoint
        , ( void * )_DATA
        , _DATA_SIZE
    );
    if( RESULT != 0 ) {
        return RESULT;
    }

    return request.retcode;
}
