#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../Robots/robots.h"
#include "../Turrets/turrets.h"
#include "../Core/core.h"
#include "camera.h"
#include "projectileManager.h"
#include "game.h"
#include "rendererAddons.h"

#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)
#define ABS(X)  ((X>=0)? X : -(X) ) 
#define ROUND(X)  (X>=0)? (int) (X + 0.5) : (int)-(ABS(X) +0.5)

typedef enum{P_TEX_REF, P_TEX_ANIM_FRAMES,P_WIDTH, P_HEIGHT, P_WEAPON_DAMAGE, P_WEAPON_SPEED, P_WEAPON_PERFORANCE, P_WEAPON_TYPE, P_NONE} projectileConfigFileParam;

static projectileManager *PROJECTILE_MANAGER = NULL;


void hitDelete(void *self){
    hit *this = self;
    deleteInList(PROJECTILE_MANAGER->hits, this);
    free(this);
}

void newHit(int damage, float x, float y, weaponType type, void *parent, void *target){
    hit *createdHit = malloc(sizeof(hit));
    createdHit->delete = hitDelete;
    createdHit->parent = parent;
    createdHit->target = target;
    createdHit->damage = damage;
    createdHit->x = x;
    createdHit->y = y;
    pushInList(PROJECTILE_MANAGER->hits, createdHit);
}

void applyHit(void *self){
    hit *this = self;
    GameObject *targetObject = this->target;
    switch (targetObject->type) {
        case GOT_Robot:
            ((robot*)targetObject->actor)->life -= this->damage;
            if(((robot*)targetObject->actor)->life <= 0)
                ((robot*)targetObject->actor)->life = 0;
            break;
        case GOT_Core:
            hitCore(targetObject, this->damage);
            break;
        default:
            break;
    }
    if(targetObject->game->key_debug == DEBUG_HITBOX)
        newGameObject_Debug(targetObject->game, this->x, this->y, 600, DO_Hit);
    this->delete(this);
}

projectileConfigFileParam getProjectileConfigFileParamFromString(char *fileParamString){
    if(!strcmp("TEX_REF", fileParamString)) return P_TEX_REF;
    if(!strcmp("TEX_ANIM_FRAMES", fileParamString)) return P_TEX_ANIM_FRAMES;
    if(!strcmp("WEAPON_DAMAGE", fileParamString)) return P_WEAPON_DAMAGE;
    if(!strcmp("WEAPON_SPEED", fileParamString)) return P_WEAPON_SPEED;
    if(!strcmp("WEAPON_PERFORANCE", fileParamString)) return P_WEAPON_PERFORANCE;
    if(!strcmp("WIDTH", fileParamString)) return P_WIDTH;
    if(!strcmp("HEIGHT", fileParamString)) return P_HEIGHT;
    if(!strcmp("WEAPON_TYPE", fileParamString)) return P_WEAPON_TYPE;
    return P_NONE;
}

weaponType getWeaponTypeFromString(char *fileParamString){
    if(!strcmp("BALLISTIC", fileParamString)) return BALLISTIC;
    if(!strcmp("PLASMA", fileParamString)) return PLASMA;
    if(!strcmp("EXPLOSIVE", fileParamString)) return EXPLOSIVE;
    if(!strcmp("LASER", fileParamString)) return LASER;
    return 0;
}

