#include "./waveGenerator.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../Game/game.h"
#include "../Maps/mapManager.h"

map_node *getSpawnModulo(list *spawns,int spawnId){
    if (spawns->length == 1){
        return getDataAtIndex(*spawns,0);
    }
    if (spawns->length < spawnId+1){
        return getDataAtIndex(*spawns,spawnId%spawns->length);
    }
    if (!spawnId%2){
        int selectedId = (rand()%(spawns->length/2))*2;
        return getDataAtIndex(*spawns,selectedId);
    }
    else {
        if(spawns->length <= 3)
            return getDataAtIndex(*spawns,1);
        
        int selectedId = ((rand()%((spawns->length/2)-1))*2)+1;
        return getDataAtIndex(*spawns,selectedId);
    }
}
    

waveSpawner *newSpawner(int number,int id,int seed, map_node *spawnLocation){
    waveSpawner *newSpawner = malloc(sizeof(waveSpawner));
    newSpawner->number = number;
    newSpawner->id = id;
    newSpawner->seed = seed;
    newSpawner->spawnLocation = spawnLocation;
    return newSpawner;
}

waveChunk *newChunk(){
    waveChunk *newChunk = malloc(sizeof(waveChunk));
    newChunk->waveSpawner = newList(COMPARE_PTR);
    newChunk->timer = -1;
    newChunk->treshold = -1;
    return newChunk;
}

wave *newWave(waveManager *parent){
    wave *newWave = malloc(sizeof(wave));
    newWave->chunks = newList(COMPARE_PTR);
    newWave->clock = -1;
    newWave->parent = parent;
    return newWave;
}

void archetype_smallTeam(list *chunks,float weight){

}

void demoWave(int waveNumber,wave *currentWave){
    waveManager *w = currentWave->parent;
    Game *game = w->parent;
    list *spawns = game->mapManager->currentMap->starts;
    switch (waveNumber)
    {
    case 1:;
        waveChunk *chunk1 = newChunk();
        appendInList(chunk1->waveSpawner,newSpawner(5,1,0,getSpawnModulo(spawns,0)));
        appendInList(currentWave->chunks,chunk1);
        waveChunk *chunk2 = newChunk();
        chunk2->timer = 100;
        appendInList(chunk2->waveSpawner,newSpawner(3,0,0,getSpawnModulo(spawns,1)));
        appendInList(currentWave->chunks,chunk2);
        waveChunk *chunk3 = newChunk();
        chunk3->timer = 100;
        appendInList(chunk3->waveSpawner,newSpawner(1,2,0,getSpawnModulo(spawns,0)));
        appendInList(currentWave->chunks,chunk3);
        break;
    case 2:;
            waveChunk *chunk4 = newChunk();
            appendInList(chunk4->waveSpawner,newSpawner(1,2,0,getSpawnModulo(spawns,0)));
            appendInList(currentWave->chunks,chunk4);
            waveChunk *chunk5 = newChunk();
            appendInList(chunk5->waveSpawner,newSpawner(1,2,0,getSpawnModulo(spawns,1)));
            appendInList(currentWave->chunks,chunk5);
            for (int i=0;i<5;i++){
                waveChunk *chunk = newChunk();
                chunk->timer = 90;
                chunk->treshold = 0;
                appendInList(chunk->waveSpawner,newSpawner(2,0,0,getSpawnModulo(spawns,1)));
                appendInList(currentWave->chunks,chunk);
            }
            for (int i=0;i<=5;i++){
            waveChunk *chunk = newChunk();
            chunk->timer = 90;
            appendInList(chunk->waveSpawner,newSpawner(3,1,0,getSpawnModulo(spawns,2)));
            appendInList(currentWave->chunks,chunk);
            }
        break;
    default:;
            waveChunk *chunk6 = newChunk();
            appendInList(chunk6->waveSpawner,newSpawner(waveNumber,0,0,getSpawnModulo(spawns,0)));
            appendInList(currentWave->chunks,chunk6);
            for (int i=0;i<2+waveNumber;i++){
                waveChunk *chunk = newChunk();
                chunk->timer = 90;
                chunk->treshold = 3;
                appendInList(chunk->waveSpawner,newSpawner(waveNumber,1,0,getSpawnModulo(spawns,i)));
                appendInList(chunk->waveSpawner,newSpawner(i%2,2,0,getSpawnModulo(spawns,i)));
                appendInList(currentWave->chunks,chunk);
            }
            for (int i=0;i<2+waveNumber;i++){
                waveChunk *chunk = newChunk();
                chunk->timer = 90;
                chunk->treshold = 5;
                appendInList(chunk->waveSpawner,newSpawner(waveNumber/3,2,0,getSpawnModulo(spawns,i)));
                appendInList(chunk->waveSpawner,newSpawner(waveNumber,1,0,getSpawnModulo(spawns,0)));
                appendInList(currentWave->chunks,chunk);
            }
            for (int i=0;i<=2+waveNumber;i++){
                waveChunk *chunk = newChunk();
                chunk->timer = 90;
                appendInList(chunk->waveSpawner,newSpawner(waveNumber*2,1,0,getSpawnModulo(spawns,0)));
                appendInList(chunk->waveSpawner,newSpawner(waveNumber*2,1,0,getSpawnModulo(spawns,1)));
                appendInList(currentWave->chunks,chunk);
            }
        break;
    }
}

void introToRobots(int waveNumber,wave *currentWave){
    waveManager *w = currentWave->parent;
    Game *game = w->parent;
    list *spawns = game->mapManager->currentMap->starts;
    switch (waveNumber)
    {
    case 1:;
            waveChunk *chunk1 = newChunk();
            appendInList(chunk1->waveSpawner,newSpawner(1,0,0,getSpawnModulo(spawns,0)));
            appendInList(currentWave->chunks,chunk1);
        for (int i=0;i<5;i++){
            waveChunk *chunk = newChunk();
            chunk->timer = 90;
            appendInList(chunk->waveSpawner,newSpawner(1,0,0,getSpawnModulo(spawns,0)));
            appendInList(currentWave->chunks,chunk);
        }
        for (int i=0;i<=5;i++){
            waveChunk *chunk = newChunk();
            chunk->timer = 90;
            appendInList(chunk->waveSpawner,newSpawner(1,0,0,getSpawnModulo(spawns,1)));
            appendInList(currentWave->chunks,chunk);
        }
        break;
    case 2:
        
        break;
    case 3:
        
        break;
    case 4:
        
        break;
    case 5:
        
        break;
    
    default:
        break;
    }
}

void introToBombers(int waveNumber,wave *currentWave){

}

void introToTanks(int waveNumber,wave *currentWave){

}

void introToVariants(int waveNumber,wave *currentWave){

}

wave *generateNewWave(int waveNumber,float difficulty,waveManager *parent){
    srand(waveNumber);
    wave *wave = newWave(parent);
    float weight = waveNumber*difficulty;
    if (waveNumber) demoWave(waveNumber,wave);
    else if (waveNumber < 6) introToRobots(waveNumber,wave);
    else if(waveNumber <11 && waveNumber >5) introToBombers(waveNumber,wave);
    else if(waveNumber <16 && waveNumber >10) introToTanks(waveNumber,wave);
    else if(waveNumber <21 && waveNumber >15) introToVariants(waveNumber,wave);
    else{};
    return wave;
}


