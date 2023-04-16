#include "tktusbrepeater/usb/driver.h"
#include "tktusbrepeater/usb/endpoints.h"
#include "tktusbrepeater/usb/endpoint.h"
#include "tktusbrepeater/common/memory.h"
#include <pspusbbus.h>
#include <pspusb.h>
#include <pspkerneltypes.h>
#include <string.h>
#include <stddef.h>

enum {
    STRING_DESCRIPTOR_SIZE = 8 * sizeof( char ),

    MAX_PACKET_SIZE_HI = 512,
    MAX_PACKET_SIZE_FULL = 64,
};

typedef struct
{
    unsigned char   devdesc[ 20 ];

    struct Config   config;
    struct ConfDesc confdesc;

    unsigned char   pad1[ 8 ];

    struct Interfaces   interfaces;
    struct InterDesc    interdesc;
} __attribute__( ( packed ) ) UsbData_;

static void initializeUsbEndpoints(
    struct UsbEndpoint *        _usbEndpoints
    , const TktUsbEndpoints *   _ENDPOINTS
    , const size_t              _USB_ENDPOINTS_COUNT
)
{
    struct UsbEndpoint *    userUsbEndpoints = _usbEndpoints + 1;

    size_t  i;
    for( i = 0 ; i < _ENDPOINTS->endpointsCount ; i++ ) {
        struct UsbEndpoint *    usbEndpoint = userUsbEndpoints + i;
        const TktUsbEndpoint *  ENDPOINT = _ENDPOINTS->endpoints + i;

        usbEndpoint->endpnum = ENDPOINT->endpoint & 0x7f;
    }
}

static void initializeUsbInterface(
    struct UsbInterface *   _usbInterface
)
{
    struct UsbInterface usbInterface = {
        0xFFFFFFFF
        , 0
        , 1
    };

    memcpy(
        _usbInterface
        , &usbInterface
        , sizeof( usbInterface )
    );
}

static void initializeUsbDataDeviceDescriptor(
    unsigned char * _deviceDescriptor
)
{
    const struct DeviceDescriptor   DEVICE_DESCRIPTOR = {
        .bLength = 18,
        .bDescriptorType = 0x01,
        .bcdUSB = 0x200,
        .bDeviceClass = 0,
        .bDeviceSubClass = 0,
        .bDeviceProtocol = 0,
        .bMaxPacketSize = 64,
        .idVendor = 0,
        .idProduct = 0,
        .bcdDevice = 0x100,
        .iManufacturer = 0,
        .iProduct = 0,
        .iSerialNumber = 0,
        .bNumConfigurations = 1,
    };

    memcpy(
        _deviceDescriptor
        , &DEVICE_DESCRIPTOR
        , sizeof( DEVICE_DESCRIPTOR )
    );
}

static void initializeUsbDataConfig(
    struct Config * _config
    , UsbData_ *    _usbData
    , struct Endp * _usbEndpointDescriptors
)
{
    _config->pconfdesc = &( _usbData->confdesc );
    _config->pinterfaces = &( _usbData->interfaces );
    _config->pinterdesc = &( _usbData->interdesc );
    _config->pendp = _usbEndpointDescriptors;
}

static void initializeUsbDataConfigDescriptor(
    struct ConfDesc *   _configDescriptor
    , UsbData_ *        _usbData
    , const size_t      _ENDPOINTS_COUNT
)
{
    const struct ConfigDescriptor   CONFIG_DESCRIPTOR = {
        .bLength = 9,
        .bDescriptorType = 2,
        .wTotalLength = (
            sizeof( struct ConfigDescriptor )
                + sizeof( struct InterfaceDescriptor )
                + ( _ENDPOINTS_COUNT * sizeof( struct EndpointDescriptor ) )
        ),
        .bNumInterfaces = 1,
        .bConfigurationValue = 1,
        .iConfiguration = 0,
        .bmAttributes = 0xC0,
        .bMaxPower = 0,
    };

    memcpy(
        _configDescriptor->desc
        , &CONFIG_DESCRIPTOR
        , sizeof( CONFIG_DESCRIPTOR )
    );

    _configDescriptor->pinterfaces = &( _usbData->interfaces );
}

static void initializeUsbDataInterfaces(
    struct Interfaces * _interfaces
    , UsbData_ *        _usbData
)
{
    _interfaces->pinterdesc[ 0 ] = &( _usbData->interdesc );
    _interfaces->intcount = 1;
}

static void initializeUsbDataInterfaceDescriptor(
    struct InterDesc *  _interfaceDescriptor
    , struct Endp *     _usbEndpointDescriptors
    , const size_t      _ENDPOINTS_COUNT
)
{
    const struct InterfaceDescriptor    INTERFACE_DESCRIPTOR = {
        .bLength = 9,
        .bDescriptorType = 4,
        .bInterfaceNumber = 0,
        .bAlternateSetting = 0,
        .bNumEndpoints = _ENDPOINTS_COUNT,
        .bInterfaceClass = 0xFF,
        .bInterfaceSubClass = 0x1,
        .bInterfaceProtocol = 0xFF,
        .iInterface = 1,
    };

    memcpy(
        _interfaceDescriptor->desc
        , &INTERFACE_DESCRIPTOR
        , sizeof( INTERFACE_DESCRIPTOR )
    );

    _interfaceDescriptor->pendp = _usbEndpointDescriptors;
}

