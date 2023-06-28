#include <stdlib.h>
#include <string.h>
#include "UI_mainMenu.h"
#include "../Game/game.h"
#include "../Game/selection.h"
#include "UI_settingsMenu.h"
#include <stdbool.h>
#include "../Core/core.h"
#include "../Turrets/turrets.h"
#include "../Robots/robots.h"
#include "UI_progressBar.h"
#include "../Waves/waveManager.h"
#include "../Game/gameManager.h"

static Game *THIS_GAME = NULL;
static float *core_health_percentage = NULL;
static float *core_shield_percentage = NULL;
static float target_health_percentage = 0.0f;
static float target_shield_percentage = 0.0f;
static UI_progressBar *target_health_bar = NULL;
static UI_progressBar *target_shield_bar = NULL;
static UI_button *nextWaveButton = NULL;
static UI_text *text_currencies_1 = NULL;
static UI_text *text_currencies_2 = NULL;
static UI_text *text_currencies_3 = NULL;
static UI_button *HUD_button_speed_1 = NULL;
static UI_button *HUD_button_speed_2 = NULL;
static UI_button *HUD_button_speed_3 = NULL;
static UI_text *HUD_text_wave_info = NULL;
static UI_text *HUD_title_right_part = NULL;
static UI_panel *HUD_right_part_panel = NULL;
static UI_textureObject *HUD_right_part_img = NULL;
static UI_textureObject *HUD_right_part_img_render = NULL;
static UI_panel *HUD_right_part_img_panel = NULL;
static UI_text *HUD_text_1_right_part = NULL;
static UI_text *HUD_text_2_right_part = NULL;
static UI_text *HUD_text_3_right_part = NULL;
static UI_textureObject *HUD_item_1_right_part = NULL;
static UI_textureObject *HUD_item_2_right_part = NULL;
static UI_textureObject *HUD_item_3_right_part = NULL;
static UI_text *HUD_item_text_1_right_part = NULL;
static UI_text *HUD_item_text_2_right_part = NULL;
static UI_text *HUD_item_text_3_right_part = NULL;

static UI_button *HUD_button_right_sell = NULL;
static UI_button *HUD_button_right_upgrade = NULL;
static char **(*LM_getTradById)(char *idToGet) = NULL;

/* Main HUD panel */
static UI_anchor *A_MID_HUD;
static UI_panel *HUD_mid_panel;
int HUD_panel_x(void *none){return 0;}
int HUD_panel_y(void *none){return THIS_GAME->winHeight-0.25*THIS_GAME->winHeight;}

/* Mid HUD panel */
int HUD_mid_panel_x(void *none){return THIS_GAME->winWidth/2-0.09375*THIS_GAME->winWidth;}
int HUD_mid_panel_y(void *none){return THIS_GAME->winHeight-0.25*THIS_GAME->winHeight;}

/* Speed HUD buttons */
int HUD_button_speed_1_x(void *none){return HUD_mid_panel_x(NULL)+0.0625*THIS_GAME->winWidth;}
int HUD_button_speed_1_y(void *none){return HUD_mid_panel_y(NULL)+0.0625*THIS_GAME->winHeight;}

int HUD_button_speed_2_x(void *none){return HUD_mid_panel_x(NULL)+0.09375*THIS_GAME->winWidth;}
int HUD_button_speed_2_y(void *none){return HUD_mid_panel_y(NULL)+0.0625*THIS_GAME->winHeight;}

int HUD_button_speed_3_x(void *none){return HUD_mid_panel_x(NULL)+0.125*THIS_GAME->winWidth;}
int HUD_button_speed_3_y(void *none){return HUD_mid_panel_y(NULL)+0.0625*THIS_GAME->winHeight;}

/* Wave info and Next button */
int HUD_wave_next_x(void *none){return THIS_GAME->winWidth/2;}
int HUD_wave_next_y(void *none){return HUD_mid_panel_y(NULL)+0.15*THIS_GAME->winHeight;}

int HUD_wave_info_x(void *none){return THIS_GAME->winWidth/2;}
int HUD_wave_info_y(void *none){return HUD_mid_panel_y(NULL)+0.21875*THIS_GAME->winHeight;}

/* Health and Shield bar */
static UI_anchor *A_HUD_SHIELD_BAR = NULL;
static UI_anchor *A_HUD_HEALTH_BAR = NULL;
int HUD_shield_bar_x(void *none){return HUD_mid_panel_x(NULL)+THIS_GAME->winWidth*0.015625;}
int HUD_shield_bar_y(void *none){return HUD_mid_panel_y(NULL)+THIS_GAME->winHeight*0.025;}
int HUD_health_bar_x(void *none){return HUD_mid_panel_x(NULL)+THIS_GAME->winWidth*0.153125;}
int HUD_health_bar_y(void *none){return HUD_mid_panel_y(NULL)+THIS_GAME->winHeight*0.025;}

/* Currencies */
static int deltaLi = 45;
static int deltaTi = 90;

int HUD_curr_panel_x(void *none){return HUD_panel_x(NULL);}
int HUD_curr_panel_y(void *none){return HUD_panel_y(NULL);}
int HUD_text_curr_1_x(void *none){return deltaTi;}
int HUD_text_curr_1_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.0625;}
int HUD_text_curr_2_x(void *none){return deltaTi;}
int HUD_text_curr_2_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.125;}
int HUD_text_curr_3_x(void *none){return deltaTi;}
int HUD_text_curr_3_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.1875;}
int HUD_img_curr_1_x(void *none){return deltaLi;}
int HUD_img_curr_1_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.0625-20;}
int HUD_img_curr_2_x(void *none){return deltaLi;}
int HUD_img_curr_2_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.125-20;}
int HUD_img_curr_3_x(void *none){return deltaLi;}
int HUD_img_curr_3_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.1875-20;}

