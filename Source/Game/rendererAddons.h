/**
 * @file renderAddons.h
 * @brief Manage all the addons that need to be rendered
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

#ifndef RENDERADDONS_H
#define RENDERADDONS_H

/** @brief Draws a circle on a renderer
*  @param renderer the SDL_Renderer that renders the drawing
*  @param centreX the x position of the circle's center
*  @param centreY the y position of the circle's center
*  @param radius the radius of the circle
*/
void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius);

/** @brief Draws a line on a renderer
*  @param renderer the SDL_Renderer that renders the drawing
*  @param x1 the x1 position of the first dot of the line
*  @param x1 the x1 position of the first dot of the line
*  @param x2 the x2 position of the second dot of the line
*  @param y2 the y2 position of the second dot of the line
*/
void DrawLine(SDL_Renderer * renderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

/** @brief Enumeration that holds the debug type of an object*/
typedef enum {DO_Hit} debugObjectType;

/** @brief Structure that holds the debug object */
typedef struct{
    int x;
    int y;
    int TTL;
    debugObjectType type;
} debugObject;

/** @brief Creates a new game object for debug
*  @param GAME the current main game system
*  @param xpos the x position of the new game object ready for debug
*  @param ypos the y position of the new game object ready for debug
*  @param TTL the time to leave for the game object debug
*  @param type the type of the debug object
*/
GameObject *newGameObject_Debug(Game *GAME, int xpos, int ypos, int TTL, debugObjectType type);

/** @brief Renders a texture through a relative position
*  @param tex the SDL_Texture to render
*  @param x the x position of the texture
*  @param y the y position of the texture
*  @param w the width of the texture
*  @param h the height of the texture
*/
void relativeRender(SDL_Texture *tex, int x, int y, int w, int h);

/** @brief Renders a texture through a relative position with his current frame
*  @param tex the SDL_Texture to render
*  @param x the x position of the texture
*  @param y the y position of the texture
*  @param w the width of the texture
*  @param h the height of the texture
*  @param frame the number of the frame to bind to the texture
*/
void relativeRenderFrame(SDL_Texture *tex, int x, int y, int w, int h, int frame);

/** @brief Renders a texture through a relative position with his current frame and a custom angle/flip
*  @param tex the SDL_Texture to render
*  @param x the x position of the texture
*  @param y the y position of the texture
*  @param w the width of the texture
*  @param h the height of the texture
*  @param frame the number of the frame to bind to the texture
*  @param angle the angle of the rotation of the texture
*  @param hFlip the boolean that flips the texture horizontally or not (1 = SDL_FLIP_HORIZONTAL) (0 = SDL_FLIP_NONE)
*  @param vFlip the boolean that flips the texture vertically or not (1 = SDL_FLIP_VERTICALLY) (0 = SDL_FLIP_NONE) 
*/
void relativeRenderEx(SDL_Texture *tex, int x, int y, int w, int h, int frame, int angle, bool hFlip, bool vFlip);

#endif