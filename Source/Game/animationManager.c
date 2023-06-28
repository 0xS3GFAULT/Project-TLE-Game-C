#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../List/SmartList.h"
#include "animationManager.h"

static animationManager *ANIMATION_MANAGER = NULL;

int compareAnimId(void *data1, void *data2){
    char *str1 = ((animation*)data1)->id;
    char *str2 = ((animation*)data2)->id;
    return strcmp(str1, str2);
}

void updateAnim(void *self){
    animation *this = self;
    if(this->timer >= this->delay){
        this->timer = 0;
        *this->toIncrement += 1;
        if(*this->toIncrement >= this->nOfFrames - 1){
            *this->toIncrement = 0;
            this->manager->killAnim(this->id);
        }
    } else this->timer++;
}

void addAnim(char* id, int* toIncrement, int nOfFrames, int delay){
    animation *newAnimation = malloc(sizeof(animation));
    ANIMATION_MANAGER->killAnim(id);
    newAnimation->id = malloc(sizeof(char) * (strlen(id)+1));
    strcpy(newAnimation->id, id);
    newAnimation->toIncrement = toIncrement;
    newAnimation->nOfFrames = nOfFrames;
    newAnimation->delay = delay;
    newAnimation->timer = 0;
    newAnimation->update = updateAnim;
    newAnimation->manager = ANIMATION_MANAGER;
    appendInList(ANIMATION_MANAGER->animList, newAnimation);
}

animation *getAnim(char* id){
    animation *request = malloc(sizeof(animation));
    request->id = id;
    animation *result = searchDataInList(*ANIMATION_MANAGER->animList, request);
    free(request);
    return result;
}

void killAnim(char* id){
    animation *toKill = getAnim(id);
    if(toKill){
        deleteInList(ANIMATION_MANAGER->animList, toKill);
        free(toKill->id);
        free(toKill);
    }
}

animationManager *initAnimManager(){
    animationManager *newManager = malloc(sizeof(animationManager));
    ANIMATION_MANAGER = newManager;
    newManager->animList = newList(compareAnimId);
    newManager->addAnim = addAnim;
    newManager->killAnim = killAnim;
    newManager->getAnim = getAnim;
    return newManager;
}