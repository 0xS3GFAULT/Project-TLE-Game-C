/**
 * @file core.h
 * @brief Create and manage the core
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Game/game.h"

/** @brief Structure holding core data */
typedef struct{
    /**Core texture*/
    SDL_Texture *coreTex;
    /**Shield texture*/
    SDL_Texture *shieldTex;
    /**Current health of the core*/
    int health;
    /**Health value when full*/
    int maxHealth;
    /**Current shield value*/
    int shield;
    /**Shield value when full*/
    int maxShield;
    /**Delay for the Shield recharge*/
    int rechargeDelay;
    /**Delay for the Shield recharge*/
    int rechargeDelayCounter;
    /**Shield recharge rate*/
    float rechargeRate;
    /**Current core radius (change if shields are disabled)*/
    int radius;
    /**Core node*/
    map_node *node;
} core;

 /** @brief Create and init a core
 * 
 * @param GAME the current game object
 * @param spawnNode the node where the core will be spawned
 * @param maxHealth the max health of the core
 * @param maxShield the max shield of the core
 * @param rechargeDelay the delay between shield recharge
 * @param rechargeRate the recharge rate of the core
 * @return the core gameObject created
 */
GameObject *newGameObject_Core(Game *GAME, map_node *spawnNode, int maxHealth, int maxShield, int rechargeDelay, float rechargeRate);

/** @brief register hit to the core
 * 
 * @param target the core gameObject
 * @param damage the damage to apply
 */
void hitCore(GameObject *target, int damage);