projectile *newProjectile(void *game, char *projectileFileName, float xpos, float ypos, float rotation, bool isFriendly, void *target){
    projectile *createdProjectile = malloc(sizeof(projectile));
    Game *GAME = game;
    static int id = 0;
    
    createdProjectile->id = id;
    id++;

    createdProjectile->target = target;
    char path[50];
    sprintf(path, "./assets/projectiles/%s", projectileFileName);
    FILE *projectile_file = fopen(path,"r");
    char line_cache[255];
    char stat_name[255];
    char stat_value[255];
    createdProjectile->isFriendly = false;
    createdProjectile->projectileRenderer.animationId = NULL;
    while(fgets(line_cache, 255, projectile_file) != NULL){
        sscanf(line_cache,"%[^ ] = %[^\n]", stat_name, stat_value);
        switch(getProjectileConfigFileParamFromString(stat_name)){
            case P_TEX_REF:
                createdProjectile->projectileRenderer.texref = malloc(sizeof(char)*(strlen(stat_value)+1));
                strcpy(createdProjectile->projectileRenderer.texref, stat_value);
                break;
            case P_TEX_ANIM_FRAMES :
                createdProjectile->projectileRenderer.nOfFrames = atoi(stat_value);
                break;
            case P_WEAPON_PERFORANCE :
                createdProjectile->perforance = atoi(stat_value);
                break;
            case P_WEAPON_DAMAGE :
                createdProjectile->damage = atof(stat_value);
                break;
            case P_WEAPON_SPEED :
                createdProjectile->speed = atof(stat_value);
                break;
            case P_WEAPON_TYPE :
                createdProjectile->type = getWeaponTypeFromString(stat_value);
                break;
            case P_NONE :
                break;
        }
    }
    fclose(projectile_file);
    char *textureName = malloc(sizeof(char)*50);
    sprintf(textureName, "p_%s.png", createdProjectile->projectileRenderer.texref);
    createdProjectile->projectileRenderer.texture = GAME->textureManager->getTexture(textureName);
    SDL_QueryTexture(createdProjectile->projectileRenderer.texture, NULL, NULL, &createdProjectile->projectileRenderer.freamWidth, &createdProjectile->projectileRenderer.freamHeight);
    createdProjectile->projectileRenderer.freamWidth /= createdProjectile->projectileRenderer.nOfFrames;
    createdProjectile->projectileRenderer.currentFrame = 0;
    if(createdProjectile->projectileRenderer.nOfFrames > 1){
        createdProjectile->projectileRenderer.animationId = malloc(sizeof(char)*50);
        sprintf(createdProjectile->projectileRenderer.animationId, "P%s%d", createdProjectile->projectileRenderer.texref, createdProjectile->id);
    }
    createdProjectile->isFriendly = isFriendly;
    createdProjectile->width = createdProjectile->projectileRenderer.freamWidth * 2;
    createdProjectile->height = createdProjectile->projectileRenderer.freamHeight * 2;
    createdProjectile->x = xpos - createdProjectile->width/2;
    createdProjectile->y = ypos - createdProjectile->height/2;
    createdProjectile->rotation = rotation;
    createdProjectile->speedx = -createdProjectile->speed * sin(rotation);
    createdProjectile->speedy = -createdProjectile->speed * cos(rotation);
    return createdProjectile;
}

void projectileUpdate(void *self){
    projectile *this = self;
    GameObject *parent = this->parent;
    if(this->type == EXPLOSIVE){
        this->projectileRenderer.currentFrame++;
        if(this->projectileRenderer.currentFrame >= this->projectileRenderer.nOfFrames){
            void areaStrike(void *t){
                GameObject *target = t;
                if(target->type == GOT_Robot){
                    robot *robot = target->actor;
                    float x = this->x + (this->width/2);
                    float y = this->y + (this->height/2);
                    if(sqrt(pow(robot->x - x, 2) + pow(robot->y - y, 2)) < (this->width/2))
                        newHit(this->damage, robot->x, robot->y, this->type, this->parent, target);
                }
            }
            forEach(parent->game->gameObjects, areaStrike);
            this->delete(this);
        }
        return;
    }
    float endPosX = this->speedx+this->x + (this->width/2);
    float endPosY = this->speedy+this->y + (this->height/2);
    list *GameObjects = parent->game->gameObjects;
    this->speedx = -this->speed * sin(this->rotation);
    this->speedy = -this->speed * cos(this->rotation);
    for(int index = 0; index < GameObjects->length; index++){
        GameObject *target = getDataAtIndex(*GameObjects, index);
        if(parent->type == GOT_Robot){
            if(target->type == GOT_Core){
                core *core = target->actor;
                int coreX = core->node->x;
                int coreY = core->node->y;
                float distance = sqrt(pow(coreX-endPosX,2)+pow(coreY-endPosY,2));
                if(distance < core->radius){
                    newHit(this->damage, endPosX, endPosY, this->type, this->parent, target);
                    this->delete(this);
                    return;
                }
            }
        } else if(target->type == GOT_Robot && target->isAlive(target)){
            robot *actor = target->actor;
            float targetX = actor->x;
            float targetY = actor->y;
            float distEndPointCenter = sqrt(pow(endPosX - targetX,2) + pow(endPosY- targetY,2));
            float r = actor->radius;
            if(distEndPointCenter < r){
                newHit(this->damage, endPosX, endPosY, this->type, this->parent, target);
                this->perforance--;
                if(this->perforance == 0){
                    this->delete(this);
                    return;
                }
            }
        }
    }
    SDL_DisplayMode *DM=malloc(sizeof(SDL_DisplayMode));
    SDL_GetCurrentDisplayMode(0, DM);
    int wWidth = DM->w;
    int wHeight = DM->h;
    if(endPosX < -this->width || endPosX > wWidth + this->width || endPosY < -this->height || endPosY > wHeight + this->height){
        this->delete(this);
        return;
    }
    this->x+=this->speedx;
    this->y+=this->speedy;
    if(this->projectileRenderer.nOfFrames > 1){
        this->projectileRenderer.currentFrame++;
        if(this->projectileRenderer.currentFrame >= this->projectileRenderer.nOfFrames){
            this->projectileRenderer.currentFrame = 0;
        }
    }
    if(this->type == PLASMA && this->target){
        GameObject *parent = this->parent;
        GameObject *target = this->target;
        if(searchDataInList(*parent->game->gameObjects, target)){
            if(target->isAlive(target)){
                robot *actor = target->actor;
                float targetX = actor->x;
                float targetY = actor->y;
                float x = this->x + (this->width/2);
                float y = this->y + (this->height/2);
                float rotToTarget = atan2(x-targetX, y-targetY);
                this->rotation = rotToTarget;
                return;
            }
        }
        this->target = NULL;
    }
}

