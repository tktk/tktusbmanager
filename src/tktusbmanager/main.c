#include "tktusbmanager/tktusbmanager.h"
#include "tktusbmanager/usb/endpoints.h"
#include "tktusbmanager/usb/endpoint.h"
#include "tktusbmanager/usb/driver.h"
#include "tktusbmanager/usb/usb.h"
#include "tktusbmanager/config/config.h"
#include "tktusbmanager/common/parson.h"
#include <pspkernel.h>
#include <pspusb.h>
#include <string.h>
#include <stddef.h>

PSP_MODULE_INFO(
    "TktUsbManager"
    , PSP_MODULE_KERNEL
    , 1
    , 1
);

#define TKTUSBMANAGER_DRIVERNAME "TktUsbManagerDriver"

#define CONFIG_FILE_PATH "ms0:/SEPLUGINS/tktusbmanager.cfg"

static TktUsbEndpoints  usbEndpoints;

static TktUsbDriver usbDriver;

static int initializeUsbEndpoints(
    TktUsbEndpoints *               _usbEndpoints
    , const TktUsbManagerConfig *   _CONFIG
)
{
    int result;

    TktUsbEndpoints usbEndpoints_;

    const size_t    ENDPOINTS_COUNT = _CONFIG->endpointsCount;

    result = allocTktUsbEndpoints(
        &usbEndpoints_
        , ENDPOINTS_COUNT
    );
    if( result != 0 ) {
        return result;
    }

    size_t i;
    for( i = 0 ; i < ENDPOINTS_COUNT ; i++ ) {
        const TktUsbManagerConfigEndpoint * ENDPOINT_CONFIG = _CONFIG->endpoints + i;

        result = allocTktUsbEndpoint(
            usbEndpoints_.endpoints + i
            , ENDPOINT_CONFIG->nameSize
            , ENDPOINT_CONFIG->name
            , ENDPOINT_CONFIG->endpoint
        );
        if( result != 0 ) {
            freeTktUsbEndpoints( &usbEndpoints_ );

            return result;
        }
    }

    memcpy(
        _usbEndpoints
        , &usbEndpoints_
        , sizeof( usbEndpoints_ )
    );

    return 0;
}

static int applyConfigFile(
)
{
    int result;

    TktUsbManagerConfig config;

    result = loadConfigFile(
        &config
        , CONFIG_FILE_PATH
    );
    if( result != 0 ) {
        return result;
    }

    result = initializeUsbEndpoints(
        &usbEndpoints
        , &config
    );

    unloadConfig( &config );

    return result;
}

static int registerAndStartUsb(
    TktUsbDriver *  _usbDriver
)
{
    int result;

    result = registerTktUsbDriver( _usbDriver );
    if( result != 0 ) {
        return result;
    }

    result = startUsbBus();
    if( result != 0 ) {
        return result;
    }

    result = startTktUsbDriver( _usbDriver );
    if( result != 0 ) {
        return result;
    }

    result = activateUsb();
    if( result != 0 ) {
        return result;
    }

    return 0;
}

static int startUsb(
)
{
    int result;

    result = allocTktUsbDriver(
        &usbDriver
        , TKTUSBMANAGER_DRIVERNAME
        , &usbEndpoints
    );
    if( result != 0 ) {
        return result;
    }

    initializeTktUsbEndpoints(
        &usbEndpoints
        , &usbDriver
    );

    result = registerAndStartUsb( &usbDriver );
    if( result != 0 ) {
        freeTktUsbDriver( &usbDriver );
        freeTktUsbEndpoints( &usbEndpoints );

        return result;
    }

    return 0;
}

static void stopUsb(
)
{
    deactivateUsb();
    stopTktUsbDriver( &usbDriver );
    stopUsbBus();
    unregisterTktUsbDriver( &usbDriver );

    freeTktUsbDriver( &usbDriver );
}

int module_start(
    SceSize     _args
    , void *    _argp
)
{
    initializeParson();

    if( applyConfigFile() != 0 ) {
        return 0;
    }

    if( startUsb() != 0 ) {
        return 0;
    }

    return 0;
}

int module_stop(
    SceSize     _args
    , void *    _argp
)
{
    stopUsb();

    freeTktUsbEndpoints( &usbEndpoints );

    return 0;
}

typedef int ( * IsEnableEndpoint )(
    const TktUsbEndpoint *
);

static TktUsbEndpoint * tktUsbGetEndpoint(
    const char *        _NAME
    , size_t            _NAME_LENGTH
    , IsEnableEndpoint  _IS_ENABLE_ENDPOINT
)
{
    TktUsbEndpoint *    endpoint = getEndpointTktUsbEndpoints(
        &usbEndpoints
        , _NAME
        , _NAME_LENGTH
    );
    if( endpoint == NULL ) {
        return NULL;
    }

    if( _IS_ENABLE_ENDPOINT( endpoint ) != 0 ) {
        return NULL;
    }

    return endpoint;
}

TktUsbEndpointR * tktUsbGetEndpointR(
    const char *    _NAME
    , size_t        _NAME_LENGTH
)
{
    return ( TktUsbEndpointR * )tktUsbGetEndpoint(
        _NAME
        , _NAME_LENGTH
        , isReadableTktUsbEndpoint
    );
}

TktUsbEndpointW * tktUsbGetEndpointW(
    const char *    _NAME
    , size_t        _NAME_LENGTH
)
{
    return ( TktUsbEndpointW * )tktUsbGetEndpoint(
        _NAME
        , _NAME_LENGTH
        , isWritableTktUsbEndpoint
    );
}

int tktUsbRead(
    TktUsbEndpointR *   _endpoint
    , void *            _buffer
    , int               _bufferSize
)
{
    //TODO
    return 0;
}

int tktUsbWrite(
    TktUsbEndpointW *   _endpoint
    , const void *      _BUFFER
    , int               _bufferSize
)
{
    //TODO
    return 0;
}
