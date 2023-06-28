#include "./waveManager.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../Game/game.h"
#include "../Robots/robots.h"
#include "./waveGenerator.h"

static waveManager *WAVE_MANAGER = NULL;

void WM_update()
{
    if(!WAVE_MANAGER->currentWave) return;
    WAVE_MANAGER->currentWave->clock++;
    static int refreshCount = 0;
    refreshCount++;

    if (refreshCount <10) return;
    refreshCount = 0;
    Game *game = WAVE_MANAGER->parent;
    int nbOfEnnemiesAlive = 0;
    void checkIfEnnmies(void *go)
    {
        GameObject *this = go;
        if (this->type == GOT_Robot)
        {
            if (this->isAlive(this))
            {
                nbOfEnnemiesAlive++;
            }
        }
    }
    forEach(game->gameObjects, checkIfEnnmies);
    if (WAVE_MANAGER->currentWave->chunks->length == 0 && nbOfEnnemiesAlive == 0)
    {
        WAVE_MANAGER->isWaveActive = false;
        return;
    }

    void executeSpawner(void *spawner)
    {
        waveSpawner *this = spawner; 
        for (int i=0;i<this->number;i++){
            switch (this->id)
            {
            case 0:
                newGameObject_Robot(WAVE_MANAGER->parent,"robot.robot",this->spawnLocation,this->seed);
                break;
            case 1:
                newGameObject_Robot(WAVE_MANAGER->parent,"bomb.robot",this->spawnLocation,this->seed);
                break;
            case 2:
                newGameObject_Robot(WAVE_MANAGER->parent,"tank.robot",this->spawnLocation,this->seed);
                break;
            }
        }
    }
    waveChunk *chunkTarget = getDataAtIndex(*WAVE_MANAGER->currentWave->chunks,0);
    if (chunkTarget == NULL) return;
    if (chunkTarget->timer == -1 && chunkTarget->treshold == -1){
        forEach(chunkTarget->waveSpawner,executeSpawner);
        deleteInList(WAVE_MANAGER->currentWave->chunks,chunkTarget);
        free(chunkTarget);
        WAVE_MANAGER->currentWave->clock = 0;
    }
    else if (chunkTarget->timer == -1 && chunkTarget->treshold != -1){
        if (chunkTarget->treshold > nbOfEnnemiesAlive){
            forEach(chunkTarget->waveSpawner,executeSpawner);
            deleteInList(WAVE_MANAGER->currentWave->chunks,chunkTarget);
            free(chunkTarget);
            WAVE_MANAGER->currentWave->clock = 0;
        }
    }
    else if (chunkTarget->timer != -1 && chunkTarget->treshold == -1){
        if (chunkTarget->timer < WAVE_MANAGER->currentWave->clock){
            forEach(chunkTarget->waveSpawner,executeSpawner);
            deleteInList(WAVE_MANAGER->currentWave->chunks,chunkTarget);
            free(chunkTarget);
            WAVE_MANAGER->currentWave->clock = 0;
        }
    }
    else if (chunkTarget->timer != -1 && chunkTarget->treshold != -1){
        if (chunkTarget->timer < WAVE_MANAGER->currentWave->clock || chunkTarget->treshold > nbOfEnnemiesAlive){
            forEach(chunkTarget->waveSpawner,executeSpawner);
            deleteInList(WAVE_MANAGER->currentWave->chunks,chunkTarget);
            free(chunkTarget);
            WAVE_MANAGER->currentWave->clock = 0;
        }
    }
}

void WM_nextWave()
{
    WAVE_MANAGER->waveNumber++;
    wave *newWave = generateNewWave(WAVE_MANAGER->waveNumber, WAVE_MANAGER->difficulty, WAVE_MANAGER);
    WAVE_MANAGER->isWaveActive = true;
    WAVE_MANAGER->currentWave = newWave;
}

void WM_killWave()
{
    WAVE_MANAGER->waveNumber = 0;
    WAVE_MANAGER->isWaveActive = false;
    WAVE_MANAGER->currentWave = NULL;

}

waveManager *initWaveManager(void *game)
{
    waveManager *manager = malloc(sizeof(waveManager));
    manager->currentWave = NULL;
    manager->parent = game;
    manager->isWaveActive = false;
    manager->waveNumber = 0;
    manager->difficulty = 0.1;
    manager->nextWave = WM_nextWave;
    manager->update = WM_update;
    WAVE_MANAGER = manager;
    return manager;
}

void deleteWaveManager(){
    free(WAVE_MANAGER);
}
