#ifndef TKTUSBREPEATER_TKTUSBREPEATER_H
#define TKTUSBREPEATER_TKTUSBREPEATER_H

#include <stddef.h>

typedef struct TktUsbEndpointR TktUsbEndpointR;
typedef struct TktUsbEndpointW TktUsbEndpointW;

TktUsbEndpointR * tktUsbGetEndpointR(
    const char *
    , size_t
);

TktUsbEndpointW * tktUsbGetEndpointW(
    const char *
    , size_t
);

int tktUsbRead(
    TktUsbEndpointR *
    , void *
    , int
);

int tktUsbWrite(
    TktUsbEndpointW *
    , const void *
    , int
);

#endif  // TKTUSBREPEATER_TKTUSBREPEATER_H
