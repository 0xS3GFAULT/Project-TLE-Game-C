#include "UI_mainMenu.h"
#include "UI_settingsMenu.h"

#include "../Game/game.h"
#include "../Game/selection.h"
#include "../Game/gameManager.h" 

static Game *THIS_GAME = NULL;

static UI_button *playButton = NULL;
static UI_button *settingsButton = NULL;
static UI_button *quitButton = NULL;
static UI_button *aboutButton = NULL;
static UI_button *endlessButton = NULL;
static UI_button *sandboxButton = NULL;
static UI_button *backButton = NULL;

int A_RNULL(void *GAME){return 0;}
int DIAG_X(void *GAME){return 240;}
int getx_middle(void *GAME){Game *game = GAME; return game->winWidth/2;}
int gety_middle(void *GAME){Game *game = GAME; return game->winHeight/2;}
int logo_x(void *GAME){Game *game = GAME; return game->winWidth-700;}
int logo_y(void *GAME){Game *game = GAME; return game->winHeight/2-100;}
static const int menuInBetween = 150;

int gety_top(void *GAME){return gety_middle(GAME) - menuInBetween;}
int gety_bottom(void *GAME){return gety_middle(GAME) + menuInBetween;}

int gety_firstInRow(void *GAME){return gety_middle(GAME) - (2 * menuInBetween);}
int gety_secondInRow(void *GAME){return gety_middle(GAME) - menuInBetween;}
int gety_thirdInRow(void *GAME){return gety_middle(GAME);}
int gety_bottomOfRow(void *GAME){return gety_middle(GAME) + menuInBetween;}

void onClickPrintf(void *self){
    printf("CLICK ON %p\n", self);
}

void showPlayMenu(){
    playButton->hidden = true;
    settingsButton->hidden = true;
    quitButton->hidden = true;
    // aboutButton->hidden = true;
}

void hidePlayMenu(){
    playButton->hidden = false;
    settingsButton->hidden = false;
    quitButton->hidden = false;
    // aboutButton->hidden = false;
}

void showHomeMenu(){
    endlessButton->hidden = true;
    sandboxButton->hidden = true;
    backButton->hidden = true;
}

void hideHomeMenu(){
    endlessButton->hidden = false;
    sandboxButton->hidden = false;
    backButton->hidden = false;
}

void switchToPlayMenu(void *self){
    showPlayMenu();
    hideHomeMenu();
}

void switchToHomeMenu(void *self){
    showHomeMenu();
    hidePlayMenu();
}

void exitGame(void *self){
    THIS_GAME->isRunning = false;
}

static UI_textureObject *menuBackground = NULL;
static UI_textureObject *menuLogo = NULL;
static char **(*LM_getTradById)(char *idToGet) = NULL;

void rescaleBackground(){
    int w = 1920;
    int h = 1080;
    float wRatio = THIS_GAME->winWidth / (float) w;
    float hRatio = THIS_GAME->winHeight / (float) h;
    if(h * wRatio >= THIS_GAME->winHeight){
        menuBackground->rect.w = w * wRatio;
        menuBackground->rect.h = h * wRatio;
    }else{
        menuBackground->rect.w = w * hRatio;
        menuBackground->rect.h = h * hRatio;
    }
}

void onUpdate(){
    rescaleBackground();
}

void switchToSettings()
{
    UI_switchToSettings(THIS_GAME);
}

void GM_launchEndless(void *self){
    if(THIS_GAME->selection){
        Selection *toFree = THIS_GAME->selection;
        free(toFree);
    }
    Selection *selection = malloc(sizeof(Selection));
    selection->type = SELECT_MAP;
    selection->selected.mapId = 0;
    THIS_GAME->selection = selection;
    launchEndlessMode(THIS_GAME);
}

void GM_launchSandbox(void *self){
    if(THIS_GAME->selection){
        Selection *toFree = THIS_GAME->selection;
        free(toFree);
    }
    Selection *selection = malloc(sizeof(Selection));
    selection->type = SELECT_MAP;
    selection->selected.mapId = 1;
    THIS_GAME->selection = selection;
    launchSandboxMode(THIS_GAME);
}


void UI_initMainMenu(void *GAME)
{
    THIS_GAME = GAME;
    Game *game = GAME;
    game->menu = UI_initMenu(GAME);
    game->menu->updateScript = onUpdate;
    LM_getTradById = game->languageManager->getTradById;

    UI_anchor *A_NULL = UI_newAnchor(game->menu, A_RNULL, A_RNULL);
    UI_anchor *a_logo = UI_newAnchor(game->menu,logo_x,logo_y);
    UI_anchor *AH_PLAY = UI_newAnchor(game->menu, DIAG_X, gety_top);
    UI_anchor *AH_SETTINGS = UI_newAnchor(game->menu, DIAG_X, gety_middle);
    UI_anchor *AH_QUIT = UI_newAnchor(game->menu, DIAG_X, gety_bottom);
    // UI_anchor *AH_ABOUT = UI_newAnchor(game->menu, ?, ?);

  
    UI_anchor *AP_ENDLESS = UI_newAnchor(game->menu, DIAG_X, gety_secondInRow);
    UI_anchor *AP_SANDBOX = UI_newAnchor(game->menu, DIAG_X, gety_thirdInRow);
    UI_anchor *AP_BACK = UI_newAnchor(game->menu, DIAG_X, gety_bottomOfRow);

    menuBackground = UI_newStaticTextureObject(game->menu, (SDL_Rect) {0,0,1920,1080}, A_NULL, "menu_background.png");
    menuLogo = UI_newStaticTextureObject(game->menu, (SDL_Rect) {400,400,400,200}, a_logo, "TLELogo.png");
    // menuBackground->hidden = true;

    //Main menu buttons
    playButton = UI_newButton(THIS_GAME->menu, LM_getTradById("menu_home_play"), UI_B_BIG, AH_PLAY, false, switchToPlayMenu, NULL, NULL, 2);
    settingsButton = UI_newButton(THIS_GAME->menu, LM_getTradById("menu_home_settings"), UI_B_BIG, AH_SETTINGS, false, switchToSettings, NULL, NULL, 2);
    quitButton = UI_newButton(THIS_GAME->menu, LM_getTradById("menu_home_quit"), UI_B_BIG, AH_QUIT, false, exitGame, NULL, NULL, 2);
    // aboutButton = UI_newButton(THIS_GAME->menu, LM_getTradById("menu_home_about"), UI_B_BIG, AH_ABOUT, false, onClickPrintf, NULL, NULL, 3);

    //Play menu buttons
    endlessButton = UI_newButton(THIS_GAME->menu, LM_getTradById("menu_home_endless"), UI_B_BIG, AP_ENDLESS, false, GM_launchEndless, NULL, NULL, 2);
    sandboxButton = UI_newButton(THIS_GAME->menu, LM_getTradById("menu_home_sandbox"), UI_B_BIG, AP_SANDBOX, false, GM_launchSandbox, NULL, NULL, 2);
    backButton = UI_newButton(THIS_GAME->menu, LM_getTradById("menu_back"), UI_B_BACK, AP_BACK, false, switchToHomeMenu, NULL, NULL, 1.5);

    switchToHomeMenu(NULL);
}