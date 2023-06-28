#include <stdlib.h>
#include "UI_pause.h"
#include "../Game/game.h"
#include "../Game/selection.h"
#include "../Game/gameManager.h"
#include "UI_settingsMenu.h"
#include "UI_mainMenu.h"

static char** (*LM_getTradById)(char *idToGet) = NULL;
static UI_menu *pause_menu = NULL; 
static UI_menu *hud = NULL;
static Game *game = NULL;
static UI_textureObject *pauseBackground = NULL;
static SDL_Texture *background = NULL;
static UI_textureObject *menuLogo = NULL;

static int logo_x(void *none){return game->winWidth-700;} 
static int logo_y(void *none){return game->winHeight/2-100;} 

static int NULL_x(void *none){return 0;}
static int NULL_y(void *none){return 0;}

static int continue_x(void *none){return 220;}
static int continue_y(void *none){return game->winHeight/2-150;}

static int settings_x(void *none){return 220;}
static int settings_y(void *none){return game->winHeight/2;}

static int quit_x(void *none){return 220;}
static int quit_y(void *none){return game->winHeight/2+150;}

static int title_x(void *none){return game->winWidth/2;}
static int title_y(void *none){return 70;}

static UI_anchor *title_anchor = NULL;
static UI_text *title = NULL;
static UI_text *title_Game_Over = NULL;
static UI_anchor *anchor_continue = NULL;
static UI_button *button_continue = NULL;
static UI_anchor *anchor_settings = NULL;
static UI_button *button_settings = NULL;
static UI_anchor *anchor_quit = NULL;
static UI_button *button_quit = NULL;
static UI_button *button_quit_game_over = NULL;
static UI_anchor *a_logo = NULL;


void clickContinue(void *self)
{
    throwPause();
    game->pause = false;
}

void clickSettings(void *self)
{
    UI_switchToSettings(game);
}

void clickQuit(void *self)
{
    endCurrentMode();
    game->selection = NULL;
    forEach(game->gameObjects, deleteGameObject);
    forEach(game->animationManager->animList, freeAnimation);
    game->projectileManager->empty();
    game->mapManager->unloadMap();
    game->pause = false;
    game->speedMultiplicator = 1;
    WM_killWave();
    UI_initMainMenu(game);
}

void gameOverToMainMenu(void *self)
{
    game->pause = false;
    game->selection = NULL;
    UI_initMainMenu(game);
}

void pauseInit(void *g)
{
    game = g;
    LM_getTradById = game->languageManager->getTradById;
    pause_menu = UI_initMenu(game);
    
    //UI_anchor *a = UI_newAnchor(pause_menu,logo_x,NULL_y);
    //pauseBackground = UI_newStaticTextureObjectStatic(pause_menu,(SDL_Rect){0,0,game->winWidth,game->winHeight},a,NULL);

    title_anchor = UI_newAnchor(pause_menu,title_x,title_y);
    title = UI_newText(pause_menu,LM_getTradById("PAUSE"),title_anchor,UI_TA_CENTER,UI_TJ_CENTER,(SDL_Color){255,255,255,255},"./assets/fonts/RulerGold.ttf",80);
    title_Game_Over = UI_newText(pause_menu,LM_getTradById("GAME_OVER"),title_anchor,UI_TA_CENTER,UI_TJ_CENTER,(SDL_Color){255,255,255,255},"./assets/fonts/RulerGold.ttf",80);
    anchor_continue = UI_newAnchor(pause_menu,continue_x,continue_y);
    button_continue =  UI_newButton(pause_menu,LM_getTradById("pause_continue"),UI_B_BIG,anchor_continue,false,clickContinue,NULL,NULL,2);

    anchor_settings = UI_newAnchor(pause_menu,settings_x,settings_y);
    button_settings =  UI_newButton(pause_menu,LM_getTradById("pause_settings"),UI_B_BIG,anchor_settings,false,clickSettings,NULL,NULL,2);

    anchor_quit = UI_newAnchor(pause_menu,quit_x,quit_y);
    button_quit =  UI_newButton(pause_menu,LM_getTradById("pause_quit"),UI_B_BIG,anchor_quit,false,clickQuit,NULL,NULL,2);
    button_quit_game_over =  UI_newButton(pause_menu,LM_getTradById("pause_quit"),UI_B_BIG,anchor_quit,false,gameOverToMainMenu,NULL,NULL,2);
    

    a_logo = UI_newAnchor(pause_menu,logo_x,logo_y);
    menuLogo = UI_newStaticTextureObject(pause_menu, (SDL_Rect) {400,400,400,200}, a_logo, "TLELogo.png");

}

void catchPause()
{
    hud = game->menu;
    game->menu = pause_menu;
    SDL_ShowCursor(SDL_ENABLE);
    title->hidden = false;
    title_Game_Over->hidden = true;
    button_continue->hidden = false;
    button_settings->hidden = false;
    button_quit->hidden = false;
    menuLogo->hidden = false;
    button_quit_game_over->hidden = true;

    SDL_PixelFormatEnum format = SDL_PIXELFORMAT_RGBA8888;
    if(background)free(background);

    UI_refreshMenu(game->menu);
    //pauseBackground->texture = background;
}

void throwPause()
{
    game->menu = hud;
    UI_refreshMenu(game->menu);
    hud = NULL;
}

void catchGameOver(){
        
    hud = game->menu;
    game->menu = pause_menu;
    SDL_ShowCursor(SDL_ENABLE);
    endCurrentMode();
    forEach(game->gameObjects, deleteGameObject);
    forEach(game->animationManager->animList, freeAnimation);
    game->projectileManager->empty();
    game->mapManager->unloadMap();
    game->pause = false;
    WM_killWave();
    
    title->hidden = true;
    title_Game_Over->hidden = false;
    button_continue->hidden = true;
    button_settings->hidden = true;
    button_quit->hidden = true;
    menuLogo->hidden = false;
    button_quit_game_over->hidden = false;

    Selection *s = game->selection;
    if(s) free(s);
    game->selection = NULL;
    UI_refreshMenu(game->menu);
}