static void initializeUsbData(
    UsbData_ *      _usbData
    , struct Endp * _usbEndpointDescriptors
    , const size_t  _ENDPOINTS_COUNT
)
{
    initializeUsbDataDeviceDescriptor( _usbData->devdesc );
    initializeUsbDataConfig(
        &( _usbData->config )
        , _usbData
        , _usbEndpointDescriptors
    );
    initializeUsbDataConfigDescriptor(
        &( _usbData->confdesc )
        , _usbData
        , _ENDPOINTS_COUNT
    );
    initializeUsbDataInterfaces(
        &( _usbData->interfaces )
        , _usbData
    );
    initializeUsbDataInterfaceDescriptor(
        &( _usbData->interdesc )
        , _usbEndpointDescriptors
        , _ENDPOINTS_COUNT
    );
}

static void initializeUsbEndpointDescriptor(
    struct Endp *               _usbEndpointDescriptor
    , const TktUsbEndpoint *    _ENDPOINT
    , const unsigned short      _MAX_PACKET_SIZE
)
{
    const struct EndpointDescriptor ENDPOINT_DESCRIPTOR = {
        .bLength = 7,
        .bDescriptorType = 5,
        .bEndpointAddress = _ENDPOINT->endpoint,
        .bmAttributes = 2,
        .wMaxPacketSize = _MAX_PACKET_SIZE,
        .bInterval = 0,
    };

    memcpy(
        _usbEndpointDescriptor
        , &ENDPOINT_DESCRIPTOR
        , sizeof( ENDPOINT_DESCRIPTOR )
    );
}

static void initializeUsbEndpointDescriptors(
    struct Endp *               _usbEndpointDescriptors
    , const TktUsbEndpoints *   _ENDPOINTS
    , const unsigned short      _MAX_PACKET_SIZE
)
{
    size_t  i;
    for( i = 0 ; i < _ENDPOINTS->endpointsCount ; i++ ) {
        struct Endp *           usbEndpointDescriptor = _usbEndpointDescriptors + i;
        const TktUsbEndpoint *  ENDPOINT = _ENDPOINTS->endpoints + i;

        initializeUsbEndpointDescriptor(
            usbEndpointDescriptor
            , ENDPOINT
            , _MAX_PACKET_SIZE
        );
    }
}

static void initializeUsbEndpointDescriptorsHi(
    struct Endp *               _usbEndpointDescriptors
    , const TktUsbEndpoints *   _ENDPOINTS
)
{
    initializeUsbEndpointDescriptors(
        _usbEndpointDescriptors
        , _ENDPOINTS
        , MAX_PACKET_SIZE_HI
    );
}

static void initializeUsbEndpointDescriptorsFull(
    struct Endp *               _usbEndpointDescriptors
    , const TktUsbEndpoints *   _ENDPOINTS
)
{
    initializeUsbEndpointDescriptors(
        _usbEndpointDescriptors
        , _ENDPOINTS
        , MAX_PACKET_SIZE_FULL
    );
}

static void initializeStringDescriptor(
    unsigned char * _stringDescriptor
)
{
    unsigned char stringDescriptor[] = {
        0x8, 0x3, '<', 0, '>', 0, 0, 0
    };

    memcpy(
        _stringDescriptor
        , stringDescriptor
        , sizeof( stringDescriptor )
    );
}

static void setUsbDataToUsbDriver(
    void **         _descriptorPtr
    , void **       _configurationPtr
    , UsbData_ *    _usbData
)
{
    *_descriptorPtr = _usbData;
    *_configurationPtr = &( _usbData->config );
}

static int usbRequest(
    int                         _arg1
    , int                       _arg2
    , struct DeviceRequest *    _req
)
{
    return 0;
}

static int func28(
    int     _arg1
    , int   _arg2
    , int   _arg3
)
{
    return 0;
}

static int usbAttach(
    int         _speed
    , void *    _arg2
    , void *    _arg3
)
{
    //TODO
    return 0;
}

static int usbDetach(
    int     _arg1
    , int   _arg2
    , int   _arg3
)
{
    //TODO
    return 0;
}

static int usbStart(
    int         _size
    , void *    _p
)
{
    return 0;
}

static int usbStop(
    int         _size
    , void *    _p
)
{
    return 0;
}

