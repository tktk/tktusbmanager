    .set noreorder

#include "pspimport.s"

#ifdef F_tktusbmanager
    IMPORT_START "tktusbmanager",0x40090000
#endif
#ifdef F_tktusbmanager_tktUsbGetEndpointR
    IMPORT_FUNC "tktusbmanager", 0x461A3F66, tktUsbGetEndpointR
#endif
#ifdef F_tktusbmanager_tktUsbGetEndpointW
    IMPORT_FUNC "tktusbmanager", 0xBCF34B66, tktUsbGetEndpointW
#endif
#ifdef F_tktusbmanager_tktUsbRead
    IMPORT_FUNC "tktusbmanager", 0xAB8BD52E, tktUsbRead
#endif
#ifdef F_tktusbmanager_tktUsbWrite
    IMPORT_FUNC "tktusbmanager", 0x3849EE46, tktUsbWrite
#endif
