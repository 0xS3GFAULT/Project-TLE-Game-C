#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "rendererAddons.h"
#include "renderSelector.h"
#include "camera.h"
#include "../Turrets/turrets.h"
#include "../Robots/robots.h"
#include "../Core/core.h"
#include "./selection.h"

static void renderTurretGOSelection(Game *game, GameObject *goum){
    turret *turret = goum->actor;
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 100);
    DrawCircle(game->renderer, turret->x + (turret->width/2), turret->y + (turret->height/2), turret->currentState->range);
    SDL_SetRenderDrawColor(game->renderer, 43, 250, 250, 200);
    DrawCircle(game->renderer, turret->x + (turret->width/2), turret->y + (turret->height/2), turret->currentState->range-1);
    DrawCircle(game->renderer, turret->x + (turret->width/2), turret->y + (turret->height/2), turret->currentState->range-2);
    DrawCircle(game->renderer, turret->x + (turret->width/2), turret->y + (turret->height/2), turret->currentState->range-3);
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 100);
    DrawCircle(game->renderer, turret->x + (turret->width/2), turret->y + (turret->height/2), turret->currentState->range-4);
}

static void renderRobotGOSelection(Game *game, GameObject *goum){
    robot *robot = goum->actor;
    int range = robot->width/1.9;
    SDL_SetRenderDrawColor(game->renderer, 250, 43, 43, 200);
    for(range; range > 0; range--){
        DrawCircle(game->renderer, robot->x, robot->y, range);
    }
}

static void renderCoreGOSelection(Game *game, GameObject *goum){

}

void renderSelection(Game *game){
    if(!game->selection) return;
    Selection *selection = game->selection;
    if(selection->type == SELECT_GAMEOBJECT){
        GameObject *goum = selection->selected.gameObject;
        if(goum->type == GOT_Turret){
            renderTurretGOSelection(game, goum);
        }
        else if(goum->type == GOT_Robot){
            renderRobotGOSelection(game, goum);
        }
        else if(goum->type == GOT_Core){
            renderCoreGOSelection(game, goum);
        }
    }
}