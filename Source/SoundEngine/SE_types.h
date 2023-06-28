#pragma once

// typedef all structs and enums
typedef enum SE_bool SE_bool;
typedef enum SE_type SE_type;

// enums
// booleans for sound engine
enum SE_bool
{
    SE_FALSE = 0,
    SE_TRUE = 1
};

// music types
enum SE_type
{
    SE_MUSIC = 0,
    SE_MUSIC_FX = 1,
    SE_SFX = 2,
    SE_AMBIENT = 3,
    SE_DIALOG = 4
};