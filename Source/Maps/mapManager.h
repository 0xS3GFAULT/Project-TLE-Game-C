/**
 * @file mapManager.h
 * @brief Manages the map manager
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "../List/SmartList.h"
#include "./mapDataManagement.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>
/** @brief a map preview used to make thumbnails for a selection menu*/
typedef struct{
    char *id;
    char *name;
    char *description;
    SDL_Texture *texture;
} mapPreview;

/** @brief a map Manager used to load and render a map*/
typedef struct
{
    map *currentMap;
    /** @brief open map files and loads the corresponding map
        @param id the name of the map asset
        @param width the width of the map
        @param height the height of the map
        @param progress progress bar when loading the map
    */
    void (*loadMap)(const char *id, const int width, const int height, float *progress);
    /** @brief unloads the current map*/
    void (*unloadMap)(void);
    /** @brief renders the loaded map*/
    void (*render)(void);
    /** @brief updates the map previews*/
    void (*updatePreviews)(void);
    list *previews;
    void *parent;
} mapManager;

/** @brief initializes a new map Manager
    @param game the current game
    @return the map Manager created
*/
mapManager *initMapManager(void *game);