#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <dirent.h>
#include "turrets.h"
#include "turretAi.h"
#include "../Game/game.h"
#include "../Game/camera.h"
#include "../Game/rendererAddons.h"
#include "../Game/gameManager.h"
#include "../Game/projectileManager.h"
#include "../Game/selection.h"
typedef enum{TP_NAME,TP_DESCRIPTION, TP_TEX_REF, TP_TEX_ANIM_FRAMES, TP_FIRE_FRAME, TP_ROTATION_SPEED, TP_ROTATION_ACCELERATION, TP_WEAPON_DELAY, TP_WEAPON_RANGE, TP_WEAPON_PROJECTILE_NAME, TP_NEWSTATE,TP_COST_A,TP_COST_B,TP_COST_C,TP_NONE} turretConfigFileParam;

turretConfigFileParam getTurretConfigFileParamFromString(char *fileParamString){
    if(!strcmp("NAME", fileParamString)) return TP_NAME;
    if(!strcmp("DESCRIPTION", fileParamString)) return TP_DESCRIPTION;
    if(!strcmp("TEX_REF", fileParamString)) return TP_TEX_REF;
    if(!strcmp("TEX_ANIM_FRAMES", fileParamString)) return TP_TEX_ANIM_FRAMES;
    if(!strcmp("FIRE_FRAME", fileParamString)) return TP_FIRE_FRAME;
    if(!strcmp("ROTATION_SPEED", fileParamString)) return TP_ROTATION_SPEED;
    if(!strcmp("ROTATION_ACCELERATION", fileParamString)) return TP_ROTATION_ACCELERATION;
    if(!strcmp("DELAY", fileParamString)) return TP_WEAPON_DELAY;
    if(!strcmp("RANGE", fileParamString)) return TP_WEAPON_RANGE;
    if(!strcmp("PROJECTILE", fileParamString)) return TP_WEAPON_PROJECTILE_NAME;
    if(!strcmp("NEW_STATE", fileParamString)) return TP_NEWSTATE;
    if(!strcmp("COST_A", fileParamString)) return TP_COST_A;
    if(!strcmp("COST_B", fileParamString)) return TP_COST_B;
    if(!strcmp("COST_C", fileParamString)) return TP_COST_C;
    return TP_NONE;
}


turret *newTurret(Game GAME,char *turretFileName, int xpos, int ypos){
    turret *createdTurret = malloc(sizeof(turret));
    
    static int id = 0;
    createdTurret->id = id;
    id++;

    char path[50];
    sprintf(path, "./assets/turrets/%s", turretFileName);
    FILE *turret_file = fopen(path,"r");
    char line_cache[255];
    char stat_name[255];
    char stat_value[255];
    createdTurret->states = newList(COMPARE_PTR);
    turret_state *currentState = NULL;
    int nOfFrame = 1;
    int fireFrame = 0;
    while(fgets(line_cache,255,turret_file) != NULL){
        if(strchr(line_cache,'=') == NULL) sscanf(line_cache," %[^\n]", stat_name);
        else sscanf(line_cache,"%[^ ] = %[^\n]", stat_name, stat_value);
        switch(getTurretConfigFileParamFromString(stat_name)){
            case TP_NAME:
                createdTurret->name = GAME.languageManager->getTradById(stat_value);
                break;
            case TP_DESCRIPTION:
                createdTurret->description = GAME.languageManager->getTradById(stat_value);
                break;
            case TP_TEX_ANIM_FRAMES :
                nOfFrame = atoi(stat_value);
                break;
            case TP_FIRE_FRAME :
                fireFrame = atoi(stat_value);
                break;
            case TP_NEWSTATE:
                currentState = malloc(sizeof(turret_state));
                currentState->texref = malloc(sizeof(char)*255);
                currentState->projectileName = malloc(sizeof(char)*255);
                currentState->canon.textureName = malloc(sizeof(char)*255);
                currentState->base.textureName = malloc(sizeof(char)*255);
                currentState->canon.animationId = malloc(sizeof(char)*255);
                appendInList(createdTurret->states, currentState);
                break;
            case TP_TEX_REF:
                strcpy(currentState->texref, stat_value);
                sprintf(currentState->base.textureName, "tur_%s_base.png", stat_value);
                currentState->base.texture = GAME.textureManager->getTexture(currentState->base.textureName);
                SDL_QueryTexture(currentState->base.texture, NULL, NULL, &currentState->base.frameWidth, &currentState->base.frameHeight);
                sprintf(currentState->canon.textureName, "tur_%s_can_anim.png", stat_value);
                currentState->canon.texture = GAME.textureManager->getTexture(currentState->canon.textureName);
                SDL_QueryTexture(currentState->canon.texture, NULL, NULL, &currentState->canon.frameWidth, &currentState->canon.frameHeight);
                currentState->canon.nOfFrames = nOfFrame;
                currentState->canon.frameWidth /= nOfFrame;
                currentState->canon.currentFrame = 0;
                currentState->canon.fireFrame = fireFrame;
                createdTurret->width = currentState->canon.frameWidth * 2;
                createdTurret->height = currentState->canon.frameHeight * 2;
                currentState->canon.animationId = malloc(sizeof(char)*50);
                sprintf(currentState->canon.animationId, "T%s%d", currentState->texref,createdTurret->id);
                break;
            case TP_ROTATION_SPEED :
                currentState->maxRotationSpeed = atof(stat_value);
                break;
            case TP_ROTATION_ACCELERATION :
                currentState->maxRotationAcceleration = atof(stat_value);
                break;
            case TP_WEAPON_DELAY :
                currentState->delay = atof(stat_value);
                break;    
            case TP_WEAPON_RANGE:
                currentState->range = atof(stat_value);
                break;
            case TP_WEAPON_PROJECTILE_NAME:
                sprintf(currentState->projectileName, "%s.projectile", stat_value);
                break;
            case TP_COST_A:
                currentState->costA = atoi(stat_value);
                break;
            case TP_COST_B:
                currentState->costB = atoi(stat_value);
                break;
            case TP_COST_C:
                currentState->costC = atoi(stat_value);
                break;
            default:
                break;
        }
    }
    fclose(turret_file);
    createdTurret->currentState = getDataAtIndex(*createdTurret->states, 0);
    createdTurret->x = xpos;
    createdTurret->y = ypos;
    createdTurret->rotation = 0.0;
    createdTurret->turretLock = false;
    return createdTurret;
}

