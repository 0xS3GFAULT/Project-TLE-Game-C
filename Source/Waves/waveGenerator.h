/**
 * @file waveGenerator.h
 * @brief Manage the wave generator
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include "./waveManager.h"

/** @brief generates a new wave
    @param waveNumber the current wave number
    @param difficulty the degree of difficulty of the wave
    @param parent the current wave manager
    @return the wave generated
*/
wave *generateNewWave(int waveNumber,float difficulty,waveManager *parent);
/** @brief written waves with ennemy variants
    @param waveNumber the current wave number
    @param currentWave the currrent wave
*/
void introToVariants(int waveNumber,wave *currentWave);
/** @brief written waves with the introduction of tanks
    @param waveNumber the current wave number
    @param currentWave the currrent wave
*/
void introToTanks(int waveNumber,wave *currentWave);
/** @brief written waves with the introduction of bombers
    @param waveNumber the current wave number
    @param currentWave the currrent wave
*/
void introToBombers(int waveNumber,wave *currentWave);
/** @brief written waves with the introduction of robots
    @param waveNumber the current wave number
    @param currentWave the currrent wave
*/
void introToRobots(int waveNumber,wave *currentWave);

/** @brief creates an empty wave
    @param parent the current wave manager
    @return the wave created
*/
wave *newWave(waveManager *parent);

/** @brief creates an empty chunk
    @return the chunk created
*/
waveChunk *newChunk();

/** @brief creates a new spawner
    @param number the number of ennemies in the spawner
    @param id the type of ennemies    
    @param seed the seed of the path taken
    @param spawnLocation the starting location for the ennemies spawned
    @return the wave Spawner created
*/
waveSpawner *newSpawner(int number,int id,int seed, map_node *spawnLocation);

/** @brief used to define a spwan location for a spawner when dealing with any maps with n spawn locations
    @param spawns the list of spawners added
    @param spawnId the wanted id of the spawn location
    @return the map node corresponding to the chosen location
*/
map_node *getSpawnModulo(list *spawns,int spawnId);

