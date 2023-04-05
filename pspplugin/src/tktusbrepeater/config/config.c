#include "tktusbrepeater/config/config.h"
#include "tktusbrepeater/common/file.h"
#include "tktusbrepeater/common/memory.h"
#include "parson/parson.h"
#include <pspkerneltypes.h>

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

    freeMemory( contentsId );

    return result;
}

void unloadConfig(
    TktUsbRepeaterConfig *  _config
)
{
    //TODO エンドポイント情報解放
}