/* Turret Selection */
int HUD_turret_panel_1_x(void *none){return HUD_panel_x(NULL)+THIS_GAME->winWidth*0.121875;}
int HUD_turret_panel_1_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.04375;}
int HUD_turret_panel_2_x(void *none){return HUD_panel_x(NULL)+THIS_GAME->winWidth*0.215625;}
int HUD_turret_panel_2_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.04375;}
int HUD_turret_panel_3_x(void *none){return HUD_panel_x(NULL)+THIS_GAME->winWidth*0.309375;}
int HUD_turret_panel_3_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.04375;}
int HUD_turret_text_1_x(void *none){return HUD_panel_x(NULL)+0.15625*THIS_GAME->winWidth;}
int HUD_turret_text_1_y(void *none){return HUD_panel_y(NULL)+0.2*THIS_GAME->winHeight;}
int HUD_turret_text_2_x(void *none){return HUD_panel_x(NULL)+0.25*THIS_GAME->winWidth;}
int HUD_turret_text_2_y(void *none){return HUD_panel_y(NULL)+0.2*THIS_GAME->winHeight;}
int HUD_turret_text_3_x(void *none){return HUD_panel_x(NULL)+0.34375*THIS_GAME->winWidth;}
int HUD_turret_text_3_y(void *none){return HUD_panel_y(NULL)+0.2*THIS_GAME->winHeight;}

list *HUD_turret_list = NULL;


/* HUD right part */
int HUD_right_part_panel_x(void *none){return THIS_GAME->winWidth-0.39375*THIS_GAME->winWidth;}
int HUD_right_part_panel_y(void *none){return HUD_panel_y(NULL)+0.025*THIS_GAME->winHeight;}
int HUD_right_part_title_x(void *none){return HUD_right_part_panel_x(NULL)+0.0125*THIS_GAME->winWidth;}
int HUD_right_part_title_y(void *none){return HUD_right_part_panel_y(NULL)+0.0375*THIS_GAME->winHeight;}
int HUD_right_part_img_panel_x(void *none){return HUD_right_part_panel_x(NULL)+0.0125*THIS_GAME->winWidth;}
int HUD_right_part_img_panel_y(void *none){return HUD_right_part_panel_y(NULL)+0.05625*THIS_GAME->winHeight;}
int HUD_right_part_text_1_x(void *none){return HUD_right_part_img_panel_x(NULL)+0.095*THIS_GAME->winWidth;}
int HUD_right_part_text_1_y(void *none){return HUD_right_part_img_panel_y(NULL)+0.025*THIS_GAME->winHeight;}
int HUD_right_part_text_2_x(void *none){return HUD_right_part_img_panel_x(NULL)+0.095*THIS_GAME->winWidth;}
int HUD_right_part_text_2_y(void *none){return HUD_right_part_img_panel_y(NULL)+0.0625*THIS_GAME->winHeight;}
int HUD_right_part_text_3_x(void *none){return HUD_right_part_img_panel_x(NULL)+0.095*THIS_GAME->winWidth;}
int HUD_right_part_text_3_y(void *none){return HUD_right_part_img_panel_y(NULL)+0.1*THIS_GAME->winHeight;}

int HUD_right_part_health_x(void *none){return HUD_right_part_text_1_x(NULL);}
int HUD_right_part_health_y(void *none){return HUD_right_part_text_1_y(NULL)-16;}
int HUD_right_part_shield_x(void *none){return HUD_right_part_text_2_x(NULL);}
int HUD_right_part_shield_y(void *none){return HUD_right_part_text_2_y(NULL)-16;}

static int deltaLeftT = 370;
static int deltaLeftI = 340;
static int HUD_right_part_text_cost_1_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftT;}
static int HUD_right_part_text_cost_1_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.0625 + 5;}
static int HUD_right_part_text_cost_2_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftT;}
static int HUD_right_part_text_cost_2_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.0625 + 25;}
static int HUD_right_part_text_cost_3_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftT;}
static int HUD_right_part_text_cost_3_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.0625 + 45;}
static int HUD_right_part_img_cost_1_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftI;}
static int HUD_right_part_img_cost_1_y(void *none){return HUD_right_part_text_cost_1_y(NULL) - 14;}
static int HUD_right_part_img_cost_2_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftI;}
static int HUD_right_part_img_cost_2_y(void *none){return HUD_right_part_text_cost_2_y(NULL) - 14;}
static int HUD_right_part_img_cost_3_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftI;}
static int HUD_right_part_img_cost_3_y(void *none){return HUD_right_part_text_cost_3_y(NULL) - 14;}

static int HUD_right_part_sellbutton_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftT;}
static int HUD_right_part_sellbutton_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.0625 + 110;}
static int HUD_right_part_upgradebutton_x(void *none){return HUD_right_part_text_1_x(NULL) + deltaLeftT;}
static int HUD_right_part_upgradebutton_y(void *none){return HUD_panel_y(NULL)+THIS_GAME->winHeight*0.0625 + 75;}

typedef struct{
    turretSelection *turret;
    int x;
    int y;
    int width;
    int height;
    UI_actionArea *actionArea;
    UI_textureObject *texObj;
    UI_textureObject *texObj_disabled;
    UI_text *text;
    UI_panel *panel;
} turretSelector;

list *turretSelectors = NULL;

void changeSpeed_1_ON(void *none)
{
    THIS_GAME->speedMultiplicator = 2;
}

