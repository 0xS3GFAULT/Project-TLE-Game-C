/**
 * @file texureManager.h
 * @brief Manage all the projectiles of the game
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "../List/SmartList.h"

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

/**
 * @brief Structure that holds the texture manager
 */
typedef struct
{
    /** @brief get the texture of a SDL_Texture from a string
    *  @param string the string to be found
    *  @return the SDL_Texture that have been found
    */
    SDL_Texture* (*getTexture)(char* string);
    void (*empty)(void);
    list *texList;
} textureManager;

/**
 * @brief Structure that holds the current texture 
 */
typedef struct
{
    char* name;
    SDL_Texture* texture;
} texture;

/** @brief Creates and inits the texture manager
*  @param renderer the SDL_Renderer that renders everything done by the texture manager
*  @return the texture manager created
*/
textureManager *initTexManager(SDL_Renderer *renderer);

#endif