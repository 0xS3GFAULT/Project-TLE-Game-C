#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "game.h"
#include "rendererAddons.h"
#include "gameManager.h"
#include "selection.h"
#include "renderSelector.h"
#include "../Turrets/turrets.h"
#include "../Turrets/turretUsher.h"
#include "../Robots/robots.h"
#include "./camera.h"
#include "../keyBindings/keyBindings.h"
#include "../UI/UI_pause.h"

static Game *GAME = NULL;

void handleEvents(){
    SDL_Event event;
    SDL_GetMouseState(&GAME->mouseX, &GAME->mouseY);
    SDL_GetWindowSize(GAME->window, &GAME->winWidth, &GAME->winHeight);
    while(SDL_PollEvent(&event))
    {
        if(GAME->selection) handleInputTurretSelection(GAME, &event);
        switch (event.type)
        {
            case SDL_MOUSEBUTTONDOWN:
                if(event.button.button == SDL_BUTTON_LEFT){
                    GAME->mouseLeftDown = true;
                    GAME->menu->handleEvent(true);
                    if(GAME->mouseY < GAME->winHeight - 200) cameraStartDrag();
                    else continue;
                    Selection *selection = GAME->selection;
                    if(!selection || selection->type != SELECT_TURRET){
                        if(selection) free(selection);
                        GAME->selection = NULL;
                        GameObject *goum = getObjectUnderMouse(GAME);
                        if(goum){
                            if(goum->type == GOT_Turret || goum->type == GOT_Robot || goum->type == GOT_Core){
                                selection = malloc(sizeof(Selection));
                                selection->type = SELECT_GAMEOBJECT;
                                selection->selected.gameObject = goum;
                                GAME->selection = selection;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    GAME->mouseLeftDown = false;
                    GAME->menu->handleEvent(false);
                    if(GAME->cameraDragging) cameraEndDrag();
                }
                break;
            case SDL_MOUSEWHEEL:
                if(event.wheel.y > 0)
                    cameraZoomAt(0.1, GAME->mouseX, GAME->mouseY);
                else if(event.wheel.y < 0)
                    cameraZoomAt(-0.1, GAME->mouseX, GAME->mouseY);
                break;
            case SDL_QUIT:
                GAME->isRunning = false;
                GAME->clean();
                break;
            case SDL_KEYDOWN:
                KB_handleKeyCode(event.key.keysym.sym, GAME);
                switch(event.key.keysym.sym)
                {
                    case SDLK_F12:
                        GAME->key_debug++;
                        if(GAME->key_debug==DEBUG_EL) GAME->key_debug = DEBUG_NULL;
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void updateGameObject(void *object){
    ((GameObject*)object)->update(object);
}

void updateAnimation(void *targetAnim){
    ((animation*)targetAnim)->update(targetAnim);
}

void update(){
    GAME->currentCursor = GAME->cursorArrow;
    if(!GAME->pause)
    {
        if (GAME->waveManager->isWaveActive) GAME->waveManager->update();
        forEach(GAME->gameObjects, updateGameObject);
        forEach(GAME->animationManager->animList, updateAnimation);
        GAME->projectileManager->updateProjectiles();
        GAME->projectileManager->applyHits();
    }
        
    GAME->menu->update();
    SDL_SetCursor(GAME->currentCursor);
        
    Selection *selection = GAME->selection;
    if(selection && selection->type == SELECT_GAMEOBJECT){
        if(!searchDataInList(*GAME->gameObjects, selection->selected.gameObject)){
            free(selection);
            GAME->selection = NULL;
        }
    }
    if(!GAME->pause){
        updateGameManager();
        if(GAME->mouseLeftDown) cameraDrag();
    }else if(selection){
        free(selection);
        GAME->selection = NULL;
    }
}

void renderGameObject(void *object){
    ((GameObject*)object)->render(object);
}

void render(){
    SDL_SetRenderDrawColor(GAME->renderer, 13, 17, 23, 255);
    SDL_RenderClear(GAME->renderer);

    
    if(!GAME->pause)
    {
        if(GAME->mapManager->currentMap) GAME->mapManager->render();
        
        renderSelection(GAME);
        forEach(GAME->gameObjects, renderGameObject);
        GAME->projectileManager->renderProjectiles();
        
        if(GAME->selection) renderTurretSelection(GAME);
        else SDL_ShowCursor(SDL_ENABLE);
    }
    
    if(GAME->menu) GAME->menu->render();
    SDL_RenderPresent(GAME->renderer);
}

void deleteGameObject(void *object){
    ((GameObject*)object)->delete(object);
}

void freeAnimation(void *animationToKill){
    animation* this = animationToKill;
    this->manager->killAnim(this->id);
}

void clean(){
    GAME->textureManager->empty();
    GAME->projectileManager->empty();
    GAME->projectileManager->free();
    GAME->mapManager->unloadMap();
    GAME->languageManager->clear();
    GAME->menu->clear();
    forEach(GAME->animationManager->animList, freeAnimation);
    freeList(GAME->animationManager->animList);
    SDL_DestroyRenderer(GAME->renderer);
    SDL_DestroyWindow(GAME->window);
    IMG_Quit();
    SDL_FreeCursor(GAME->cursorArrow);
    SDL_FreeCursor(GAME->cursorHand);
    SDL_Quit();
    printf("SDL Cleaned...\n");
    forEach(GAME->gameObjects, deleteGameObject);
    freeList(GAME->gameObjects);
    KB_free();
    deleteWaveManager();
    free(GAME);
    exit(0);
}

Game *initGame(const char* title, int width, int height, bool fullscreen){
    int flags = 0;
    if(fullscreen) flags = SDL_WINDOW_FULLSCREEN;
    GAME = malloc(sizeof(Game));
    
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0){
        printf("SDL Subsystems Initialised...\n");
        GAME->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
        if(GAME->window) printf("Window Created...\n");
        else printf("\033[1;31mFailed to create window : %s\033[0m\n", SDL_GetError());
        GAME->renderer = SDL_CreateRenderer(GAME->window, -1, 0);
        if(GAME->renderer){
            printf("Renderer Created...\n");
            SDL_SetRenderDrawBlendMode(GAME->renderer, SDL_BLENDMODE_BLEND);
        }
        else printf("\033[1;31mFailed to create renderer : %s\033[0m\n", SDL_GetError());
        GAME->isRunning = true;
        if(!IMG_Init(IMG_INIT_PNG)) printf("\033[1;31mIMG INIT: %s\033[0m\n", IMG_GetError());
        if(TTF_Init()) printf("\033[1;31mTTF INIT: %s\033[0m\n", TTF_GetError());
    } else {
        GAME->isRunning = false;
        printf("\033[1;31mSDL Subsystems Initialising FAILED : %s\033[0m\n", SDL_GetError());
    }
    GAME->SEMusicWeight = 0;
    GAME->mouseX = 0;
    GAME->mouseY = 0;
    GAME->mouseLeftDown = false;
    GAME->winWidth = 0;
    GAME->winHeight = 0;
    GAME->selection = NULL;
    GAME->handleEvents = handleEvents;
    GAME->update = update;
    GAME->render = render;
    GAME->clean = clean;
    GAME->textureManager = initTexManager(GAME->renderer);
    GAME->waveManager = initWaveManager(GAME);
    GAME->animationManager = initAnimManager();
    GAME->projectileManager = initProjectileManager();
    GAME->mapManager = initMapManager(GAME);
    GAME->languageManager = initLanguageManager();
    pauseInit(GAME);
    launchMainMenu(GAME);
    GAME->gameObjects = newList(COMPARE_PTR);
    GAME->keyBindings =  KB_init(GAME);
    GAME->key_debug = DEBUG_NULL;
    GAME->cursorHand = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    GAME->cursorArrow = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    GAME->currentCursor = GAME->cursorArrow;
    GAME->winWidth = width;
    GAME->winWidth = height;
    GAME->pause = false;
    GAME->speedMultiplicator = 1;
    initCamera(GAME);
    return GAME;
}