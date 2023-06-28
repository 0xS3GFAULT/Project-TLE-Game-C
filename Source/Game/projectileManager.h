/**
 * @file projectileManager.h
 * @brief Manage all the projectiles of the game
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../List/SmartList.h"

#ifndef PROJECTILEMANAGER_H
#define PROJECTILEMANAGER_H

/**
 * @brief Enumeration that holds the weapon type
 */
typedef enum{BALLISTIC, PLASMA, EXPLOSIVE, LASER} weaponType;

/**
 * @brief Structure that holds all the data of a hit
 */
typedef struct{
    /** @brief deletes a hit when done
    *  @param self the hit to delete
    */
    void (*delete)(void *self);
    int damage;
    float x;
    float y;
    weaponType type;
    void *parent;
    void *target;
} hit;

typedef struct projectile projectile;

/**
 * @brief Structure that holds the projectile manager
 */
typedef struct
{
    /** @brief empties the list of projectile
    */
    void (*empty)(void);

    /** @brief frees a projectile when the hit is done or when the projectile can't be seen
    */
    void (*free)(void);

    /** @brief creates a new projectile
    *  @param GAME the current main game system
    *  @param projectileFileName the name of the projectile to bind with its texture
    *  @param xpos x position of the projectile
    *  @param ypos y position of the projectile
    *  @param rotation the angle of the projectile
    *  @param parent the parent of the projectile
    *  @param target the target of the projectile
    */
    projectile *(*newProjectile)(void *GAME, char *projectileFileName, float xpos, float ypos, float rotation, void *parent, void *target);
    
    /** @brief Creates a new hit
    *  @param damage the damage of the hit
    *  @param x the x position of the hit
    *  @param y the y position of the hit
    *  @param type the weapon type of the hit
    *  @param parent the parent of the hit
    *  @param target the target of the hit
    */
    void (*newHit)(int damage, float x, float y, weaponType type, void *parent, void *target);

    /** @brief updates projectiles
    */
    void (*updateProjectiles)(void);

    /** @brief render projectiles
    */
    void (*renderProjectiles)(void);

    /** @brief apply the hits when projectiles are touching the ennemies
    */
    void (*applyHits)(void);

    /** list of projectiles  */
    list *projectiles;
    /** list of hits */
    list *hits;
} projectileManager;

/** @brief Structure that holds the projectile renderer
    */
typedef struct{
    char* texref;
    SDL_Texture *texture;
    int freamWidth;
    int freamHeight;
    int nOfFrames;
    int currentFrame;
    char *animationId;
} projectileRenderer;


/** @brief Structure that holds the projectile
    */
typedef struct projectile{
    /** @brief Updates a projectile
    *  @param self the projectile to be updated
    */
    void (*update)(void *self);

    /** @brief Renders a projectile
    *  @param self the projectile to be rendered
    */
    void (*render)(void *self);

    /** @brief Deletes a projectile
    *  @param self the projectile to be deleted
    */
    void (*delete)(void *self);

    /** @brief Checks if a projectile is alive or not
    *  @param self the projectile to be checked
    *  @return the boolean that indicates if a projectile is alive or not (1 = is alive) (0 = is dead) 
    */
    bool (*isAlive)(void *self);
    unsigned int id;
    float speedx;
    float speedy;
    float speed;
    float rotation;
    float x;
    float y;
    int width;
    int height;
    int damage;
    int perforance;
    bool isFriendly;
    projectileRenderer projectileRenderer;
    projectileManager *manager;
    weaponType type;
    void *target;
    void *parent;
} projectile;

/** @brief Delete a projectile
 * @param self the projectile to be deleted
    */
void projectileDelete(void *self);

/** @brief Creates and inits the projectile manager
 *  @return the projectile manager created
*/
projectileManager *initProjectileManager();
#endif