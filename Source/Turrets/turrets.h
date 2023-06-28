/**
 * @file turrets.h
 * @brief Manages the turrets
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "../Game/game.h"

/** @brief the turret base and its texture*/
typedef struct{
    char *textureName;
    SDL_Texture *texture;
    int frameWidth;
    int frameHeight;
} turret_base;

/** @brief the turret canon, its texture and animation*/
typedef struct{
    char *textureName;
    SDL_Texture *texture;
    int nOfFrames;
    int currentFrame;
    int fireFrame;
    char *animationId;
    int frameWidth;
    int frameHeight;
} turret_canon;

/** @brief the turret state corresponding to its level with its stats and cost*/
typedef struct{
    char* texref;
    turret_base base;
    turret_canon canon;
    int delay;
    int range;
    int costA;
    int costB;
    int costC;
    char *projectileName;
    float maxRotationSpeed;
    float maxRotationAcceleration;
} turret_state;

/** @brief the turret information that doesn't change */
typedef struct{
    char** name;
    char** description;
    int id;
    float rotation;
    int x;
    int y;
    int width;
    int height;
    bool turretLock;
    turret_state *currentState;
    list *states;
} turret;

/** @brief the turret selection process linked to the selected turret*/
typedef struct{
    char *turretId;
    int x;
    int y;
    int width;
    int height;
    int radius;
    int firerate;
    int damage;
    int costA;
    int costB;
    int costC;
    SDL_Texture *thumbnail;
    SDL_Texture *allowed;
    SDL_Texture *forbidden;
    char **name;
} turretSelection;

/** @brief the turret selection process linked to the selected turret
    @param game the current game
    @param turretFileName the name of the config file for our turret
    @param xpos the x postion the turret
    @param ypos the y position the turret
    @return the new turret Game Object
*/
GameObject *newGameObject_Turret(Game *GAME, char *turretFileName, int xpos, int ypos);
/** @brief the turret selection process linked to the selected turret
    @param GAME the current game
    @return the list of turrets for the bottom of the HUD
*/
list *generateTurretsSelection(Game *GAME);

/** @brief upgrade a turret to next state
    @param this the turret to upgrade
*/
void upgradeTurret(GameObject *this);

/** @brief sell a turret
    @param this the turret to sell
*/
void sellTurret(GameObject *this);