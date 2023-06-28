/**
 * @file robots.h
 * @brief Manage all the robots of the game
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Game/game.h"

#ifndef ROBOT_H
#define ROBOT_H

/** @brief holds the robot walking state an animation
*/
typedef struct{
    char *textureName;
    SDL_Texture *texture;
    int nOfFrames;
    int currentFrame;
    int animationDelay;
    int animationDelayCount;
    int frameWidth;
    int frameHeight;
} robot_walk;

/** @brief holds the robot death state and animation
*/
typedef struct{
    char *textureName;
    SDL_Texture *texture;
    int nOfFrames;
    int currentFrame;
    int animationDelay;
    int animationDelayCount;
    int frameWidth;
    int frameHeight;
} robot_death;

/** @brief holds all the information on a robot
*/
typedef struct{
    char* name;
    char* texref;
    int id;
    int seed;
    float rotation;
    float rotationCache;
    float x;
    float y;
    int width;
    int height;
    float speedx;
    float speedy;
    float maxSpeed;
    int life;
    int maxLife;
    int shield;
    int maxShield;
    int lootA;
    int lootB;
    int lootC;
    int delay;
    int delayCounter;
    int range;
    int radius;
    bool isFriendly;
    robot_walk walk;
    robot_death death;
    map_node *targetNode;
    map_node *lastNode;
    char *projectileName;
} robot;

/** @brief creates a new robot Game Object
    @param GAME the current game
    @param robotFileName the name of the config file for our robot
    @param spawnNode the spawn location of the robot
    @param seed the robot pathing
    @returns the created robot Game Object
*/
GameObject *newGameObject_Robot(Game *GAME, char *robotFileName, map_node *spawnNode, int seed);

void robotDelete(void *self);
/** @brief deletes a robot
    @param self the robot himself*/
#endif