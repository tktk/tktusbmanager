#include "tktusbrepeater/usb/endpoints.h"
#include "tktusbrepeater/usb/driver.h"
#include "tktusbrepeater/usb/usb.h"
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

static TktUsbEndpoints  usbEndpoints;

static TktUsbDriver usbDriver;

static int initializeUsbEndpoints(
    TktUsbEndpoints *               _usbEndpoints
    , const TktUsbRepeaterConfig *  _CONFIG
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
        const TktUsbRepeaterConfigEndpoint *    ENDPOINT_CONFIG = _CONFIG->endpoints + i;

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

    TktUsbRepeaterConfig    config;

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
        , TKTUSBREPEATER_DRIVERNAME
        , &usbEndpoints
    );
    if( result != 0 ) {
        return result;
    }

    result = registerAndStartUsb( &usbDriver );
    if( result != 0 ) {
        freeTktUsbDriver( &usbDriver );
        freeTktUsbEndpoints( &usbEndpoints );

        return result;
    }

    return 0;
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
    //TODO USB無効化
    //TODO USBドライバの登録解除

    freeTktUsbEndpoints( &usbEndpoints );

    return 0;
}