void turretUpdate(void *self){
    GameObject *thisGameObject = self;
    turret *thisTurret = thisGameObject->actor;
    if(thisTurret->currentState->range > 700) updateArtilleryAi(thisGameObject);
    else updateTurretAi(thisGameObject);
}

void turretRender(void *self){
    GameObject *thisGameObject = self;
    turret *this = thisGameObject->actor;
    SDL_Rect rect={this->x,this->y,this->width,this->height};
    
    cameraRender(this->currentState->base.texture, rect);
    cameraRenderEx(this->currentState->canon.texture, rect, this->currentState->canon.currentFrame, -this->rotation*90/(M_PI/2), false, false);
}

void stateDestroy(void *self){
    turret_state *this = self;
    free(this->texref);
    free(this->projectileName);
    free(this->canon.textureName);
    free(this->base.textureName);
    free(this->canon.animationId);
    free(this);
}

void turretDelete(void *self){
    GameObject *thisGameObject = self;
    turret *this = thisGameObject->actor;
    forEach(this->states, stateDestroy);
    freeList(this->states);
    deleteInList(thisGameObject->game->gameObjects, thisGameObject);
    free(this);
    Selection *selection = thisGameObject->game->selection;
    if(selection && selection->type == SELECT_GAMEOBJECT){
        if(selection->selected.gameObject == thisGameObject){
            free(selection);
            thisGameObject->game->selection = NULL;
        }
    }
}

bool turretIsAlive(void *self){
    return true;
}

GameObject *newGameObject_Turret(Game *GAME, char *turretFileName, int xpos, int ypos){
    GameObject *gameObject = malloc(sizeof(GameObject));
    gameObject->actor = newTurret(*GAME, turretFileName, xpos, ypos);
    gameObject->type = GOT_Turret;
    gameObject->game = GAME;
    gameObject->update = turretUpdate;
    gameObject->render = turretRender;
    gameObject->delete = turretDelete;
    gameObject->isAlive = turretIsAlive;
    appendInList(GAME->gameObjects, gameObject);
    return gameObject;
}