void changeSpeed_1_OFF(void *none)
{
    if(THIS_GAME->speedMultiplicator == 2)
    {
        THIS_GAME->speedMultiplicator = 1;
    }
    else
    {
        THIS_GAME->speedMultiplicator = 2;
    }
}

void changeSpeed_2_ON(void *none)
{
    THIS_GAME->speedMultiplicator = 3;
}

void changeSpeed_2_OFF(void *none)
{
    if(THIS_GAME->speedMultiplicator == 3)
    {
        THIS_GAME->speedMultiplicator = 1;
    }
    else
    {
        THIS_GAME->speedMultiplicator = 3;
    }
}

void changeSpeed_3_ON(void *none)
{
    THIS_GAME->speedMultiplicator = 4;
}

void changeSpeed_3_OFF(void *none)
{
    if(THIS_GAME->speedMultiplicator == 4)
    {
        THIS_GAME->speedMultiplicator = 1;
    }
    else
    {
        THIS_GAME->speedMultiplicator = 4;
    }
}

static void onClickSell(void *none){
    Selection *sel = THIS_GAME->selection;
    sellTurret(sel->selected.gameObject);
}

static void onClickUpgrade(void *none){
    Selection *sel = THIS_GAME->selection;
    upgradeTurret(sel->selected.gameObject);
}

void nextWave(void *none)
{
    THIS_GAME->waveManager->nextWave();
}

void eventTurretSelector(void *triggeredActionArea)
{
    turretSelector *selected = NULL;
    void getSelectedTurret(void *self){
        turretSelector *this = self;
        if(this->actionArea == triggeredActionArea){
            selected = this;
        }
    }
    forEach(turretSelectors, getSelectedTurret);
    if(selected){
        if(THIS_GAME->selection){
            Selection *currentSelection = THIS_GAME->selection;
            if(currentSelection->type == SELECT_TURRET){
                turretSelection *currentTurret = currentSelection->selected.turretSelection;
                if(currentTurret == selected->turret){
                    free(currentSelection);
                    THIS_GAME->selection = NULL;
                    return;
                }
            }
            free(currentSelection);
            THIS_GAME->selection = NULL;
        }
        Selection *selection = malloc(sizeof(Selection));
        selection->type = SELECT_TURRET;
        selection->selected.turretSelection = selected->turret;
        THIS_GAME->selection = selection;
    }
}

static void cropedAndControlledRender(GameObject *object){
    Game *game = THIS_GAME;
    float camScale = game->cameraScale;
    int camX = game->cameraX;
    int camY = game->cameraY;
    game->cameraScale = 1.0;
    game->cameraX = 0;
    game->cameraY = 0;
    object->render(object);
    game->cameraScale = camScale;
    game->cameraX = camX;
    game->cameraY = camY;
}

