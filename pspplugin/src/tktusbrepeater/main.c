#include "tktusbrepeater/usb/endpoints.h"
#include "tktusbrepeater/usb/endpoint.h"
#include "tktusbrepeater/config/config.h"
#include "tktusbrepeater/common/parson.h"
#include <pspkernel.h>
#include <pspusb.h>
#include <string.h>
#include <stddef.h>

PSP_MODULE_INFO(
    "TktUsbRepeater"
    , PSP_MODULE_KERNEL
    , 1
    , 1
);

#define TKTUSBREPEATER_DRIVERNAME "TktUsbRepeaterDriver"

#define CONFIG_FILE_PATH "ms0:/SEPLUGINS/tktusbrepeater.cfg"

enum {
    TKTUSBREPEATER_DRIVERPID = 0x1c9,
};

static TktUsbEndpoints  endpoints;

static int initializeEndpoints(
    TktUsbEndpoints *               _endpoints
    , const TktUsbRepeaterConfig *  _CONFIG
)
{
    int result;

    TktUsbEndpoints endpoints;

    const size_t    ENDPOINTS_COUNT = _CONFIG->endpointsCount;

    result = allocEndpoints(
        &endpoints
        , ENDPOINTS_COUNT
    );
    if( result != 0 ) {
        return result;
    }

    size_t i;
    for( i = 0 ; i < ENDPOINTS_COUNT ; i++ ) {
        const TktUsbRepeaterConfigEndpoint *    ENDPOINT_CONFIG = _CONFIG->endpoints + i;

        result = allocEndpoint(
            endpoints.endpoints + i
            , ENDPOINT_CONFIG->nameSize
            , ENDPOINT_CONFIG->name
            , ENDPOINT_CONFIG->endpoint
        );
        if( result != 0 ) {
            freeEndpoints( &endpoints );

            return result;
        }
    }

    memcpy(
        _endpoints
        , &endpoints
        , sizeof( endpoints )
    );

    return 0;
}

static int applyConfigFile(
)
{
    int result;

    TktUsbRepeaterConfig    config;

    result = loadConfigFile(
        &config
        , CONFIG_FILE_PATH
    );
    if( result != 0 ) {
        return result;
    }

    result = initializeEndpoints(
        &endpoints
        , &config
    );

    unloadConfig( &config );

    return result;
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

    //TODO USBドライバの登録

    //TODO USB有効化
/*
    if( sceUsbStart(
        PSP_USBBUS_DRIVERNAME
        , 0
        , 0
    ) != 0 ) {
        return 0;
    }

    if( sceUsbStart(
        TKTUSBREPEATER_DRIVERNAME
        , 0
        , 0
    ) != 0 ) {
        return 0;
    }

    if( sceUsbActivate( TKTUSBREPEATER_DRIVERPID ) != 0 ) {
        return 0;
    }
*/

    return 0;
}

int module_stop(
    SceSize     _args
    , void *    _argp
)
{
    //TODO USB無効化
    //TODO USBドライバの登録解除

    return 0;
}
