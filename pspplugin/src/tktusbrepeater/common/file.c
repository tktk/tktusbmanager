#include "tktusbrepeater/common/file.h"
#include "tktusbrepeater/common/memory.h"
#include <pspiofilemgr.h>
#include <pspkerneltypes.h>
#include <string.h>

enum {
    BUFFER_SIZE = 512,

    TAIL = '\0',
};

static int allocContents(
    SceUID *        _contentsIdPtr
    , char **       _contentsPtr
    , const SceSize _CONTENTS_SIZE
)
{
    const SceUID    CONTENTS_ID = allocMemory( _CONTENTS_SIZE );
    if( CONTENTS_ID < 0 ) {
        return 1;
    }

    *_contentsIdPtr = CONTENTS_ID;
    *_contentsPtr = getMemoryAddress( CONTENTS_ID );

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

    freeMemory( *_contentsIdPtr );

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

    SceUID  contentsId = 0;
    char *  contents;

    result = allocContents(
        &contentsId
        , &contents
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

        result = reallocContents(
            &contentsId
            , &contents
            , contentsSize
            , NEW_CONTENTS_SIZE
        );
        if( result != 0 ) {
            freeMemory( contentsId );

            return result;
        }

        contentsSize = NEW_CONTENTS_SIZE;
    }

    *( contents + contentsSize ) = TAIL;

    *_contentsIdPtr = contentsId;
    *_contentsPtr = contents;

    return 0;
}

int readFile(
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