static void onUpdate(){
    core *ThisCore = THIS_GAME->coreObj->actor;
    *core_health_percentage = (float)ThisCore->health/(float)ThisCore->maxHealth;
    *core_shield_percentage = (float)ThisCore->shield/(float)ThisCore->maxShield;
    gameModeData data = getGameModeData();
    sprintf(*text_currencies_1->text,"%d",data.currencyA);
    sprintf(*text_currencies_2->text,"%d",data.currencyB);
    sprintf(*text_currencies_3->text,"%d",data.currencyC);
    
    nextWaveButton->isDisabled = THIS_GAME->waveManager->isWaveActive;
    HUD_button_speed_1->isPressed = THIS_GAME->speedMultiplicator > 1;
    HUD_button_speed_2->isPressed = THIS_GAME->speedMultiplicator > 2;
    HUD_button_speed_3->isPressed = THIS_GAME->speedMultiplicator > 3;

    char *tradHudWaveInfo = *LM_getTradById("hud_wave_info");
    sprintf(*HUD_text_wave_info->text,"%s %d",tradHudWaveInfo,THIS_GAME->waveManager->waveNumber);

    Selection *s = THIS_GAME->selection;
    if(s && (s->type == SELECT_TURRET || s->type == SELECT_GAMEOBJECT))
    {
        HUD_title_right_part->hidden = false;
        HUD_right_part_panel->hidden = false;
        HUD_right_part_img_panel->hidden = false;
        HUD_right_part_img_panel->isDisabled = false;
        HUD_text_1_right_part->hidden = false;
        HUD_text_2_right_part->hidden = false;
        HUD_text_3_right_part->hidden = false;
        HUD_button_right_sell->hidden = true;
        HUD_button_right_upgrade->hidden = true;


        int costA = -1;
        int costB = -1;
        int costC = -1;
        if(s->type == SELECT_TURRET)
            {
            HUD_title_right_part->text = LM_getTradById(*s->selected.turretSelection->name);
            HUD_right_part_img_render->hidden = true;
            HUD_right_part_img->hidden = false;

            char *tradTurretRadius = *LM_getTradById("turret_radius");
            sprintf(*HUD_text_1_right_part->text,"%s : %d",tradTurretRadius, s->selected.turretSelection->radius);

            char *tradTurretDamage = *LM_getTradById("turret_damage");
            sprintf(*HUD_text_2_right_part->text,"%s : %d",tradTurretDamage, s->selected.turretSelection->damage);

            char *tradTurretFireRate = *LM_getTradById("turret_fire_rate");
            sprintf(*HUD_text_3_right_part->text,"%s : %d",tradTurretFireRate, s->selected.turretSelection->firerate);

            turretSelection *selected = s->selected.turretSelection;
            HUD_right_part_img->texture = selected->thumbnail;
            costA = selected->costA;
            costB = selected->costB;
            costC = selected->costC;

        }else if(s->type == SELECT_GAMEOBJECT){
            HUD_right_part_img_render->hidden = false;
            HUD_right_part_img->hidden = true;
            GameObject *selected = s->selected.gameObject;
            switch (selected->type)
            {
            case GOT_Turret:
                {   
                    target_health_bar->hidden = true;
                    target_shield_bar->hidden = true;
                    HUD_button_right_sell->hidden = false;
                    turret *turret = selected->actor;
                    HUD_title_right_part->text = LM_getTradById(*turret->name);
                    turret_state *nextState = getDataAtIndex(*turret->states,searchIndexInList(*turret->states,turret->currentState)+1);
                    projectile *currentprojectile = selected->game->projectileManager->newProjectile(selected->game,turret->currentState->projectileName,0,0,0,NULL, NULL);
                    projectile *nextProjectile = NULL;
                    char *tradTurretRadius = *LM_getTradById("turret_radius");
                    char *tradTurretDamage = *LM_getTradById("turret_damage");
                    char *tradTurretFireRate = *LM_getTradById("turret_fire_rate");
                    if(nextState){
                        nextProjectile = selected->game->projectileManager->newProjectile(selected->game,nextState->projectileName,0,0,0,NULL, NULL);
                        costA = nextState->costA;
                        costB = nextState->costB;
                        costC = nextState->costC;
                        sprintf(*HUD_text_1_right_part->text,"%s : %d (> %d)",tradTurretRadius, turret->currentState->range, nextState->range);
                        sprintf(*HUD_text_2_right_part->text,"%s : %d (> %d)",tradTurretDamage, currentprojectile->damage, nextProjectile->damage);
                        sprintf(*HUD_text_3_right_part->text,"%s : %d (> %d)",tradTurretFireRate, 60/(turret->currentState->delay + turret->currentState->canon.nOfFrames), 60/(nextState->delay + nextState->canon.nOfFrames));
                        projectileDelete(nextProjectile);
                        HUD_button_right_upgrade->hidden = false;
                    }else{
                        sprintf(*HUD_text_1_right_part->text,"%s : %d",tradTurretRadius, turret->currentState->range);
                        sprintf(*HUD_text_2_right_part->text,"%s : %d",tradTurretDamage, currentprojectile->damage);
                        sprintf(*HUD_text_3_right_part->text,"%s : %d",tradTurretFireRate, 60/(turret->currentState->delay + turret->currentState->canon.nOfFrames));
                        HUD_button_right_upgrade->hidden = true;
                    }
                    projectileDelete(currentprojectile);
                    if(HUD_right_part_img_render->texture){
                        SDL_DestroyTexture(HUD_right_part_img_render->texture);
                        HUD_right_part_img_render->texture = NULL;
                    }
                    SDL_PixelFormatEnum format = SDL_PIXELFORMAT_RGBA8888;
                    HUD_right_part_img_render->texture = SDL_CreateTexture(selected->game->renderer, format, SDL_TEXTUREACCESS_TARGET,turret->width,turret->height);
                    SDL_SetTextureBlendMode(HUD_right_part_img_render->texture, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderTarget(selected->game->renderer, HUD_right_part_img_render->texture);

                    int oldX = turret->x;
                    int oldY = turret->y;
                    turret->x = 0;
                    turret->y = 0;
                    cropedAndControlledRender(selected);
                    turret->x = oldX;
                    turret->y = oldY;
                    SDL_SetRenderTarget(selected->game->renderer, NULL);
                    break;
                }
            case GOT_Robot:
                {
                    robot *robot = selected->actor;
                    HUD_text_1_right_part->hidden = false;
                    HUD_text_2_right_part->hidden = robot->maxShield == 0;
                    HUD_right_part_img_panel->isDisabled = !robot->isFriendly;
                    HUD_text_3_right_part->hidden = true;
                    target_health_bar->hidden = false;
                    target_shield_bar->hidden = robot->maxShield == 0;
                    HUD_title_right_part->text = LM_getTradById(robot->name);
                    if(robot->life > 0)
                        target_health_percentage = (float)robot->life/(float)robot->maxLife;
                    else target_health_percentage = 0;
                    if(robot->maxShield > 0)
                        target_shield_percentage = (float)robot->shield/(float)robot->maxShield;
                    else target_shield_percentage = 0;
                    if(HUD_right_part_img_render->texture){
                        SDL_DestroyTexture(HUD_right_part_img_render->texture);
                        HUD_right_part_img_render->texture = NULL;
                    }
                    sprintf(*HUD_text_1_right_part->text,"                             %d/%d", robot->life, robot->maxLife);
                    sprintf(*HUD_text_2_right_part->text,"                             %d/%d", robot->shield, robot->maxShield);

                    SDL_PixelFormatEnum format = SDL_PIXELFORMAT_RGBA8888;
                    HUD_right_part_img_render->texture = SDL_CreateTexture(selected->game->renderer, format, SDL_TEXTUREACCESS_TARGET,robot->width,robot->height);
                    SDL_SetTextureBlendMode(HUD_right_part_img_render->texture, SDL_BLENDMODE_BLEND);
                    SDL_SetRenderTarget(selected->game->renderer, HUD_right_part_img_render->texture);

                    float oldX = robot->x;
                    float oldY = robot->y;
                    robot->x = robot->width/2;
                    robot->y = robot->height/2;
                    cropedAndControlledRender(selected);
                    robot->x = oldX;
                    robot->y = oldY;
                    SDL_SetRenderTarget(selected->game->renderer, NULL);
                    break;   
                }
            default:
                break;
            }
        }
        HUD_button_right_upgrade->isDisabled = false;
        if(costA > 0){
                HUD_item_1_right_part->hidden = false;
                HUD_item_text_1_right_part->hidden = false;
                sprintf(*HUD_item_text_1_right_part->text,"%d",costA);
                if(data.currencyA >= costA)
                    HUD_item_text_1_right_part->color = (SDL_Color){255,255,255,255};
                else{
                    HUD_item_text_1_right_part->color = (SDL_Color){255,25,25,255};
                    HUD_button_right_upgrade->isDisabled = true;
                }
        }else{
            HUD_item_1_right_part->hidden = true;
            HUD_item_text_1_right_part->hidden = true;
        }
        if(costB > 0){
            HUD_item_2_right_part->hidden = false;
            HUD_item_text_2_right_part->hidden = false;
            sprintf(*HUD_item_text_2_right_part->text,"%d",costB);
            if(data.currencyB >= costB)
                HUD_item_text_2_right_part->color = (SDL_Color){255,255,255,255};
            else{
                HUD_item_text_2_right_part->color = (SDL_Color){255,25,25,255};
                HUD_button_right_upgrade->isDisabled = true;
            }
        }else{
            HUD_item_2_right_part->hidden = true;
            HUD_item_text_2_right_part->hidden = true;
        }

        if(costC > 0){
            HUD_item_3_right_part->hidden = false;
            HUD_item_text_3_right_part->hidden = false;
            sprintf(*HUD_item_text_3_right_part->text,"%d",costC);
            if(data.currencyC >= costC)
                HUD_item_text_3_right_part->color = (SDL_Color){255,255,255,255};
            else {
                HUD_item_text_3_right_part->color = (SDL_Color){255,25,25,255};
                HUD_button_right_upgrade->isDisabled = true;
            }
        }else{
            HUD_item_3_right_part->hidden = true;
            HUD_item_text_3_right_part->hidden = true;
        }
    } else {
        HUD_title_right_part->hidden = true;
        HUD_right_part_panel->hidden = true;
        HUD_right_part_img_panel->hidden = true;
        HUD_right_part_img->hidden = true;
        HUD_right_part_img_render->hidden = true;
        HUD_text_1_right_part->hidden = true;
        HUD_text_2_right_part->hidden = true;
        HUD_text_3_right_part->hidden = true;

        HUD_item_1_right_part->hidden = true;
        HUD_item_text_1_right_part->hidden = true;
        HUD_item_2_right_part->hidden = true;
        HUD_item_text_2_right_part->hidden = true;
        HUD_item_3_right_part->hidden = true;
        HUD_item_text_3_right_part->hidden = true;

        target_health_bar->hidden = true;
        target_shield_bar->hidden = true;

        HUD_button_right_sell->hidden = true;
        HUD_button_right_upgrade->hidden = true;
    }

    void updateTurretSelector(void *self){
        turretSelector *this = self;
        gameModeData data = getGameModeData();
        if(s && s->type == SELECT_TURRET){
            turretSelection *selected = s->selected.turretSelection;
            if(!strcmp(this->turret->turretId,selected->turretId))
                this->panel->isActive = true;
            else
                this->panel->isActive = false;
        } else this->panel->isActive = false;
        if(data.currencyA >= this->turret->costA && data.currencyB >= this->turret->costB && data.currencyC >= this->turret->costC){
            this->panel->isDisabled = false;
            this->texObj_disabled->hidden = true;
            this->texObj->hidden = false;
        }
        else{
            this->panel->isActive = false;
            this->panel->isDisabled = true;
            this->texObj_disabled->hidden = false;
            this->texObj->hidden = true;
        }
    }
    forEach(turretSelectors, updateTurretSelector);
}

void UI_initHud(void *GAME)
{
    THIS_GAME = GAME;
    Game *game = GAME;
    game->menu = UI_initMenu(GAME);
    game->menu->updateScript = onUpdate;
    LM_getTradById = game->languageManager->getTradById;

    /* Main HUD panel */
    UI_anchor *A_HUD = UI_newAnchor(game->menu, HUD_panel_x, HUD_panel_y);
    UI_panel *HUD_panel = UI_newPanel(game->menu, game->winWidth, 0.25*THIS_GAME->winHeight, A_HUD, 2, UI_PT_A);
    
    /* Mid HUD panel */
    A_MID_HUD = UI_newAnchor(game->menu, HUD_mid_panel_x, HUD_mid_panel_y);
    HUD_mid_panel = UI_newPanel(game->menu, 0.1875*THIS_GAME->winWidth, 0.25*THIS_GAME->winHeight, A_MID_HUD, 2, UI_PT_A);
    HUD_mid_panel->isActive = true;

    /* Speed HUD buttons */
    float speed_size_factor = (THIS_GAME->winWidth > 1200) ? 1.5 : 1.15;
    UI_anchor *A_SPEED_1_HUD = UI_newAnchor(game->menu, HUD_button_speed_1_x, HUD_button_speed_1_y);
    HUD_button_speed_1 = UI_newButton(HUD_mid_panel->menu, NULL, UI_ARROW,A_SPEED_1_HUD,true,NULL,changeSpeed_1_ON,changeSpeed_1_OFF,speed_size_factor);
    UI_flipButton(HUD_button_speed_1,SDL_FLIP_HORIZONTAL);
    UI_anchor *A_SPEED_2_HUD = UI_newAnchor(game->menu, HUD_button_speed_2_x, HUD_button_speed_2_y);
    HUD_button_speed_2 = UI_newButton(HUD_mid_panel->menu, NULL, UI_ARROW,A_SPEED_2_HUD,true,NULL,changeSpeed_2_ON,changeSpeed_2_OFF,speed_size_factor);
    UI_flipButton(HUD_button_speed_2,SDL_FLIP_HORIZONTAL);
    UI_anchor *A_SPEED_3_HUD = UI_newAnchor(game->menu, HUD_button_speed_3_x, HUD_button_speed_3_y);
    HUD_button_speed_3 = UI_newButton(HUD_mid_panel->menu, NULL, UI_ARROW,A_SPEED_3_HUD,true,NULL,changeSpeed_3_ON,changeSpeed_3_OFF,speed_size_factor);
    UI_flipButton(HUD_button_speed_3,SDL_FLIP_HORIZONTAL);

    /* Wave info and Next button */
    int next_font_size = (THIS_GAME->winWidth > 1200) ? 30 : 25;
    float next_size_factor = (THIS_GAME->winWidth > 1200) ? 1.7 : 1.4;
    UI_anchor *A_WAVE_NEXT_HUD = UI_newAnchor(game->menu, HUD_wave_next_x, HUD_wave_next_y);
    nextWaveButton = UI_newButton(HUD_mid_panel->menu,LM_getTradById("hud_next_wave"), UI_B_LONG,A_WAVE_NEXT_HUD,false,nextWave,NULL,NULL,next_size_factor);
    UI_anchor *A_WAVE_INFO_HUD = UI_newAnchor(game->menu, HUD_wave_info_x, HUD_wave_info_y);
    char **strWaveInfo = malloc(sizeof(char*));
    *strWaveInfo = malloc(sizeof(char)*255);
    sprintf(*strWaveInfo,"WAVE X");
    HUD_text_wave_info = UI_newText(game->menu,strWaveInfo,A_WAVE_INFO_HUD, UI_TA_CENTER, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", next_font_size);
    
    /* Health and Shield bar */
    A_HUD_SHIELD_BAR = UI_newAnchor(game->menu, HUD_shield_bar_x, HUD_shield_bar_y);
    A_HUD_HEALTH_BAR = UI_newAnchor(game->menu, HUD_health_bar_x, HUD_health_bar_y);
    core *ThisCore = game->coreObj->actor;
    core_health_percentage = malloc(sizeof(float));
    core_shield_percentage = malloc(sizeof(float));
    *core_health_percentage = (float)ThisCore->health/(float)ThisCore->maxHealth;
    *core_shield_percentage = (float)ThisCore->shield/(float)ThisCore->maxShield;
    UI_progressBar *progressBar_health =  UI_newProgressBar(THIS_GAME,0.01875*THIS_GAME->winWidth, 0.2*THIS_GAME->winHeight,NULL, NULL,A_HUD_HEALTH_BAR,2,1,core_health_percentage,UI_PGB_HEALTH);
    UI_progressBar *progressBar_shield =  UI_newProgressBar(THIS_GAME,0.01875*THIS_GAME->winWidth, 0.2*THIS_GAME->winHeight,NULL, NULL,A_HUD_SHIELD_BAR,2,1,core_shield_percentage,UI_PGB_SHIELD);

    /* Golds */
    UI_anchor *A_PANEL_GOLDS_HUD = UI_newAnchor(game->menu, HUD_curr_panel_x, HUD_curr_panel_y);
    UI_panel *HUD_curr_panel = UI_newPanel(game->menu,0.103125*THIS_GAME->winWidth,0.25*THIS_GAME->winHeight, A_PANEL_GOLDS_HUD, 2, UI_PT_B);

    UI_anchor *A_TEXT_GOLDS_HUD_1 = UI_newAnchor(game->menu, HUD_text_curr_1_x, HUD_text_curr_1_y);
    char **text_curr_1 = malloc(sizeof(char*));
    *text_curr_1 = malloc(sizeof(char)*10);
    int font_size = (THIS_GAME->winWidth > 1200) ? 30 : 25;
    gameModeData data = getGameModeData();

    char **string = malloc(sizeof(char*));
    *string = malloc(sizeof(char)*5);
    sprintf(*string,"%d",data.currencyA);
    text_currencies_1 = UI_newText(game->menu,string,A_TEXT_GOLDS_HUD_1, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", font_size);
    UI_anchor *A_IMG_GOLDS_HUD_1 = UI_newAnchor(game->menu, HUD_img_curr_1_x, HUD_img_curr_1_y);
    UI_textureObject *HUD_img_curr_1 = UI_newStaticTextureObject(game->menu, (SDL_Rect){0,0,32,32},A_IMG_GOLDS_HUD_1,"cur_scrap.png"); // A

    char **string2 = malloc(sizeof(char*));
    *string2 = malloc(sizeof(char)*5);
    sprintf(*string2,"%d",data.currencyA);
    UI_anchor *A_TEXT_GOLDS_HUD_2 = UI_newAnchor(game->menu, HUD_text_curr_2_x, HUD_text_curr_2_y);
    sprintf(*string2,"%d",data.currencyB);
    text_currencies_2 = UI_newText(game->menu,string2,A_TEXT_GOLDS_HUD_2, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", font_size);
    UI_anchor *A_IMG_GOLDS_HUD_2 = UI_newAnchor(game->menu, HUD_img_curr_2_x, HUD_img_curr_2_y);
    UI_textureObject *HUD_img_curr_2 = UI_newStaticTextureObject(game->menu, (SDL_Rect){0,0,32,32},A_IMG_GOLDS_HUD_2,"cur_battery.png"); // B

    char **string3 = malloc(sizeof(char*));
    *string3 = malloc(sizeof(char)*5);
    sprintf(*string3,"%d",data.currencyA);
    UI_anchor *A_TEXT_GOLDS_HUD_3 = UI_newAnchor(game->menu, HUD_text_curr_3_x, HUD_text_curr_3_y);
    sprintf(*string3,"%d",data.currencyC);
    text_currencies_3 = UI_newText(game->menu,string3,A_TEXT_GOLDS_HUD_3, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", font_size);
    UI_anchor *A_IMG_GOLDS_HUD_3 = UI_newAnchor(game->menu, HUD_img_curr_3_x, HUD_img_curr_3_y);
    UI_textureObject *HUD_img_curr_3 = UI_newStaticTextureObject(game->menu, (SDL_Rect){0,0,32,32},A_IMG_GOLDS_HUD_3,"cur_aicore.png"); // C

    /* Turret Selection*/
    UI_anchor *A_PANEL_TURRET_HUD_1 = UI_newAnchor(game->menu, HUD_turret_panel_1_x, HUD_turret_panel_1_y);
    UI_anchor *A_PANEL_TURRET_HUD_2 = UI_newAnchor(game->menu, HUD_turret_panel_2_x, HUD_turret_panel_2_y);
    UI_anchor *A_PANEL_TURRET_HUD_3 = UI_newAnchor(game->menu, HUD_turret_panel_3_x, HUD_turret_panel_3_y);

    UI_anchor *A_TEXT_TURRET_HUD_1 = UI_newAnchor(game->menu, HUD_turret_text_1_x, HUD_turret_text_1_y);
    UI_anchor *A_TEXT_TURRET_HUD_2 = UI_newAnchor(game->menu, HUD_turret_text_2_x, HUD_turret_text_2_y);
    UI_anchor *A_TEXT_TURRET_HUD_3 = UI_newAnchor(game->menu, HUD_turret_text_3_x, HUD_turret_text_3_y);

    HUD_turret_list = generateTurretsSelection(game);
    
    turretSelectors = newList(COMPARE_PTR);
    for(int i = 0; i < 3; i++){
        turretSelection *tmp = getDataAtIndex(*HUD_turret_list, i);
        if(!tmp) break;
        UI_anchor *A_text_tmp;
        UI_anchor *A_panel_tmp;
        turretSelector *tmp_selector = malloc(sizeof(turretSelector));
        switch(i){
            case 0:
                A_text_tmp = A_TEXT_TURRET_HUD_1;
                A_panel_tmp = A_PANEL_TURRET_HUD_1;
                break;
            case 1:
                A_text_tmp = A_TEXT_TURRET_HUD_2;
                A_panel_tmp = A_PANEL_TURRET_HUD_2;
                break;
            case 2:
                A_text_tmp = A_TEXT_TURRET_HUD_3;
                A_panel_tmp = A_PANEL_TURRET_HUD_3;
                break;
        }
        int width = 0.125*THIS_GAME->winHeight;
        int height = 0.125*THIS_GAME->winHeight;
        tmp_selector->text = UI_newText(game->menu,tmp->name,A_text_tmp, UI_TA_CENTER, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 20);
        tmp_selector->panel = UI_newPanel(game->menu,width,height, A_panel_tmp, 2, UI_PT_B);
        tmp_selector->actionArea = UI_newActionArea(game->menu, (SDL_Rect) {0,0, width, height}, A_panel_tmp, eventTurretSelector);
        tmp_selector->texObj = UI_newStaticTextureObjectStatic(game->menu, (SDL_Rect) {0,0, width, height}, A_panel_tmp, tmp->thumbnail);
        tmp_selector->texObj_disabled = UI_newStaticTextureObjectStatic(game->menu, (SDL_Rect) {0,0, width, height}, A_panel_tmp, tmp->forbidden);
        tmp_selector->turret = tmp;
        appendInList(turretSelectors, tmp_selector);
    }

    /* HUD right part */
    UI_anchor *A_TEXT_1_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_text_1_x, HUD_right_part_text_1_y);
    UI_anchor *A_TEXT_2_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_text_2_x, HUD_right_part_text_2_y);
    UI_anchor *A_TEXT_3_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_text_3_x, HUD_right_part_text_3_y);

    UI_anchor *A_HEALTH_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_health_x, HUD_right_part_health_y);
    UI_anchor *A_SHIELD_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_shield_x, HUD_right_part_shield_y);

    target_health_bar = UI_newProgressBar(game, 25, 250, NULL, NULL, A_HEALTH_RIGHT_PART_HUD, 2, false, &target_health_percentage, UI_PGB_HEALTH);
    target_shield_bar = UI_newProgressBar(game, 25, 250, NULL, NULL, A_SHIELD_RIGHT_PART_HUD, 2, false, &target_shield_percentage, UI_PGB_SHIELD);
    
    UI_anchor *A_PANEL_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_panel_x, HUD_right_part_panel_y);
    HUD_right_part_panel = UI_newPanel(game->menu,0.378125*THIS_GAME->winWidth,0.2*THIS_GAME->winHeight, A_PANEL_RIGHT_PART_HUD, 2, UI_PT_A);
    
    UI_anchor *A_TITLE_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_title_x, HUD_right_part_title_y);
    char **title_right_part = malloc(sizeof(char*));
    *title_right_part = malloc(sizeof(char)*255);
    strcpy(*title_right_part,"SHIELD");
    HUD_title_right_part = UI_newText(game->menu,title_right_part,A_TITLE_RIGHT_PART_HUD, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 30);
    
    UI_anchor *A_IMG_RIGHT_PART_HUD = UI_newAnchor(game->menu, HUD_right_part_img_panel_x, HUD_right_part_img_panel_y);
    HUD_right_part_img_panel = UI_newPanel(game->menu,0.125*THIS_GAME->winHeight,0.125*THIS_GAME->winHeight, A_IMG_RIGHT_PART_HUD, 2, UI_PT_B);
    HUD_right_part_img = UI_newStaticTextureObjectStatic(game->menu, (SDL_Rect){0,0,0.125*THIS_GAME->winHeight,0.125*THIS_GAME->winHeight}, A_IMG_RIGHT_PART_HUD, NULL);
    HUD_right_part_img_render = UI_newStaticTextureObjectStatic(game->menu, (SDL_Rect){0,0,0.125*THIS_GAME->winHeight,0.125*THIS_GAME->winHeight}, A_IMG_RIGHT_PART_HUD, NULL);
    char **text_1_right_part = malloc(sizeof(char*));
    *text_1_right_part = malloc(sizeof(char)*255);
    strcpy(*text_1_right_part,"text");
    HUD_text_1_right_part = UI_newText(game->menu,text_1_right_part,A_TEXT_1_RIGHT_PART_HUD, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 30);
    char **text_2_right_part = malloc(sizeof(char*));
    *text_2_right_part = malloc(sizeof(char)*255);
    strcpy(*text_2_right_part,"text");
    HUD_text_2_right_part = UI_newText(game->menu,text_2_right_part,A_TEXT_2_RIGHT_PART_HUD, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 30);
    char **text_3_right_part = malloc(sizeof(char*));
    *text_3_right_part = malloc(sizeof(char)*255);
    strcpy(*text_3_right_part,"text");
    HUD_text_3_right_part = UI_newText(game->menu,text_3_right_part,A_TEXT_3_RIGHT_PART_HUD, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 30);
    
    UI_anchor *A_COST_RIGHT_IMG_1 = UI_newAnchor(game->menu, HUD_right_part_img_cost_1_x, HUD_right_part_img_cost_1_y);
    UI_anchor *A_COST_RIGHT_TEXT_1  = UI_newAnchor(game->menu, HUD_right_part_text_cost_1_x, HUD_right_part_text_cost_1_y);
    char **HUD_costA_string = malloc(sizeof(char*));
    *HUD_costA_string = malloc(sizeof(char)*5);
    sprintf(*HUD_costA_string,"%d",data.currencyA);
    HUD_item_text_1_right_part = UI_newText(game->menu,HUD_costA_string,A_COST_RIGHT_TEXT_1, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 25);
    HUD_item_1_right_part = UI_newStaticTextureObject(game->menu, (SDL_Rect){0,0,20,20},A_COST_RIGHT_IMG_1,"cur_scrap.png");

    UI_anchor *A_COST_RIGHT_IMG_2 = UI_newAnchor(game->menu, HUD_right_part_img_cost_2_x, HUD_right_part_img_cost_2_y);
    UI_anchor *A_COST_RIGHT_TEXT_2  = UI_newAnchor(game->menu, HUD_right_part_text_cost_2_x, HUD_right_part_text_cost_2_y);
    char **HUD_costB_string = malloc(sizeof(char*));
    *HUD_costB_string = malloc(sizeof(char)*5);
    sprintf(*HUD_costB_string,"%d",data.currencyB);
    HUD_item_text_2_right_part = UI_newText(game->menu,HUD_costB_string,A_COST_RIGHT_TEXT_2, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 25);
    HUD_item_2_right_part = UI_newStaticTextureObject(game->menu, (SDL_Rect){0,0,20,20},A_COST_RIGHT_IMG_2,"cur_battery.png");

    UI_anchor *A_COST_RIGHT_IMG_3 = UI_newAnchor(game->menu, HUD_right_part_img_cost_3_x, HUD_right_part_img_cost_3_y);
    UI_anchor *A_COST_RIGHT_TEXT_3  = UI_newAnchor(game->menu, HUD_right_part_text_cost_3_x, HUD_right_part_text_cost_3_y);
    char **HUD_costC_string = malloc(sizeof(char*));
    *HUD_costC_string = malloc(sizeof(char)*5);
    sprintf(*HUD_costC_string,"%d",data.currencyC);
    HUD_item_text_3_right_part = UI_newText(game->menu,HUD_costC_string,A_COST_RIGHT_TEXT_3, UI_TA_LEFT, UI_TJ_CENTER,(SDL_Color){255,255,255,255}, "./assets/fonts/RulerGold.ttf", 25);
    HUD_item_3_right_part = UI_newStaticTextureObject(game->menu, (SDL_Rect){0,0,20,20},A_COST_RIGHT_IMG_3,"cur_aicore.png");

    UI_anchor *A_SELL_RIGHT_BUTTON = UI_newAnchor(game->menu, HUD_right_part_sellbutton_x, HUD_right_part_sellbutton_y);
    HUD_button_right_sell = UI_newButtonIcon(game->menu, UI_B_DIAG, A_SELL_RIGHT_BUTTON, false, onClickSell, NULL, NULL, 1.1, "UI_icon_sell.png");

    UI_anchor *A_UPGRADE_RIGHT_BUTTON = UI_newAnchor(game->menu, HUD_right_part_upgradebutton_x, HUD_right_part_upgradebutton_y);
    HUD_button_right_upgrade = UI_newButtonIcon(game->menu, UI_B_DIAG, A_UPGRADE_RIGHT_BUTTON, false, onClickUpgrade, NULL, NULL, 1.1, "UI_icon_upgrade.png");
    HUD_button_right_upgrade->isDisabled = true;
}