turretSelection *newTurretSelection(Game *GAME, char *turretFileName){
    turretSelection *createdTurretSelection = malloc(sizeof(turretSelection));
    createdTurretSelection->x = 0;
    createdTurretSelection->y = 0;
    createdTurretSelection->turretId = malloc(sizeof(char)*strlen(turretFileName)+1);
    strcpy(createdTurretSelection->turretId, turretFileName);
    turret *srcturret = newTurret(*GAME, turretFileName, 0, 0);
    createdTurretSelection->width = srcturret->width;
    createdTurretSelection->height = srcturret->height;
    createdTurretSelection->radius = srcturret->currentState->range;
    createdTurretSelection->name = srcturret->name;
    createdTurretSelection->costA = srcturret->currentState->costA;
    createdTurretSelection->costB = srcturret->currentState->costB;
    createdTurretSelection->costC = srcturret->currentState->costC;
    int fireDelay = srcturret->currentState->delay + srcturret->currentState->canon.nOfFrames;
    createdTurretSelection->firerate = 60/fireDelay;
    projectile *srcprojectile = GAME->projectileManager->newProjectile(GAME, srcturret->currentState->projectileName, 0, 0, 0, NULL, NULL);
    createdTurretSelection->damage = srcprojectile->damage;
    projectileDelete(srcprojectile);

    SDL_PixelFormatEnum format = SDL_PIXELFORMAT_RGBA8888;
    // SDL_QueryTexture(srcturret->currentState->base.texture, &format, NULL, NULL, NULL);

    createdTurretSelection->thumbnail = SDL_CreateTexture(GAME->renderer, format, SDL_TEXTUREACCESS_TARGET, srcturret->currentState->canon.frameWidth, srcturret->currentState->canon.frameHeight);
    SDL_SetTextureBlendMode(createdTurretSelection->thumbnail, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(GAME->renderer, createdTurretSelection->thumbnail);
    SDL_Rect can_rect;
    SDL_Rect base_rect;
    if(srcturret->currentState->base.texture)
    {
        base_rect = (SDL_Rect) {0,0,srcturret->currentState->base.frameWidth,srcturret->currentState->base.frameHeight};
        SDL_RenderCopy(GAME->renderer, srcturret->currentState->base.texture, &base_rect, NULL);
    }
    if(srcturret->currentState->canon.texture)
    {
        can_rect = (SDL_Rect) {0,0,srcturret->currentState->canon.frameWidth,srcturret->currentState->canon.frameHeight};
        SDL_RenderCopy(GAME->renderer, srcturret->currentState->canon.texture, &can_rect, NULL);
    }

    createdTurretSelection->allowed = SDL_CreateTexture(GAME->renderer, format, SDL_TEXTUREACCESS_TARGET, srcturret->currentState->canon.frameWidth, srcturret->currentState->canon.frameHeight);
    SDL_SetTextureBlendMode(createdTurretSelection->allowed, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(GAME->renderer, createdTurretSelection->allowed);
    if(srcturret->currentState->base.texture){
        SDL_SetTextureAlphaMod(srcturret->currentState->base.texture, 128);
        SDL_RenderCopy(GAME->renderer, srcturret->currentState->base.texture, &base_rect, NULL);
        SDL_SetTextureAlphaMod(srcturret->currentState->base.texture, 255);
    }
    if(srcturret->currentState->canon.texture){
        SDL_SetTextureAlphaMod(srcturret->currentState->canon.texture, 128);
        SDL_RenderCopy(GAME->renderer, srcturret->currentState->canon.texture, &can_rect, NULL);
        SDL_SetTextureAlphaMod(srcturret->currentState->canon.texture, 255);
    }
    SDL_SetTextureColorMod(createdTurretSelection->allowed, 0, 0, 255);
    createdTurretSelection->forbidden = SDL_CreateTexture(GAME->renderer, format, SDL_TEXTUREACCESS_TARGET, srcturret->currentState->canon.frameWidth, srcturret->currentState->canon.frameHeight);
    SDL_SetTextureBlendMode(createdTurretSelection->forbidden, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(GAME->renderer, createdTurretSelection->forbidden);
    if(srcturret->currentState->base.texture){
        SDL_SetTextureAlphaMod(srcturret->currentState->base.texture, 150);
        SDL_RenderCopy(GAME->renderer, srcturret->currentState->base.texture, &base_rect, NULL);
        SDL_SetTextureAlphaMod(srcturret->currentState->base.texture, 255);
    }
    if(srcturret->currentState->canon.texture){
        SDL_SetTextureAlphaMod(srcturret->currentState->canon.texture, 200);
        SDL_RenderCopy(GAME->renderer, srcturret->currentState->canon.texture, &can_rect, NULL);
        SDL_SetTextureAlphaMod(srcturret->currentState->canon.texture, 255);
    }    
    SDL_SetTextureColorMod(createdTurretSelection->forbidden, 255, 0, 0);
    SDL_SetRenderTarget(GAME->renderer, NULL);
    free(srcturret);

    return createdTurretSelection;
}

void upgradeTurret(GameObject *this){
    if(!this) return;
    if(this->type != GOT_Turret) return;
    turret *turret = this->actor;
    int state = searchIndexInList(*turret->states,turret->currentState);
    turret_state *next = getDataAtIndex(*turret->states,state+1);
    gameModeData data = getGameModeData();
    if(next){
        if(next->costA <= data.currencyA && next->costB <= data.currencyB && next->costC <= data.currencyC){
            turret->currentState = next;
            data.currencyA -= turret->currentState->costA;
            data.currencyB -= turret->currentState->costB;
            data.currencyC -= turret->currentState->costC;
            setGameModeData(data);
        }
    }
}

void sellTurret(GameObject *this){
    if(!this) return;
    if(this->type != GOT_Turret) return;
    turret *turret = this->actor;
    gameModeData data = getGameModeData();
    data.currencyA += turret->currentState->costA;
    data.currencyB += turret->currentState->costB;
    data.currencyC += turret->currentState->costC;
    turretDelete(this);
    setGameModeData(data);
}

list *generateTurretsSelection(Game *GAME){
    list *turrets = newList(COMPARE_PTR);
    DIR *dir = opendir("./assets/turrets");
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        if(strstr(ent->d_name, ".turret") != NULL){
            turretSelection *turret = newTurretSelection(GAME, ent->d_name);
            appendInList(turrets, turret);
        }
    }
    closedir(dir);
    int compareTurretSelection(void *a, void *b){
        turretSelection *a_turret = (turretSelection *)a;
        turretSelection *b_turret = (turretSelection *)b;
        return a_turret->costA - b_turret->costA;
    }
    sortList(turrets, compareTurretSelection, true);
    // sort list by cost
    return turrets;
}
