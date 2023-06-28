/**
 * @file game.h
 * @brief Manage the main game system
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "textureManager.h"
#include "animationManager.h"
#include "projectileManager.h"
#include "../UI/UI_mainMenu.h"
#include "../Maps/mapManager.h"
#include "../List/SmartList.h"
#include "../Language/lang.h"
#include "../SoundEngine/sound-engine.h"
#include "../Waves/waveGenerator.h"

/** @brief Enumeration holding debug modes */
typedef enum {DEBUG_NULL, DEBUG_HITBOX, DEBUG_PATH, DEBUG_EL} debug;

/** @brief Enumeration holding game status */
typedef enum {GS_LoadingMainMenu, GS_MainMenu, GS_LoadingMap, GS_InGame} gameStatus;

/** @brief Enumeration holding game object types */
typedef struct GameObject GameObject;
typedef enum {GOT_Turret, GOT_Robot, GOT_Projectile, GOT_Effect, GOT_Core, GOT_DEBUG} GameObjectType;

/** @brief Structure holding the main game system */
typedef struct
{
    /** @brief handle all the events
    */
    void (*handleEvents)(void);

    /** @brief update every element of the game that need to be updated
    */
    void (*update)(void);

    /** @brief render every element of the game that need to be rendered
    */
    void (*render)(void);

    /** @brief clean every element of the game that need to be cleared
    */
    void (*clean)(void);
    /** the current window */
    SDL_Window *window;
    /** the current renderer */
    SDL_Renderer *renderer;
    /** the texture manager that manages all the textures */
    textureManager *textureManager;
    /** the animation manager that manages all the animations of the textures */
    animationManager *animationManager;
    /** the projectile manager that manages all the allies/ennemies projectiles */
    projectileManager *projectileManager;
    /** the map manager that manages the loading of the maps */
    mapManager *mapManager;
    /** the lang manager that manages all the modifications of languages */
    langManager *languageManager;
    /** the current menu */
    UI_menu *menu;
    /** the sound engine that manages all the sounds and musics */
    SoundEngine *soundEngine;
    /** the wave manager that manages all the waves of the game */
    waveManager *waveManager;
    /** sound engine float weight target */
    float SEMusicWeight;
    /** list of all the gameObjects */
    list *gameObjects;
    /** list of all the keys binded */
    list *keyBindings;
    /** the main core held as a game object */
    GameObject *coreObj;
    /** boolean that checks of the game is running or not (1 = running) (0 = not running) */
    bool isRunning;
    /** checks if the key for debugging is pressed */
    debug key_debug;
    /** holds the status of the game */
    gameStatus status;
    /** the current cursor of the mouse */
    SDL_Cursor *currentCursor;
    /** the mouse cursor that represents an arrow*/
    SDL_Cursor *cursorArrow;
    /** the mouse cursor that represents a hand */
    SDL_Cursor *cursorHand;
    /** represents all the things that have been selected from the mouse */
    void *selection;
    /** the x position of the mouse */
    int mouseX;
    /** the y position of the mouse */
    int mouseY;
    /** boolean that checks if the left mouse button is pressed (1 = is pressed) (0 = is not pressed)*/
    bool mouseLeftDown;
    /** the width of the current window */
    int winWidth;
    /** the height of the current window */
    int winHeight;
    /** the x position of the camera */
    int cameraX;
    /** the y position of the camera */
    int cameraY;
    /** boolean that checks if the camera is dragged (1 = dragged) (0 = not dragged) */
    bool cameraDragging;
    /** the current scale of the camera */
    float cameraScale;
    /** boolean that checks if the game is in pause (1 = paused) (0 = not paused) */
    bool pause;
    /** multiplies the FPS to change speed game */
    int speedMultiplicator;
} Game;

/** @brief Creates and Inits the main game system
*  @param title the title of the game
*  @param width the width of the current window of the game
*  @param height the height of the current window of the game
*  @param fullscreen boolean that checks if the game is in fullscreen mode (1 = fullscreen) (0 = windowed)
*  @return the main game system created
*/
Game *initGame(const char* title, int width, int height, bool fullscreen);

/** @brief Structure that holds a gameobject
*/
struct GameObject{
    /** @brief Updates all the game objects
    *  @param self the game object to update
    */
    void (*update)(void *self);

    /** @brief Renders all the game objects
    *  @param self the game object to render
    */
    void (*render)(void *self);

    /** @brief Deletes all the game objects
    *  @param self the game object to delete
    */
    void (*delete)(void *self);

    /** @brief Checks if a game object is alive or not 
    *   @param self the game object to check
    *   @return the boolean that indicates if a game object is alive or not (1 = alive) (0 = dead)
    */
    bool (*isAlive)(void *self);
    /** The main game system */
    Game *game;
    /** The type of the game Object */
    GameObjectType type;
    /* the current actor of a game object */
    void *actor;
};

/** @brief Deletes a game object
*   @param object the game object that needs to be deleted
*/
void deleteGameObject(void *object);

/** @brief Frees the animation of a game object
*   @param animationToKill the animation to kill
*/
void freeAnimation(void *animationToKill);