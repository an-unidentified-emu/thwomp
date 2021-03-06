#ifndef LEVEL_TABLE_H
#define LEVEL_TABLE_H

// For LEVEL_NAME defines, see level_defines.h.
// Please include this file if you want to use them.

#define STUB_LEVEL(_0, levelenum, _2, _3, _4, _5, _6, _7, _8) levelenum,
#define DEFINE_LEVEL(_0, levelenum, _2, _3, _4, _5, _6, _7, _8, _9, _10) levelenum,

enum LevelNum {
    LEVEL_RESTART_GAME = -1,
    LEVEL_NONE,
#include "levels/level_defines.h"
    LEVEL_COUNT,
    LEVEL_MAX = LEVEL_COUNT - 1,
    LEVEL_MIN = LEVEL_NONE + 1,
    LEVEL_FILE_SELECT  = 100, //! This probably breaks if you have 99+ levels
    LEVEL_LEVEL_SELECT = (LEVEL_FILE_SELECT + 1)
};

#undef STUB_LEVEL
#undef DEFINE_LEVEL

#endif // LEVEL_TABLE_H
