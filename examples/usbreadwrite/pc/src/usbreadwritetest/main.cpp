#include <libusb-1.0/libusb.h>
#include <iostream>
#include <sstream>
#include <array>
#include <memory>
#include <string>
#include <stdexcept>
#include <cstring>

namespace {
    enum {
        VENDOR_ID = 0x054c,
        PRODUCT_ID = 0x01c9,

        ENDPOINT_W = 2 | LIBUSB_ENDPOINT_OUT,
        ENDPOINT_R = 1 | LIBUSB_ENDPOINT_IN,
    };

    struct ExitLibusb
    {
        void operator()(
            libusb_context *    _contextPtr
        ) const
        {
            libusb_exit( _contextPtr );
        }
    };

    using LibusbContextUnique = std::unique_ptr<
        libusb_context
        , ExitLibusb
    >;

    LibusbContextUnique initializeLibusb(
    )
    {
        auto    context = static_cast< libusb_context * >( nullptr );

        const auto  ERROR_CODE = libusb_init( &context );
        if( ERROR_CODE != 0 ) {
            auto    stringStream = std::stringstream();

            stringStream << "libusb_init()が失敗 : " << ERROR_CODE;

            throw std::runtime_error( stringStream.str() );
        }

        return LibusbContextUnique( context );
    }

    struct CloseLibusbHandle
    {
        void operator()(
            libusb_device_handle *  _handlePtr
        ) const
        {
            libusb_reset_device( _handlePtr );
            libusb_close( _handlePtr );
        }
    };

    using LibusbHandleUnique = std::unique_ptr<
        libusb_device_handle
        , CloseLibusbHandle
    >;


    LibusbHandleUnique openLibusbDevice(
        libusb_context &    _context
    )
    {
        auto    handleUnique = LibusbHandleUnique(
            libusb_open_device_with_vid_pid(
                &_context
                , VENDOR_ID
                , PRODUCT_ID
            )
        );
        if( handleUnique.get() == nullptr ) {
            throw std::runtime_error( "libusb_open_device_with_vid_pid()が失敗" );
        }

        return handleUnique;
    }

    void writeLibusb(
        libusb_device_handle &  _handle
        , char *                _STRING
        , int                   _STRING_SIZE
    )
    {
        const auto  ERROR_CODE = libusb_bulk_transfer(
            &_handle
            , ENDPOINT_W
            , reinterpret_cast< unsigned char * >( _STRING )
            , _STRING_SIZE
            , nullptr
            , 0
        );
        if( ERROR_CODE != 0 ) {
            auto    stringStream = std::stringstream();

            stringStream << "libusb_bulk_transfer()が失敗 : " << ERROR_CODE;

            throw std::runtime_error( stringStream.str() );
        }
    }

    void readLibusb(
        libusb_device_handle &  _handle
    )
    {
        auto    data = std::array< unsigned char, 512 >();

        auto    readSize = 0;

        const auto  ERROR_CODE = libusb_bulk_transfer(
            &_handle
            , ENDPOINT_R
            , data.data()
            , data.size()
            , &readSize
            , 0
        );
        if( ERROR_CODE != 0 ) {
            auto    stringStream = std::stringstream();

            stringStream << "libusb_bulk_transfer()が失敗 : " << ERROR_CODE;

            throw std::runtime_error( stringStream.str() );
        }

        auto    str = std::string(
            data.begin()
            , data.begin() + readSize
        );

        std::cout << str << std::endl;
    }
}

int main(
    int         _argc
    , char **   _argv
)
{
    if( _argc < 2 ) {
        std::cout << "引数が必要" << std::endl;

        return 0;
    }

    const auto  STRING = _argv[ 1 ];

    auto    contextUnique = initializeLibusb();
    auto &  context = *contextUnique;

    auto    handleUnique = openLibusbDevice( context );
    auto &  handle = *handleUnique;

    writeLibusb(
        handle
        , STRING
        , std::strlen( STRING )
    );
    readLibusb( handle );

    return 0;
}