static void initializeUsbDriver(
    struct UsbDriver *      _usbDriver
    , struct UsbEndpoint *  _usbEndpoints
    , struct UsbInterface * _usbInterface
    , UsbData_ *            _usbDataHi
    , UsbData_ *            _usbDataFull
    , unsigned char *       _stringDescriptor
    , const char *          _DRIVER_NAME
    , const size_t          _USB_ENDPOINTS_COUNT
)
{
    _usbDriver->name = _DRIVER_NAME;
    _usbDriver->endpoints = _USB_ENDPOINTS_COUNT;
    _usbDriver->endp = _usbEndpoints;
    _usbDriver->intp = _usbInterface;
    setUsbDataToUsbDriver(
        &( _usbDriver->devp_hi )
        , &( _usbDriver->confp_hi )
        , _usbDataHi
    );
    setUsbDataToUsbDriver(
        &( _usbDriver->devp )
        , &( _usbDriver->confp )
        , _usbDataFull
    );
    _usbDriver->str = ( struct StringDescriptor * )_stringDescriptor;
    _usbDriver->recvctl = usbRequest;
    _usbDriver->func28 = func28;
    _usbDriver->attach = usbAttach;
    _usbDriver->detach = usbDetach;
    _usbDriver->unk34 = 0;
    _usbDriver->start_func = usbStart;
    _usbDriver->stop_func = usbStop;
    _usbDriver->link = NULL;
}

int allocTktUsbDriver(
    TktUsbDriver *              _driver
    , const char *              _DRIVER_NAME
    , const TktUsbEndpoints *   _ENDPOINTS
)
{
    struct UsbDriver *  usbDriver = &( _driver->usbDriver );

    const size_t    USER_ENDPOINTS_COUNT = _ENDPOINTS->endpointsCount;

    const size_t    ALL_ENDPOINTS_COUNT = USER_ENDPOINTS_COUNT + 1;

    const SceSize   MEMORY_SIZE = ALL_ENDPOINTS_COUNT * sizeof( struct UsbEndpoint )
        + sizeof( struct UsbInterface )
        + sizeof( UsbData_ )
        + ALL_ENDPOINTS_COUNT * sizeof( struct Endp )
        + sizeof( UsbData_ )
        + ALL_ENDPOINTS_COUNT * sizeof( struct Endp )
        + STRING_DESCRIPTOR_SIZE
    ;

    SceUID  memoryId = allocMemory( MEMORY_SIZE );

    void *  memoryPtr = getMemoryAddress( memoryId );
    memset(
        memoryPtr
        , 0
        , MEMORY_SIZE
    );

    struct UsbEndpoint *    usbEndpoints = ( struct UsbEndpoint * )memoryPtr;
    struct UsbInterface *   usbInterface = ( struct UsbInterface * )( usbEndpoints + ALL_ENDPOINTS_COUNT );
    UsbData_ *              usbDataHi = ( UsbData_ * )( usbInterface + 1 );
    struct Endp *           usbEndpointDescriptorsHi = ( struct Endp * )( usbDataHi + 1 );
    UsbData_ *              usbDataFull = ( UsbData_ * )( usbEndpointDescriptorsHi + ALL_ENDPOINTS_COUNT );
    struct Endp *           usbEndpointDescriptorsFull = ( struct Endp * )( usbDataFull + 1 );
    unsigned char *         stringDescriptor = ( unsigned char * )( usbEndpointDescriptorsFull + ALL_ENDPOINTS_COUNT );

    initializeUsbEndpoints(
        usbEndpoints
        , _ENDPOINTS
        , ALL_ENDPOINTS_COUNT
    );
    initializeUsbInterface( usbInterface );
    initializeUsbData(
        usbDataHi
        , usbEndpointDescriptorsHi
        , USER_ENDPOINTS_COUNT
    );
    initializeUsbEndpointDescriptorsHi(
        usbEndpointDescriptorsHi
        , _ENDPOINTS
    );
    initializeUsbData(
        usbDataFull
        , usbEndpointDescriptorsFull
        , USER_ENDPOINTS_COUNT
    );
    initializeUsbEndpointDescriptorsFull(
        usbEndpointDescriptorsFull
        , _ENDPOINTS
    );
    initializeStringDescriptor( stringDescriptor );

    initializeUsbDriver(
        usbDriver
        , usbEndpoints
        , usbInterface
        , usbDataHi
        , usbDataFull
        , stringDescriptor
        , _DRIVER_NAME
        , ALL_ENDPOINTS_COUNT
    );

    _driver->memoryId = memoryId;

    return 0;
}

void freeTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    freeMemory( _driver->memoryId );

    _driver->memoryId = 0;
}

int registerTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    return sceUsbbdRegister( &( _driver->usbDriver ) );
}

void unregisterTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    sceUsbbdUnregister( &( _driver->usbDriver ) );
}

int startTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    return sceUsbStart(
        _driver->usbDriver.name
        , 0
        , NULL
    );
}

void stopTktUsbDriver(
    TktUsbDriver *  _driver
)
{
    sceUsbStop(
        _driver->usbDriver.name
        , 0
        , NULL
    );
}
