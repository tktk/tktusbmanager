#include "tktusbmanager/tktusbmanager.h"
#include <pspkerneltypes.h>
#include <pspthreadman.h>

PSP_MODULE_INFO(
    "UsbReadWriteTest"
    , PSP_MODULE_KERNEL
    , 1
    , 1
);

#define ENDPOINT_NAME_R "testR"
#define ENDPOINT_NAME_W "testW"

enum {
    BUFFER_SIZE = 512,
};

static TktUsbEndpointR *    endpointR = NULL;
static TktUsbEndpointW *    endpointW = NULL;

static SceUID   threadId = 0;
static int  ended = 0;

int mainThread(
    SceSize     _size
    , void *    _argp
)
{
    char    buffer[ BUFFER_SIZE ];

    while( ended == 0 ) {
        sceKernelDelayThread( 1000000 );

        const int   READ_SIZE = tktUsbRead(
            endpointR
            , buffer
            , sizeof( buffer )
        );
        if( READ_SIZE < 0 ) {
            continue;
        }

        tktUsbWrite(
            endpointW
            , buffer
            , READ_SIZE
        );
    }

    sceKernelExitThread( 0 );

    return 0;
}

int module_start(
    SceSize     _args
    , void *    _argp
)
{
    endpointR = tktUsbGetEndpointR(
        ENDPOINT_NAME_R
        , sizeof( ENDPOINT_NAME_R ) - 1
    );
    if( endpointR == NULL ) {
        return 0;
    }

    endpointW = tktUsbGetEndpointW(
        ENDPOINT_NAME_W
        , sizeof( ENDPOINT_NAME_W ) - 1
    );
    if( endpointW == NULL ) {
        return 0;
    }

    threadId = sceKernelCreateThread(
        "UsbReadWriteTest"
        , mainThread
        , 15
        , 0x800
        , 0
        , NULL
    );
    if( threadId < 0 ) {
        return 0;
    }

    if( sceKernelStartThread(
        threadId
        , 0
        , NULL
    ) != 0 ) {
        return 0;
    }

    return 0;
}

int module_stop(
    SceSize     _args
    , void *    _argp
)
{
    ended = 1;

    if( threadId >= 0 ) {
        sceKernelTerminateDeleteThread( threadId );
    }

    return 0;
}
