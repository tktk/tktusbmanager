#include "tktusbrepeater/config.h"
#include "parson/parson.h"
#include <pspiofilemgr.h>
#include <pspsysmem.h>
#include <string.h>

#define PARTITION_NAME_CONTENTS "contents"

enum {
    BUFFER_SIZE = 512,

    TAIL = '\0',
};

/*
static TktUsbRepeaterConfig * parseConfigJson(
    const JSON_Value *  _ROOT_JSON
)
{
    const JSON_Object * ROOT_OBJECT = json_value_get_object( _ROOT_JSON );

    //TODO エンドポイント情報読み込み

    TktUsbRepeaterConfig *  config = malloc( sizeof( TktUsbRepeaterConfig ) );

    return config;
}
*/

static int allocContents(
    SceUID *        _contentsIdPtr
    , char **       _contentsPtr
    , const SceSize _CONTENTS_SIZE
)
{
    const SceUID    CONTENTS_ID = sceKernelAllocPartitionMemory(
        2
        , PARTITION_NAME_CONTENTS
        , PSP_SMEM_Low
        , _CONTENTS_SIZE
        , NULL
    );
    if( CONTENTS_ID < 0 ) {
        return 1;
    }

    *_contentsIdPtr = CONTENTS_ID;
    *_contentsPtr = sceKernelGetBlockHeadAddr( CONTENTS_ID );

    return 0;
}

static int reallocContents(
    SceUID *        _contentsIdPtr
    , char **       _contentsPtr
    , const SceSize _CONTENTS_SIZE
    , const SceSize _NEW_CONTENTS_SIZE
)
{
    SceUID  newContentsId;
    char *  newContents;

    const int   RESULT = allocContents(
        &newContentsId
        , &newContents
        , _NEW_CONTENTS_SIZE
    );
    if( RESULT != 0 ) {
        return RESULT;
    }

    memcpy(
        newContents
        , *_contentsPtr
        , _CONTENTS_SIZE
    );

    sceKernelFreePartitionMemory( *_contentsIdPtr );

    *_contentsIdPtr = newContentsId;
    *_contentsPtr = newContents;

    return 0;
}

static int readFileLoop(
    SceUID *        _contentsIdPtr
    , char **       _contentsPtr
    , const SceUID  _FILE_ID
)
{
    int result;

    SceSize contentsSize = BUFFER_SIZE;

    result = allocContents(
        _contentsIdPtr
        , _contentsPtr
        , contentsSize
    );
    if( result != 0 ) {
        return result;
    }

    while( 1 ) {
        const int   READ_SIZE = sceIoRead(
            _FILE_ID
            , ( *_contentsPtr ) + ( contentsSize - BUFFER_SIZE )
            , BUFFER_SIZE
        );
        if( READ_SIZE < BUFFER_SIZE ) {
            contentsSize = contentsSize - BUFFER_SIZE + READ_SIZE;

            break;
        }

        const int   NEW_CONTENTS_SIZE = contentsSize + BUFFER_SIZE;

        reallocContents(
            _contentsIdPtr
            , _contentsPtr
            , contentsSize
            , NEW_CONTENTS_SIZE
        );

        contentsSize = NEW_CONTENTS_SIZE;
    }

    *( ( *_contentsPtr ) + contentsSize ) = TAIL;

    return 0;
}

static int readFile(
    SceUID *        _contentsIdPtr
    , char **       _contentsPtr
    , const char *  _FILE_PATH
)
{
    const SceUID    FILE_ID = sceIoOpen(
        _FILE_PATH
        , PSP_O_RDONLY
        , 0777
    );
    if( FILE_ID <= 0 ) {
        return 1;
    }

    const int   RESULT = readFileLoop(
        _contentsIdPtr
        , _contentsPtr
        , FILE_ID
    );

    sceIoClose( FILE_ID );

    return RESULT;
}

static int parseConfigFile(
    TktUsbRepeaterConfig *  _config
    , const char *          _CONTENTS
)
{
    SceUID  rootJsonId = 0;

    JSON_Value *    rootJson = NULL;
/*
    JSON_Value *    rootJson = json_parse_file_with_comments(
        &rootJsonId
        , _FILE_PATH
    );
    if( rootJson == NULL ) {
        return 1;
    }

    TktUsbRepeaterConfig *  config = parseConfigJson( rootJson );
*/

    json_value_free(
        rootJsonId
        , rootJson
    );

    return 0;
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

    sceKernelFreePartitionMemory( contentsId );

    return result;
}

void unloadConfig(
    TktUsbRepeaterConfig *  _config
)
{
    //TODO エンドポイント情報解放
}
