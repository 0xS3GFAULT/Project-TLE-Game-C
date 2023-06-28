/**
 * @file waveManager.h
 * @brief Manage the wave manager
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include "../List/SmartList.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Maps/mapManager.h"

/** @brief holds a list of chunks, a pointer to a Wavemanager and a clock
*/
typedef struct
{
    list *chunks;
    int clock;
    void *parent;
} wave;

/** @brief holds the current wave and elements regarding her as well as a pointer to the game
*/
typedef struct
{
    wave *currentWave;
    /** @brief updates the current wave's state and spawns new ennemies if needed*/
    void (*update)(void);
    /** @brief calls the wave generator to create the next wave and sets it to the current wave*/
    void (*nextWave)(void);
    bool isWaveActive;
    void *parent;
    int waveNumber;
    float difficulty;
} waveManager;

/** @brief holds the number of ennemies it contains, their type and their pathing
*/
typedef struct
{
    int number;
    int id;
    int seed;
    map_node *spawnLocation;
}waveSpawner;

/** @brief holds a list of spawners and two conditions regarding its activation
*/
typedef struct
{
    list *waveSpawner;
    int timer;
    int treshold;
} waveChunk;

/** @brief initializes the wave Manager
    @param game the current main game system 
    @return the wave manager created
    */
waveManager *initWaveManager(void *game);

/** @brief free the game wave Manager*/
void deleteWaveManager();
/** @brief kills a wave (used in case the game is exited)*/
void WM_killWave();
