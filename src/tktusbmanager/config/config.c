#include "tktusbmanager/config/config.h"
#include "tktusbmanager/common/file.h"
#include "tktusbmanager/common/memory.h"
#include "parson/parson.h"
#include <pspkerneltypes.h>
#include <string.h>
#include <stddef.h>

#define CONFIG_ELEMENT_NAME_ENDPOINTS "endpoints"

#define BLOCK_COMMENT_BEGIN "/*"
#define BLOCK_COMMENT_END "*/"

#define LINE_COMMENT_BEGIN "//"

enum {
    SPACE = ' ',
    COMMA = ',',

    END_LINE = '\n',
    END_STRING = '\0',

    END_JSON_ARRAY = ']',
    END_JSON_OBJECT = '}',

    BLOCK_COMMENT_BEGIN_SIZE = sizeof( BLOCK_COMMENT_BEGIN ) - 1,
    BLOCK_COMMENT_END_SIZE = sizeof( BLOCK_COMMENT_END ) - 1,

    LINE_COMMENT_BEGIN_SIZE = sizeof( LINE_COMMENT_BEGIN ) - 1,
};

static void freeEndpoint(
    TktUsbManagerConfigEndpoint *   _endpoint
)
{
    freeMemory( _endpoint->nameId );
}

static void freeEndpoints(
    const SceUID                    _ENDPOINTS_ID
    , const size_t                  _ENDPOINTS_COUNT
    , TktUsbManagerConfigEndpoint * _endpoints
)
{
    size_t  i;
    for( i = 0 ; i < _ENDPOINTS_COUNT ; i++ ) {
        freeEndpoint( _endpoints + i );
    }

    freeMemory( _ENDPOINTS_ID );
}

static int parseEndpointJson(
    TktUsbManagerConfigEndpoint *   _endpoint
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
    TktUsbManagerConfig *   _config
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

    SceUID  endpointsId = allocMemory( ENDPOINTS_COUNT * sizeof( TktUsbManagerConfigEndpoint ) );
    if( endpointsId < 0 ) {
        return 2;
    }

    TktUsbManagerConfigEndpoint *   endpoints = getMemoryAddress( endpointsId );

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
    TktUsbManagerConfig *   _config
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

static void fillLineComment(
    char *  _contents
)
{
    char *  contentsPtr = _contents;

    while( 1 ) {
        char *  beginCommentPtr = strstr(
            contentsPtr
            , LINE_COMMENT_BEGIN
        );
        if( beginCommentPtr == NULL ) {
            break;
        }

        char *  inCommentPtr = beginCommentPtr + LINE_COMMENT_BEGIN_SIZE;

        char *  endCommentPtr = strchr(
            inCommentPtr
            , END_LINE
        );
        if( endCommentPtr == NULL ) {
            endCommentPtr = strchr(
                inCommentPtr
                , END_STRING
            );
        }

        memset(
            beginCommentPtr
            , SPACE
            , ( size_t )endCommentPtr - ( size_t )beginCommentPtr
        );

        if( *endCommentPtr == END_STRING ) {
            break;
        }

        contentsPtr = endCommentPtr + 1;
    }
}

static int fillBlockComment(
    char *  _contents
)
{
    char *  contentsPtr = _contents;

    while( 1 ) {
        char *  beginCommentPtr = strstr(
            contentsPtr
            , BLOCK_COMMENT_BEGIN
        );
        if( beginCommentPtr == NULL ) {
            break;
        }

        char *  endCommentPtr = strstr(
            beginCommentPtr + BLOCK_COMMENT_BEGIN_SIZE
            , BLOCK_COMMENT_END
        );
        if( endCommentPtr == NULL ) {
            return 1;
        }

        endCommentPtr += BLOCK_COMMENT_END_SIZE;

        memset(
            beginCommentPtr
            , SPACE
            , ( size_t )endCommentPtr - ( size_t )beginCommentPtr
        );

        contentsPtr = endCommentPtr;
    }

    return 0;
}

static void fillTailComma(
    char *  _contents
)
{
    char *  contentsPtr = _contents;

    while( 1 ) {
        char *  commaPtr = strchr(
            contentsPtr
            , COMMA
        );
        if( commaPtr == NULL ) {
            break;
        }

        contentsPtr = commaPtr + 1;

        while( isspace( *contentsPtr ) != 0 ) {
            contentsPtr++;
        }

        if( *contentsPtr != END_JSON_ARRAY && *contentsPtr != END_JSON_OBJECT ) {
            continue;
        }

        *commaPtr = SPACE;
    }
}

static int parseConfigFile(
    TktUsbManagerConfig *   _config
    , char *                _contents
)
{
    int result;

    fillLineComment( _contents );

    result = fillBlockComment( _contents );
    if( result != 0 ) {
        return result;
    }

    fillTailComma( _contents );

    SceUID  rootJsonId = 0;

    JSON_Value *    rootJson = json_parse_string(
        &rootJsonId
        , _contents
    );
    if( rootJson == NULL ) {
        return 1;
    }

    result = parseConfigJson(
        _config
        , rootJson
    );

    json_value_free(
        rootJsonId
        , rootJson
    );

    return result;
}

int loadConfigFile(
    TktUsbManagerConfig *   _config
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
    TktUsbManagerConfig *   _config
)
{
    freeEndpoints(
        _config->endpointsId
        , _config->endpointsCount
        , _config->endpoints
    );
}
