#ifndef TKTUSBMANAGER_CONFIG_ENDPOINT_H
#define TKTUSBMANAGER_CONFIG_ENDPOINT_H

#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    SceUID  nameId;
    size_t  nameSize;
    char *  name;

    char    endpoint;
} TktUsbManagerConfigEndpoint;

#endif  // TKTUSBMANAGER_CONFIG_ENDPOINT_H
