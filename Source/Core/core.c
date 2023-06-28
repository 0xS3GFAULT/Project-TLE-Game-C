#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "core.h"
#include "../Game/game.h"
#include "../Game/rendererAddons.h"
#include "../Game/camera.h"

static void coreUpdate(void *self){
    GameObject *thisGameObject = self;
    core *this = thisGameObject->actor;
    if(this->health <= 0){
        this->radius = 0;
        thisGameObject->game->speedMultiplicator = 1;
        return;
    }
    if(this->shield < this->maxShield){
        this->rechargeDelayCounter--;
        if(this->rechargeDelayCounter <= 0){
            this->rechargeDelayCounter = 0;
            static float rechargeAmount = 0;
            rechargeAmount += this->rechargeRate;
            if(rechargeAmount >= 1){
                this->shield += rechargeAmount;
                rechargeAmount = 0;
                if(this->shield >= this->maxShield){
                    this->shield = this->maxShield;
                    this->rechargeDelayCounter = this->rechargeDelay;
                }
            }
        }
    }
}

static void coreRender(void *self){
    GameObject *thisGameObject = self;
    core *this = thisGameObject->actor;

    int coreFrames = 6;
    int shieldFrames = 10;

    static int coreTexWidth = 0, coreTexHeight = 0;
    if(!coreTexWidth) SDL_QueryTexture(this->coreTex, NULL, NULL, &coreTexWidth, &coreTexHeight);
    static int shieldTexWidth = 0, shieldTexHeight = 0;
    if(!shieldTexWidth) SDL_QueryTexture(this->shieldTex, NULL, NULL, &shieldTexWidth, &shieldTexHeight);
    int centerX = this->node->x, centerY = this->node->y;

    int coreWidth = coreTexWidth / coreFrames, coreHeight = coreTexHeight;
    int shieldWidth = shieldTexWidth / shieldFrames, shieldHeight = shieldTexHeight;
    if(this->shield > 0) this->radius = shieldWidth;
    else this->radius = coreWidth/2;

    centerX -= coreWidth / 2;

    int coreCurrentFrame = coreFrames - (this->health / (this->maxHealth / coreFrames)) - 1;
    if(coreCurrentFrame < 0) coreCurrentFrame = 0;
    int shieldCurrentFrame = shieldFrames - (this->shield / (this->maxShield / shieldFrames)) - 1;
    if(shieldCurrentFrame < 0) shieldCurrentFrame = 0;

    SDL_Rect rectCore = {centerX - coreWidth/2, centerY - coreHeight/2, coreWidth, coreHeight};
    SDL_Rect rectShield = {centerX - shieldWidth, centerY - shieldHeight, shieldWidth * 2, shieldHeight * 2};
    
    cameraRenderFrame(this->coreTex, rectCore, coreCurrentFrame);
    if(this->shield) cameraRenderFrame(this->shieldTex, rectShield, shieldCurrentFrame);
}

static void coreDelete(void *self){
    GameObject *thisGameObject = self;
    core *this = thisGameObject->actor;
    deleteInList(thisGameObject->game->gameObjects, thisGameObject);
    free(this);
}

static bool coreIsAlive(void *self){
    GameObject *thisGameObject = self;
    core *this = thisGameObject->actor;
    return this->health > 0;
}

GameObject *newGameObject_Core(Game *GAME, map_node *spawnNode, int maxHealth, int maxShield, int rechargeDelay, float rechargeRate){
    core *this = malloc(sizeof(core));
    this->coreTex = GAME->textureManager->getTexture("core.png");
    this->shieldTex = GAME->textureManager->getTexture("shield.png");
    this->health = maxHealth;
    this->maxHealth = maxHealth;
    this->shield = maxShield;
    this->maxShield = maxShield;
    this->rechargeDelay = rechargeDelay;
    this->rechargeDelayCounter = rechargeDelay;
    this->rechargeRate = rechargeRate;
    this->node = spawnNode;
    int decreaseLevel = 0, decreaseTryNumber = 0;
    GameObject *gameObject = malloc(sizeof(GameObject));
    gameObject->actor = this;
    gameObject->type = GOT_Core;
    gameObject->game = GAME;
    gameObject->update = coreUpdate;
    gameObject->render = coreRender;
    gameObject->delete = coreDelete;
    gameObject->isAlive = coreIsAlive;
    appendInList(GAME->gameObjects, gameObject);
    GAME->coreObj = gameObject;
    return gameObject;
}

void hitCore(GameObject *target, int damage){
    core *this = target->actor;
    if(this->shield > 0){
        this->shield -= damage;
        if(this->shield < 0){
            this->shield = 0;
        }
    }else{
        this->health -= damage;
    }
    this->rechargeDelayCounter = this->rechargeDelay;
}