#ifndef TKTUSBREPEATER_CONFIG_ENDPOINT_H
#define TKTUSBREPEATER_CONFIG_ENDPOINT_H

#include <pspkerneltypes.h>
#include <stddef.h>

typedef struct
{
    SceUID  nameId;
    size_t  nameSize;
    char *  name;

    char    endpoint;
} TktUsbRepeaterConfigEndpoint;

#endif  // TKTUSBREPEATER_CONFIG_ENDPOINT_H
