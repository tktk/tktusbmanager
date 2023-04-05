#include "tktusbrepeater/config/config.h"
#include "tktusbrepeater/parson.h"
#include <pspkernel.h>
#include <pspusb.h>

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

static TktUsbRepeaterConfig config;

int module_start(
    SceSize     _args
    , void *    _argp
)
{
    initializeParson();

    if( loadConfigFile(
        &config
        , CONFIG_FILE_PATH
    ) != 0 ) {
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

    unloadConfig( &config );

    return 0;
}
