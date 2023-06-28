#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "selection.h"
#include "../UI/UI_loading.h"
#include "../UI/UI_mainMenu.h"
#include "../UI/UI_pause.h"
#include "../UI/UI_hud.h"
#include "../Core/core.h"
#include "../Robots/robots.h"
#include "../List/SmartList.h"
#include "gameManager.h"

static Game *GAME = NULL;
static gameModeData data = {0,0,0};
static bool sandbox = false;

void launchMainMenu(void *game){
    GAME = game;
    UI_initMainMenu(game);
}

static GameObject *Core = NULL;

static void launchGameMode(){
    Selection *selection = GAME->selection;
    int mapId = 0;
    if(selection->type == SELECT_MAP){
        mapId = selection->selected.mapId;
    }
    float progress = 0.0f;
    launchLoading(GAME, &progress, "Loading Endless Mode");
    GAME->mapManager->loadMap("final", 1920, 1080, &progress);
    Core = newGameObject_Core(GAME, GAME->mapManager->currentMap->end, 100, 100, 500, 0.5);
    endLoading(GAME);
    UI_initHud(GAME);
    GAME->cameraScale = 0.7f;
    cameraCheckSize();
}

void launchEndlessMode(void *game){
    GAME = game;
    sandbox = false;
    data.currencyA = 50;
    data.currencyB = 0;
    data.currencyC = 0;
    launchGameMode();
}

void launchSandboxMode(void *game){
    GAME = game;
    sandbox = true;
    data.currencyA = 999;
    data.currencyB = 999;
    data.currencyC = 999;
    launchGameMode();
}


void endCurrentMode(){
    if(Core) deleteGameObject(Core);
    Core = NULL;
}

void updateGameManager(){
    if(Core){
        core *coreActor = Core->actor;
        if(!GAME->waveManager->isWaveActive) coreActor->rechargeDelayCounter = 0;
        if(coreActor->health <= 0){
            catchGameOver();
        }
    }
}

bool isGameModeActive(){
    return Core != NULL;
}

gameModeData getGameModeData(void)
{
    return data;
}

void setGameModeData(gameModeData d)
{
    if(sandbox) return;
    if(d.currencyA > 999) d.currencyA = 999;
    if(d.currencyB > 999) d.currencyB = 999;
    if(d.currencyC > 999) d.currencyC = 999;
    data = d;
}