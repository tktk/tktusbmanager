#ifndef TKTUSBREPEATER_ENDPOINT_H
#define TKTUSBREPEATER_ENDPOINT_H

#include <stddef.h>

enum {
    MAX_ENDPOINT_NAME_SIZE = 32,
};

typedef struct
{
    size_t  nameSize;
    char    name[ MAX_ENDPOINT_NAME_SIZE ];

    char    endpoint;
} TktUsbEndpoint;

#endif  // TKTUSBREPEATER_ENDPOINT_H
