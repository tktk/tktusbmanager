#include "tktusbrepeater/config/config.h"
#include "tktusbrepeater/common/file.h"
#include "tktusbrepeater/common/memory.h"
#include "parson/parson.h"
#include <pspkerneltypes.h>
#include <string.h>
#include <stddef.h>

#define CONFIG_ELEMENT_NAME_ENDPOINTS "endpoints"

static void freeEndpoint(
    TktUsbRepeaterConfigEndpoint *  _endpoint
)
{
    freeMemory( _endpoint->nameId );
}

static void freeEndpoints(
    const SceUID                        _ENDPOINTS_ID
    , const size_t                      _ENDPOINTS_COUNT
    , TktUsbRepeaterConfigEndpoint *    _endpoints
)
{
    size_t  i;
    for( i = 0 ; i < _ENDPOINTS_COUNT ; i++ ) {
        freeEndpoint( _endpoints + i );
    }

    freeMemory( _ENDPOINTS_ID );
}

static int parseEndpointJson(
    TktUsbRepeaterConfigEndpoint *  _endpoint
    , const JSON_Object *           _ENDPOINTS_OBJECT
    , const size_t                  _INDEX
)
{
    const JSON_Value *  VALUE_JSON = json_object_get_value_at(
        _ENDPOINTS_OBJECT
        , _INDEX
    );
    if( json_value_get_type( VALUE_JSON ) != JSONNumber ) {
        return 1;
    }
    const double    ENDPOINT = json_value_get_number( VALUE_JSON );

    const char *    NAME = json_object_get_name(
        _ENDPOINTS_OBJECT
        , _INDEX
    );
    const size_t    NAME_SIZE = strlen( NAME );

    SceUID  nameId = allocMemory( NAME_SIZE * sizeof( char ) );
    if( nameId < 0 ) {
        return 2;
    }

    char *  name = getMemoryAddress( nameId );

    memcpy(
        name
        , NAME
        , NAME_SIZE
    );

    _endpoint->nameId = nameId;
    _endpoint->nameSize = NAME_SIZE;
    _endpoint->name = name;
    _endpoint->endpoint = ( char )ENDPOINT;

    return 0;
}

static int parseEndpointsJson(
    TktUsbRepeaterConfig *  _config
    , const JSON_Object *   _ROOT_OBJECT
)
{
    const JSON_Object * ENDPOINTS_OBJECT = json_object_get_object(
        _ROOT_OBJECT
        , CONFIG_ELEMENT_NAME_ENDPOINTS
    );
    if( ENDPOINTS_OBJECT == NULL ) {
        return 1;
    }

    const size_t    ENDPOINTS_COUNT = json_object_get_count( ENDPOINTS_OBJECT );

    SceUID  endpointsId = allocMemory( ENDPOINTS_COUNT * sizeof( TktUsbRepeaterConfigEndpoint ) );
    if( endpointsId < 0 ) {
        return 2;
    }

    TktUsbRepeaterConfigEndpoint *  endpoints = getMemoryAddress( endpointsId );

    size_t  i;
    for( i = 0 ; i < ENDPOINTS_COUNT ; i++ ) {
        const int   RESULT = parseEndpointJson(
            endpoints + i
            , ENDPOINTS_OBJECT
            , i
        );
        if( RESULT != 0 ) {
            freeEndpoints(
                endpointsId
                , i
                , endpoints
            );

            return RESULT;
        }
    }

    _config->endpointsId = endpointsId;
    _config->endpointsCount = ENDPOINTS_COUNT;
    _config->endpoints = endpoints;

    return 0;
}

static int parseConfigJson(
    TktUsbRepeaterConfig *  _config
    , const JSON_Value *    _ROOT_JSON
)
{
    const JSON_Object * ROOT_OBJECT = json_value_get_object( _ROOT_JSON );

    const int   RESULT = parseEndpointsJson(
        _config
        , ROOT_OBJECT
    );
    if( RESULT != 0 ) {
        return RESULT;
    }

    return 0;
}

static int parseConfigFile(
    TktUsbRepeaterConfig *  _config
    , const char *          _CONTENTS
)
{
    SceUID  rootJsonId = 0;

    JSON_Value *    rootJson = json_parse_string_with_comments(
        &rootJsonId
        , _CONTENTS
    );
    if( rootJson == NULL ) {
        return 1;
    }

    const int   RESULT = parseConfigJson(
        _config
        , rootJson
    );

    json_value_free(
        rootJsonId
        , rootJson
    );

    return RESULT;
}

int loadConfigFile(
    TktUsbRepeaterConfig *  _config
    , const char *          _FILE_PATH
)
{
    int result;

    SceUID  contentsId = 0;
    char *  contents;

    result = readFile(
        &contentsId
        , &contents
        , _FILE_PATH
    );
    if( result == 0 ) {
        result = parseConfigFile(
            _config
            , contents
        );
    }

    freeMemory( contentsId );

    return result;
}

void unloadConfig(
    TktUsbRepeaterConfig *  _config
)
{
    freeEndpoints(
        _config->endpointsId
        , _config->endpointsCount
        , _config->endpoints
    );
}
