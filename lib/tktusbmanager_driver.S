    .set noreorder

#include "pspimport.s"

#ifdef F_tktusbmanager_driver
    IMPORT_START "tktusbmanager_driver",0x00090000
#endif
#ifdef F_tktusbmanager_driver_tktUsbGetEndpointR
    IMPORT_FUNC "tktusbmanager_driver", 0x461A3F66, tktUsbGetEndpointR
#endif
#ifdef F_tktusbmanager_driver_tktUsbGetEndpointW
    IMPORT_FUNC "tktusbmanager_driver", 0xBCF34B66, tktUsbGetEndpointW
#endif
#ifdef F_tktusbmanager_driver_tktUsbRead
    IMPORT_FUNC "tktusbmanager_driver", 0xAB8BD52E, tktUsbRead
#endif
#ifdef F_tktusbmanager_driver_tktUsbWrite
    IMPORT_FUNC "tktusbmanager_driver", 0x3849EE46, tktUsbWrite
#endif
