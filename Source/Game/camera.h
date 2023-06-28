/**
 * @file camera.h
 * @brief manage camera abstraction for renderer
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

/** @brief Create and init the camera
*  @param Game the current game
*/
void initCamera(Game *game);

/** @brief Render the camera
*  @param tex the SDL_Texture to render
*  @param des the SDL_Rect of the texture
*/
void cameraRender(SDL_Texture *tex, SDL_Rect dest);

/** @brief Render camera's frames
*  @param tex the SDL_Texture to render
*  @param dest the SDL_Rect of the texture
*  @param frame the number of the frame to render
*/
void cameraRenderFrame(SDL_Texture *tex, SDL_Rect dest, int frame);

/** @brief Render the camera through rectangles with custom angles and flips
*  @param tex the SDL_Texture to render
*  @param dest the SDL_Rect of the texture
*  @param frame the number of the frame to render
*  @param angle the rotation of the camera
*  @param hFlip the horizontal flip (1 = SDL_FLIP_HORIZONTAL) (0 = SDL_FLIP_NONE)
*/
void cameraRenderEx(SDL_Texture *tex, SDL_Rect dest, int frame, float angle, bool hFlip, bool vFlip);

/** @brief Render the camera without rectangles with custom angles and flips
*  @param tex the SDL_Texture to render
*  @param dest the SDL_Rect of the texture
*  @param frame the number of the frame to render
*  @param nOfFrames the number of frames to render
*  @param angle the rotation of the camera
*  @param hFlip the horizontal flip (1 = SDL_FLIP_HORIZONTAL) (0 = SDL_FLIP_NONE)
*/
void cameraRenderExUnsquared(SDL_Texture *tex, SDL_Rect dest, int frame, int nOfFrames, float angle, bool hFlip, bool vFlip);

/** @brief Project a SDL_Rect to a camera
*  @param dest the SDL_Rect of the texture
*/
void projectRectToCamera(SDL_Rect *dest);

/** @brief Prject a SDL_Rect to a camera and inverses it
*  @param dest the SDL_Rect of the texture
*/
void projectRectToCamera_inv(SDL_Rect *dest);

/** @brief Check the size of the camera
*/
void cameraCheckSize();

/** @brief Zooms the camera
*  @param zoom the float to apply for zooming / unzooming
*/
void cameraZoom(float zoom);

/** @brief Moves the camera
*  @param x the abscissa of the moving movement
*  @param y the ordinate of the moving movement
*/
void cameraMove(int x, int y);

/** @brief Zomms the camera from a chosen point
*  @param zoom the SDL_Texture to render
*  @param x the abscissa of where the zoom will be
*  @param y the ordinate of where the zoom will be
*/
void cameraZoomAt(float zoom, int x, int y);

/** @brief Starts the dragging movement of the camera
*/
void cameraStartDrag();

/** @brief Drags the camera while the mouse is pressed
*/
void cameraDrag();

/** @brief Ends the dragging movement of the camera
*/
void cameraEndDrag();