void projectileRender(void *self){
    projectile *this = self;
    GameObject *parent = this->parent;
    SDL_Rect rect={ROUND(this->x)+(this->width/2),ROUND(this->y),this->width, this->height};
    //NEEED TO FIX THIS
    cameraRenderExUnsquared(this->projectileRenderer.texture, rect, this->projectileRenderer.currentFrame, this->projectileRenderer.nOfFrames, -this->rotation*90/(M_PI/2) + 180, false, false);
    
}

void projectileDelete(void *self){
    projectile *this = self;
    free(this->projectileRenderer.texref);
    if(this->projectileRenderer.animationId){
        free(this->projectileRenderer.animationId);
    }
    deleteInList(PROJECTILE_MANAGER->projectiles, this);
    free(this);
}

projectile *createProjectile(void *GAME, char *projectileFileName, float xpos, float ypos, float rotation, void *parent, void *target){
    projectile *createdProjectile = newProjectile(GAME, projectileFileName, xpos, ypos, rotation, true, target);
    createdProjectile->parent = parent;
    createdProjectile->update = projectileUpdate;
    createdProjectile->render = projectileRender;
    createdProjectile->delete = projectileDelete;
    pushInList(PROJECTILE_MANAGER->projectiles, createdProjectile);
    return createdProjectile;
}


static void clearProjectiles(){
    if(PROJECTILE_MANAGER->projectiles->length) forEach(PROJECTILE_MANAGER->projectiles, projectileDelete);
    if(PROJECTILE_MANAGER->hits->length) forEach(PROJECTILE_MANAGER->hits, hitDelete);
}

static void clearManager()
{
    free(PROJECTILE_MANAGER->projectiles);
    free(PROJECTILE_MANAGER->hits);
    free(PROJECTILE_MANAGER);
}

void updateProjectiles(){
    forEach(PROJECTILE_MANAGER->projectiles, projectileUpdate);
}

void renderProjectiles(){
    forEach(PROJECTILE_MANAGER->projectiles, projectileRender);
}

void applyHits(){
    forEach(PROJECTILE_MANAGER->hits, applyHit);
}

projectileManager *initProjectileManager(){
    projectileManager *manager = malloc(sizeof(projectileManager));
    manager->empty = clearProjectiles;
    manager->free = clearManager;
    manager->newProjectile = createProjectile;
    manager->newHit = newHit;
    manager->projectiles = newList(COMPARE_PTR);
    manager->hits = newList(COMPARE_PTR);
    manager->updateProjectiles = updateProjectiles;
    manager->renderProjectiles = renderProjectiles;
    manager->applyHits = applyHits;
    PROJECTILE_MANAGER = manager;
    return manager;
}