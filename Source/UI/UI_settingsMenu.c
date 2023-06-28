#include "../Game/game.h"
#include <stdbool.h>
#include "../Language/lang.h"
#include "../keyBindings/keyBindings.h"


static Game *THIS_GAME = NULL;
static UI_menu *backStateMenu = NULL;

static list *KB_button_bindings = NULL;

typedef struct 
{
    UI_button *button;
    GA_type action;
}buttonBinding;

int getLT_x(void *none){return 200;}
int getLT_y(void *none){return 100;}

//CORE PANEL
UI_panel *st_panel = NULL;
static UI_button *st_button_back = NULL;

//GENERAL PANEL
static UI_panelButton *st_button_general = NULL;
static UI_text *st_text_general_title = NULL;

//DISPLAY PANEL
static UI_panelButton *st_button_display = NULL;
static UI_text *st_text_display_title = NULL;

//RESOLUTION
static UI_text *st_text_resolution = NULL;
static UI_text *st_text_fullscreen = NULL;
static UI_button *st_button_fullscreen = NULL;

//AUDIO PANEL
static UI_panelButton *st_button_audio = NULL;
static UI_text *st_text_audio_title = NULL;

static float screen_size = 0.5;
static float master_volume = 0.5;
static float music_volume = 0.5;
static float effects_volume = 0.5;
static float dialogues_volume = 0.5;
static UI_text *st_text_master = NULL;
static UI_text *st_text_music = NULL;
static UI_text *st_text_effects = NULL;
static UI_text *st_text_dialogues = NULL;

static UI_slider *st_slider_master = NULL;
static UI_slider *st_slider_music = NULL;
static UI_slider *st_slider_effects = NULL;
static UI_slider *st_slider_dialogues = NULL;


//KEYS MAP PANEL
static UI_panelButton *st_button_commands = NULL;
static UI_text *st_text_commands_title = NULL;
static UI_text *st_text_command_up = NULL;
static UI_text *st_text_command_down = NULL;
static UI_text *st_text_command_right = NULL;
static UI_text *st_text_command_left = NULL;
static UI_text *st_text_command_zoom_in = NULL;
static UI_text *st_text_command_zoom_out = NULL;
static UI_text *st_text_command_turret_1 = NULL;
static UI_text *st_text_command_turret_2 = NULL;
static UI_text *st_text_command_turret_3 = NULL;
static UI_text *st_text_command_pause = NULL;
static UI_text *st_text_command_sell = NULL;
static UI_text *st_text_command_upgrade = NULL;
    

//ACCESSIBILITY PANEL
static UI_panelButton *st_button_accessibility = NULL;
static UI_text *st_text_accessibility_title = NULL;

static UI_text *st_text_language = NULL;
static UI_text *st_text_daltonism = NULL;
static UI_text *st_text_screen_shake = NULL;
static UI_text *st_text_inverse_x_axis = NULL;
static UI_text *st_text_inverse_y_axis = NULL;
static UI_text *st_text_text_size = NULL;

static UI_button *st_arrow_left_carousel = NULL;
static UI_button *st_arrow_right_carousel = NULL;

static UI_button *st_arrow_left_carousel_resolution = NULL;
static UI_button *st_arrow_right_carousel_resolution = NULL;

static UI_button *st_arrow_left_carousel_daltonism = NULL;
static UI_button *st_arrow_right_carousel_daltonism = NULL;

static UI_button *st_button_accessibility_screen_shake = NULL;
static UI_button *st_button_accessibility_invert_x_axis = NULL;
static UI_button *st_button_accessibility_invert_y_axis = NULL;


static char **(*LM_getTradById)(char *string) = NULL;

int getPannelMiddle_x(void *none){return THIS_GAME->winWidth/2;}
int getPannelMiddle_y(void *none){return THIS_GAME->winHeight/2;}

int back_button_anchor_x(void *non){return getPannelMiddle_x(NULL)+st_panel->width/2-125;}
int back_button_anchor_y(void *non){return getPannelMiddle_y(NULL)+st_panel->height/2+40;}

int col_left_x(void *none){return getLT_x(NULL)+200;}
int col_right_x(void *none){return  getLT_x(NULL)+st_panel->width-400;}
int col_left_x_command_button(void *none){return col_left_x(NULL)-80;}
int col_right_x_command_button(void *none){return col_right_x(NULL)-80;}

int x_arrow_left(void *none){return col_right_x(NULL)-50;}
int x_arrow_right(void *none){return col_right_x(NULL) + 300;}

int st_title_x(void *none){return getLT_x(NULL)+50;}
int st_title_y(void *none){return getLT_y(NULL)+50;}

int st_text_row_1(void *none){return getPannelMiddle_y(NULL)-200;}
int st_text_row_2(void *none){return getPannelMiddle_y(NULL)-140;}
int st_text_row_3(void *none){return getPannelMiddle_y(NULL)-80;}
int st_text_row_4(void *none){return getPannelMiddle_y(NULL)-20;}
int st_text_row_5(void *none){return getPannelMiddle_y(NULL)+40;}
int st_text_row_6(void *none){return getPannelMiddle_y(NULL)+100;}
int st_text_row_7(void *none){return getPannelMiddle_y(NULL)+160;}
int st_text_row_1_button(void *none){return getPannelMiddle_y(NULL)-205;}
int st_text_row_2_button(void *none){return getPannelMiddle_y(NULL)-145;}
int st_text_row_3_button(void *none){return getPannelMiddle_y(NULL)-85;}
int st_text_row_4_button(void *none){return getPannelMiddle_y(NULL)-25;}
int st_text_row_5_button(void *none){return getPannelMiddle_y(NULL)+35;}
int st_text_row_6_button(void *none){return getPannelMiddle_y(NULL)+95;}
int st_text_row_7_button(void *none){return getPannelMiddle_y(NULL)+155;}

int x_panel_carousel(void *none){return col_right_x(NULL) + 20;}
int y_panel_carousel(void *none){return st_text_row_2(NULL) - 22;}
UI_anchor *st_panel_carousel_anchor = NULL;
UI_panel *st_panel_carousel_lang = NULL;

int x_panel_carousel_text(void *none){return col_right_x(NULL) + 125;}
int y_panel_carousel_text(void *none){return st_text_row_2(NULL) + 5;}

int x_panel_carousel_text_daltonism(void *none){return col_right_x(NULL) + 125;}
int y_panel_carousel_text_daltonism(void *none){return st_text_row_2(NULL) + 65;}
int x_panel_carousel_daltonism(void *none){return col_right_x(NULL) + 20;}
int y_panel_carousel_daltonism(void *none){return st_text_row_2(NULL) + 40;}
int x_arrow_left_daltonism(void *none){return col_right_x(NULL);}
int x_arrow_right_daltonism(void *none){return col_right_x(NULL) + 250;}

//LANG
static UI_text *st_text_lang_choice = NULL;

// RESOLUTION
UI_anchor *st_panel_carousel_anchor_resolution = NULL;
UI_panel *st_panel_carousel_resolution = NULL;
char *st_display_resolution_choice[5];
static int int_st_display_resolution_choice = 3;
static UI_text *st_text_display_resolution = NULL;
UI_anchor *st_text_display_resolution_anchor = NULL;

//DALTONISM
UI_anchor *st_panel_carousel_anchor_daltonism = NULL;
UI_panel *st_panel_carousel_daltonism = NULL;
char *st_accessibility_daltonism_choice[4];
static int int_st_accessibility_daltonism_choice = 0;
static UI_text *st_text_accessibility_daltonism = NULL;
UI_anchor *st_text_accessibility_daltonism_anchor = NULL;
UI_anchor *st_arrow_left_accessibility_daltonism_anchor = NULL;
UI_anchor *st_arrow_right_accessibility_daltonism_anchor = NULL;

void changeGeneralHiddenState(bool hidden){
    st_text_general_title->hidden = hidden;
}

void changeDisplayHiddenState(bool hidden){
    st_text_display_title->hidden = hidden;
    st_text_resolution->hidden = hidden;
    st_text_fullscreen->hidden = hidden;
    st_button_fullscreen->hidden = hidden;
    st_text_resolution->hidden = hidden;
    st_text_display_resolution->hidden = hidden;
    st_arrow_left_carousel_resolution->hidden = hidden;
    st_arrow_right_carousel_resolution->hidden = hidden;
    st_panel_carousel_resolution->hidden = hidden;
}

void changeAudioHiddenState(bool hidden){
    st_text_audio_title->hidden = hidden;
    st_text_master->hidden = hidden;
    st_text_music->hidden = hidden;
    st_text_effects->hidden = hidden;
    st_text_dialogues->hidden = hidden;
    st_slider_master->hidden = hidden;
    st_slider_music->hidden = hidden;
    st_slider_effects->hidden = hidden;
    st_slider_dialogues->hidden = hidden;
}

void changeCommandsHiddenState(bool hidden){
    st_text_commands_title->hidden = hidden;
    st_text_command_up->hidden = hidden;
    st_text_command_down->hidden = hidden;
    st_text_command_right->hidden = hidden;
    st_text_command_left->hidden = hidden;
    st_text_command_zoom_in->hidden = hidden;
    st_text_command_zoom_out->hidden = hidden;
    st_text_command_turret_1->hidden = hidden;
    st_text_command_turret_2->hidden = hidden;
    st_text_command_turret_3->hidden = hidden;
    st_text_command_pause->hidden = hidden;
    st_text_command_sell->hidden = hidden;
    st_text_command_upgrade->hidden = hidden;

    void hiddenButtons(void *self)
    {  
        buttonBinding *b = self;
        b->button->hidden = hidden;
    }

    forEach(KB_button_bindings,hiddenButtons);
}

void changeAccessibilityHiddenState(bool hidden){
    st_text_accessibility_title->hidden = hidden;
    st_text_language->hidden = hidden;
    st_text_daltonism->hidden = hidden;
    st_text_screen_shake->hidden = hidden;
    st_text_inverse_x_axis->hidden = hidden;
    st_text_inverse_y_axis->hidden = hidden;
    st_arrow_left_carousel->hidden = hidden;
    st_arrow_right_carousel->hidden = hidden;
    st_panel_carousel_lang->hidden = hidden;
    st_arrow_left_carousel->hidden = hidden;
    st_arrow_right_carousel->hidden = hidden;
    st_text_lang_choice->hidden = hidden;
    st_panel_carousel_daltonism->hidden = hidden;
    st_arrow_left_carousel_daltonism->hidden = hidden;
    st_arrow_right_carousel_daltonism->hidden = hidden;
    st_text_accessibility_daltonism->hidden = hidden;
    st_button_accessibility_screen_shake->hidden = hidden;
    st_button_accessibility_invert_x_axis->hidden = hidden;
    st_button_accessibility_invert_y_axis->hidden = hidden;
}

void updateButtonIcon(void *data)
{
    buttonBinding *this = data;
    if(this->button->isPressed) return;
    list *keyTextureList = KB_getKCTA();
    SDL_Keycode code = *((SDL_Keycode*)getDataAtIndex(*KB_getKeys(this->action),0));
    SDL_Texture *texture_found = NULL;
    if(!this->button->textureObjectIcon && this->button->textureObjectIconCache) return;
    void searchTexture(void *asso)
    {
        keyTexture *kt = asso;
        
        if(kt->code == code)
        {
            texture_found = kt->texture;
        }
    }
    forEach(keyTextureList,searchTexture);
    if(texture_found)
    {
        if(this->button->text)
        {
            deleteInList(this->button->menu->texts,this->button->text);
            UI_FreeText(this->button->text);
            this->button->text = NULL;
        }
        if(this->button->textureObjectIcon)
        {
            this->button->textureObjectIcon->texture = texture_found;
        }
        else
        {
            UI_setButtonIconFromTex(this->button,texture_found,SDL_FLIP_NONE);
        }
    }
    else
    {
        if(this->button->textureObjectIcon)
        {
            deleteInList(this->button->menu->textureObjects,this->button->textureObjectIcon);
            UI_FreeTextureObject(this->button->textureObjectIcon);
            this->button->textureObjectIcon = NULL;
        }
        if(this->button->textureObjectIconCache)
        {
            deleteInList(this->button->menu->textureObjects,this->button->textureObjectIconCache);
            UI_FreeTextureObject(this->button->textureObjectIconCache);
            this->button->textureObjectIconCache = NULL;
        }
        if(this->button->text)
        {
            UI_text *text = this->button->text;
            char **string = text->text;
            strcpy(*string,SDL_GetKeyName(code));
        }
        else
        {
            char **string = malloc(sizeof(char*));
            *string = malloc(sizeof(char)*32);
            strcpy(*string,SDL_GetKeyName(code));
            this->button->text = UI_newText(this->button->menu,string,this->button->anchor,UI_TA_CENTER,UI_TJ_CENTER,(SDL_Color){255,255,255,255},"./assets/fonts/RulerGold.ttf",25);
        }
    }

}

void onUpdateSettings(){
    st_panel->width = THIS_GAME->winWidth - (getLT_x(NULL)*2);
    st_panel->height = THIS_GAME->winHeight - (getLT_y(NULL)*2);
    
    forEach(KB_button_bindings,updateButtonIcon);

    changeGeneralHiddenState(!st_button_general->isActive);
    changeAccessibilityHiddenState(!st_button_accessibility->isActive);
    changeAudioHiddenState(!st_button_audio->isActive);
    changeCommandsHiddenState(!st_button_commands->isActive);
    changeDisplayHiddenState(!st_button_display->isActive);
    
}

void switchBack(void)
{
    THIS_GAME->menu = backStateMenu;
    UI_refreshMenu(backStateMenu);
}

void onClear(void *self){
    switchBack();
}

void onMenuSwitch(void *self)
{
    st_button_general->isActive = (self == st_button_general);
    st_button_audio->isActive = (self == st_button_audio);
    st_button_display->isActive = (self == st_button_display);
    st_button_commands->isActive = (self == st_button_commands);
    st_button_accessibility->isActive = (self == st_button_accessibility);
}

void fullscreen_on(void *self)
{
    UI_button *button = self;
    button->isPressed = true;
    SDL_SetWindowFullscreen(THIS_GAME->window,SDL_WINDOW_FULLSCREEN);
}

void fullscreen_off(void *self)
{
    UI_button *button = self;
    button->isPressed = false;
    SDL_SetWindowFullscreen(THIS_GAME->window,0);
}

void changeLangLeft(void *self)
{
    int currentLangIndex = searchIndexInList(*THIS_GAME->languageManager->availableLangIds, THIS_GAME->languageManager->currentLangId);
    currentLangIndex++;
    if(currentLangIndex >= THIS_GAME->languageManager->availableLangIds->length)
        currentLangIndex = 0;
    char *langToLoad = getDataAtIndex(*THIS_GAME->languageManager->availableLangIds, currentLangIndex);
    THIS_GAME->languageManager->loadLang(langToLoad);
}

void changeLangRight(void *self)
{
    int currentLangIndex = searchIndexInList(*THIS_GAME->languageManager->availableLangIds, THIS_GAME->languageManager->currentLangId);
    currentLangIndex--;
    if(currentLangIndex < 0)
        currentLangIndex = THIS_GAME->languageManager->availableLangIds->length - 1;
    char *langToLoad = getDataAtIndex(*THIS_GAME->languageManager->availableLangIds, currentLangIndex);
    THIS_GAME->languageManager->loadLang(langToLoad);
}

void changeResolutionRight(void *self)
{
    UI_button *button = self;
    int_st_display_resolution_choice += 1;
    if(int_st_display_resolution_choice==4)
        int_st_display_resolution_choice = 0;
    Game *game = THIS_GAME;
    switch(int_st_display_resolution_choice)
    {
        case 0:
            SDL_SetWindowSize(game->window,1280,720);
            break;
        case 1:
            SDL_SetWindowSize(game->window,1366,768);
            break;
        case 2:
            SDL_SetWindowSize(game->window,1536,864);
            break;
        case 3:
            SDL_SetWindowSize(game->window,1920,1080);
            break;
    }
    st_text_display_resolution->text = LM_getTradById(st_display_resolution_choice[int_st_display_resolution_choice]);
}

void changeResolutionLeft(void *self)
{
    UI_button *button = self;
    int_st_display_resolution_choice -= 1;
    if(int_st_display_resolution_choice==-1)
        int_st_display_resolution_choice = 3;
    Game *game = THIS_GAME;
    switch(int_st_display_resolution_choice)
    {
        case 0:
            SDL_SetWindowSize(game->window,1280,720);
            break;
        case 1:
            SDL_SetWindowSize(game->window,1366,768);
            break;
        case 2:
            SDL_SetWindowSize(game->window,1536,864);
            break;
        case 3:
            SDL_SetWindowSize(game->window,1920,1080);
            break;
    }
    st_text_display_resolution->text = LM_getTradById(st_display_resolution_choice[int_st_display_resolution_choice]);
}

void changeDaltonismLeft(void *self)
{
    UI_button *button = self;
    int_st_accessibility_daltonism_choice -= 1;
    if(int_st_accessibility_daltonism_choice==-1)
        int_st_accessibility_daltonism_choice = 3;
    st_text_accessibility_daltonism->text = LM_getTradById(st_accessibility_daltonism_choice[int_st_accessibility_daltonism_choice]);
}

void changeDaltonismRight(void *self)
{
    UI_button *button = self;
    int_st_accessibility_daltonism_choice += 1;
    if(int_st_accessibility_daltonism_choice==4)
        int_st_accessibility_daltonism_choice = 0; 
    st_text_accessibility_daltonism->text = LM_getTradById(st_accessibility_daltonism_choice[int_st_accessibility_daltonism_choice]);
}

void screenShakeOn(void *self)
{

}

void screenShakeOff(void *self)
{
    
}

void invertXAxisOn(void *self)
{
    
}

void invertXAxisOff(void *self)
{
    
}

void invertYAxisOn(void *self)
{
    
}

void invertYAxisOff(void *self)
{
    
}



void buttonInput(SDL_Keycode code, void *self)
{
    buttonBinding *b = self;
    b->button->isPressed = false;
    if(code == -1)
    {
        b->button->textureObjectIcon = b->button->textureObjectIconCache;
        return;
    }
    UI_buttonShowIcon(b->button);
    list *l = KB_getKeys(b->action);
    GA_type *action = getDataAtIndex(*l,0);
    if(l->length)
    {
        void deleteKB(void *self)
        {
            SDL_Keycode *key = self;
            keyBinding *k =  KB_getKeyBinding(*key, b->action);
            KB_removeKeyBinding(k);
        }
        forEach(l,deleteKB);
    }
    KB_add(code,b->action);
}

void inputKeyButton(void *self)
{
    UI_button *button = self;
    if(button->text)
    {
        deleteInList(button->menu->texts,button->text);
        UI_FreeText(button->text);
        button->text = NULL;
    }
    if(button->isPressed)
    {
        KB_handleKeyCode(-1, button->menu->game);
        button->isPressed = false;
        return;
    }
    UI_buttonHideIcon(button);
    buttonBinding *found = NULL;
    void searchBinding(void *self2)
    {
        buttonBinding *b = self2;
        if(b->button == button)
        {
            found = b;
        }
    }
    forEach(KB_button_bindings,searchBinding);
    KB_getInput(buttonInput,found);
}

void UI_switchToSettings(void *GAME)
{
    THIS_GAME = GAME;
    Game *game = GAME;
    LM_getTradById = game->languageManager->getTradById;
    backStateMenu = game->menu; // previous save
    game->menu = UI_initMenu(GAME);
    game->menu->updateScript = onUpdateSettings;
    game->menu->clearScript = onClear;
    UI_anchor *A_LC = UI_newAnchor(game->menu, getLT_x, getLT_y);
    UI_anchor *anchor_back_button = UI_newAnchor(game->menu, back_button_anchor_x,back_button_anchor_y);
    st_panel = UI_newPanel(game->menu, 0, 0, A_LC, 2, UI_PT_A);

    st_panel->isActive = true;
    st_button_general = UI_newButtonPanelIcon(st_panel,(SDL_Rect){100,0,200,50}, 0,onMenuSwitch,"UI_icon_settings.png");
    st_button_general->rect.w *=0.5;
    st_button_general->isActive = true;
    st_button_display = UI_newButtonPanelIcon(st_panel,(SDL_Rect){200,0,200,50}, 0,onMenuSwitch,"UI_icon_display.png");
    st_button_display->rect.w *=0.5;
    st_button_audio = UI_newButtonPanelIcon(st_panel,(SDL_Rect){300,0,200,50}, 0,onMenuSwitch,"UI_icon_sound.png");
    st_button_audio->rect.w *=0.5;
    st_button_commands = UI_newButtonPanelIcon(st_panel,(SDL_Rect){400,0,200,50}, 0,onMenuSwitch,"UI_icon_commands.png");
    st_button_commands->rect.w *=0.5;
    st_button_accessibility = UI_newButtonPanelIcon(st_panel,(SDL_Rect){500,0,200,50}, 0,onMenuSwitch,"UI_icon_accessibility.png");
    st_button_accessibility->rect.w *=0.5;
    st_button_back = UI_newButton(game->menu, LM_getTradById("menu_settings_back"), UI_B_BACK,anchor_back_button,false,onClear,NULL,NULL,1);

    SDL_Color white = {255,255,255,255};
    UI_anchor *st_title_anchor = UI_newAnchor(game->menu, st_title_x, st_title_y);
    st_text_general_title = UI_newText(game->menu,LM_getTradById("menu_settings_general"),st_title_anchor, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 60);
    st_text_display_title = UI_newText(game->menu,LM_getTradById("menu_settings_display"),st_title_anchor, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 60);
    st_text_audio_title = UI_newText(game->menu,LM_getTradById("menu_settings_audio"),st_title_anchor, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 60);
    st_text_commands_title = UI_newText(game->menu,LM_getTradById("menu_settings_commands"),st_title_anchor, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 60);
    st_text_accessibility_title = UI_newText(game->menu,LM_getTradById("menu_settings_accessibility"),st_title_anchor, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 60);

    UI_anchor *st_title_line_anchor_col_left_row_1 = UI_newAnchor(game->menu, col_left_x, st_text_row_1);
    UI_anchor *st_title_line_anchor_col_left_row_2 = UI_newAnchor(game->menu, col_left_x, st_text_row_2);
    UI_anchor *st_title_line_anchor_col_left_row_3 = UI_newAnchor(game->menu, col_left_x, st_text_row_3);
    UI_anchor *st_title_line_anchor_col_left_row_4 = UI_newAnchor(game->menu, col_left_x, st_text_row_4);
    UI_anchor *st_title_line_anchor_col_left_row_5 = UI_newAnchor(game->menu, col_left_x, st_text_row_5);
    UI_anchor *st_title_line_anchor_col_left_row_6 = UI_newAnchor(game->menu, col_left_x, st_text_row_6);
    UI_anchor *st_title_line_anchor_col_left_row_7 = UI_newAnchor(game->menu, col_left_x, st_text_row_7);
    UI_anchor *st_title_line_anchor_col_right_row_1 = UI_newAnchor(game->menu, col_right_x, st_text_row_1);
    UI_anchor *st_title_line_anchor_col_right_row_2 = UI_newAnchor(game->menu, col_right_x, st_text_row_2);
    UI_anchor *st_title_line_anchor_col_right_row_3 = UI_newAnchor(game->menu, col_right_x, st_text_row_3);
    UI_anchor *st_title_line_anchor_col_right_row_4 = UI_newAnchor(game->menu, col_right_x, st_text_row_4);
    UI_anchor *st_title_line_anchor_col_right_row_5 = UI_newAnchor(game->menu, col_right_x, st_text_row_5);
    UI_anchor *st_title_line_anchor_col_right_row_6 = UI_newAnchor(game->menu, col_right_x, st_text_row_6);
    UI_anchor *st_title_line_anchor_col_right_row_7 = UI_newAnchor(game->menu, col_right_x, st_text_row_7);
    UI_anchor *st_title_command_anchor_col_left_row_1 = UI_newAnchor(game->menu, col_left_x_command_button, st_text_row_1_button);
    UI_anchor *st_title_command_anchor_col_left_row_2 = UI_newAnchor(game->menu, col_left_x_command_button, st_text_row_2_button);
    UI_anchor *st_title_command_anchor_col_left_row_3 = UI_newAnchor(game->menu, col_left_x_command_button, st_text_row_3_button);
    UI_anchor *st_title_command_anchor_col_left_row_4 = UI_newAnchor(game->menu, col_left_x_command_button, st_text_row_4_button);
    UI_anchor *st_title_command_anchor_col_left_row_5 = UI_newAnchor(game->menu, col_left_x_command_button, st_text_row_5_button);
    UI_anchor *st_title_command_anchor_col_left_row_6 = UI_newAnchor(game->menu, col_left_x_command_button, st_text_row_6_button);
    UI_anchor *st_title_command_anchor_col_left_row_7 = UI_newAnchor(game->menu, col_left_x_command_button, st_text_row_7_button);
    UI_anchor *st_title_command_anchor_col_right_row_1 = UI_newAnchor(game->menu, col_right_x_command_button, st_text_row_1_button);
    UI_anchor *st_title_command_anchor_col_right_row_2 = UI_newAnchor(game->menu, col_right_x_command_button, st_text_row_2_button);
    UI_anchor *st_title_command_anchor_col_right_row_3 = UI_newAnchor(game->menu, col_right_x_command_button, st_text_row_3_button);
    UI_anchor *st_title_command_anchor_col_right_row_4 = UI_newAnchor(game->menu, col_right_x_command_button, st_text_row_4_button);
    UI_anchor *st_title_command_anchor_col_right_row_5 = UI_newAnchor(game->menu, col_right_x_command_button, st_text_row_5_button);
    UI_anchor *st_title_command_anchor_col_right_row_6 = UI_newAnchor(game->menu, col_right_x_command_button, st_text_row_6_button);
    UI_anchor *st_title_command_anchor_col_right_row_7 = UI_newAnchor(game->menu, col_right_x_command_button, st_text_row_7_button);

    st_text_resolution = UI_newText(st_panel->menu,LM_getTradById("options_menu_video_resolution"),st_title_line_anchor_col_left_row_2, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_panel_carousel_anchor_resolution = UI_newAnchor(game->menu, x_panel_carousel, y_panel_carousel);
    st_panel_carousel_resolution = UI_newPanel(game->menu,210, 40, st_panel_carousel_anchor_resolution, 2, UI_PT_B);

    for(int i=0;i<4;i++)
    {
        st_display_resolution_choice[i] = malloc(sizeof(char)*255);
    }

    strcpy(st_display_resolution_choice[3],"options_menu_video_1920_1080");
    strcpy(st_display_resolution_choice[2],"options_menu_video_1536_864");
    strcpy(st_display_resolution_choice[1],"options_menu_video_1366_768");
    strcpy(st_display_resolution_choice[0],"options_menu_video_1280_720");

    st_text_display_resolution_anchor = UI_newAnchor(game->menu,x_panel_carousel_text,y_panel_carousel_text);
    switch(game->winWidth)
    {
        case 2560:
            st_text_display_resolution = UI_newText(game->menu,LM_getTradById("options_menu_video_2560_1440"),st_text_display_resolution_anchor,UI_TA_CENTER, UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf",30);
            break;
        case 1536:
            st_text_display_resolution = UI_newText(game->menu,LM_getTradById("options_menu_video_1536_864"),st_text_display_resolution_anchor,UI_TA_CENTER, UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf",30);
            break;
        case 1366:
            st_text_display_resolution = UI_newText(game->menu,LM_getTradById("options_menu_video_1366_768"),st_text_display_resolution_anchor,UI_TA_CENTER, UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf",30);
            break;
        case 1280:
            st_text_display_resolution = UI_newText(game->menu,LM_getTradById("options_menu_video_1280_720"),st_text_display_resolution_anchor,UI_TA_CENTER, UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf",30);
            break;
        default:
            st_text_display_resolution = UI_newText(game->menu,LM_getTradById("options_menu_video_1920_1080"),st_text_display_resolution_anchor,UI_TA_CENTER, UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf",30);
            break;
    }
    UI_anchor *st_arrow_left_carousel_resolution_anchor = UI_newAnchor(game->menu, x_arrow_left, st_text_row_2);
    UI_anchor *st_arrow_right_carousel_resolution_anchor = UI_newAnchor(game->menu, x_arrow_right, st_text_row_2);
    st_arrow_left_carousel_resolution = UI_newButton(st_panel->menu, NULL, UI_ARROW,st_arrow_right_carousel_resolution_anchor,false,changeResolutionRight,NULL,NULL,1);
    st_arrow_right_carousel_resolution = UI_newButton(st_panel->menu, NULL, UI_ARROW,st_arrow_left_carousel_resolution_anchor,false,changeResolutionLeft,NULL,NULL,1);
    UI_flipButton(st_arrow_left_carousel_resolution,SDL_FLIP_HORIZONTAL);

    st_text_fullscreen = UI_newText(st_panel->menu,LM_getTradById("options_menu_video_fullscreen"),st_title_line_anchor_col_left_row_3, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_button_fullscreen = UI_newButton(st_panel->menu, NULL, UI_CHECK_DEFAULT,st_title_line_anchor_col_right_row_3,true,NULL,fullscreen_on,fullscreen_off,1);
    if(SDL_GetWindowFlags(THIS_GAME->window) & SDL_WINDOW_FULLSCREEN)
        st_button_fullscreen->isPressed = true;

    st_text_master = UI_newText(st_panel->menu,LM_getTradById("options_menu_sound_master"),st_title_line_anchor_col_left_row_2, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_text_music = UI_newText(st_panel->menu,LM_getTradById("options_menu_sound_music"),st_title_line_anchor_col_left_row_3, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_text_effects = UI_newText(st_panel->menu,LM_getTradById("options_menu_sound_effects"),st_title_line_anchor_col_left_row_4, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_text_dialogues = UI_newText(st_panel->menu,LM_getTradById("options_menu_sound_dialogues"),st_title_line_anchor_col_left_row_5, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_slider_master = UI_newSlider(st_panel->menu, &master_volume, 25,4,st_title_line_anchor_col_right_row_2,true,NULL);
    st_slider_music = UI_newSlider(st_panel->menu, &music_volume, 25,4,st_title_line_anchor_col_right_row_3,true,NULL);
    st_slider_effects = UI_newSlider(st_panel->menu, &effects_volume, 25,4,st_title_line_anchor_col_right_row_4,true,NULL);
    st_slider_dialogues = UI_newSlider(st_panel->menu, &dialogues_volume, 25,4,st_title_line_anchor_col_right_row_5,true,NULL);

    st_text_language = UI_newText(st_panel->menu,LM_getTradById("options_menu_accessibility_language"),st_title_line_anchor_col_left_row_2, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    UI_anchor *st_arrow_left_anchor = UI_newAnchor(game->menu, x_arrow_left, st_text_row_2);
    UI_anchor *st_arrow_right_anchor = UI_newAnchor(game->menu, x_arrow_right, st_text_row_2);
    st_arrow_left_carousel = UI_newButton(st_panel->menu, NULL, UI_ARROW,st_arrow_right_anchor,false,changeLangLeft,NULL,NULL,1);
    st_arrow_right_carousel = UI_newButton(st_panel->menu, NULL, UI_ARROW,st_arrow_left_anchor,false,changeLangRight,NULL,NULL,1);
    UI_flipButton(st_arrow_left_carousel,SDL_FLIP_HORIZONTAL); //<- (!)
    st_panel_carousel_anchor = UI_newAnchor(game->menu, x_panel_carousel, y_panel_carousel);
    st_panel_carousel_lang = UI_newPanel(game->menu, 210, 40, st_panel_carousel_anchor, 2, UI_PT_B);

    UI_anchor *st_accessibility_lang_anchor = UI_newAnchor(game->menu,x_panel_carousel_text,y_panel_carousel_text);
    st_text_lang_choice = UI_newText(game->menu,LM_getTradById("lang_name"),st_accessibility_lang_anchor,UI_TA_CENTER, UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf",30);
    
    st_panel_carousel_anchor_daltonism = UI_newAnchor(game->menu,x_panel_carousel_daltonism,y_panel_carousel_daltonism);
    st_panel_carousel_daltonism = UI_newPanel(game->menu, 210, 40, st_panel_carousel_anchor_daltonism, 2, UI_PT_B);
    st_arrow_left_accessibility_daltonism_anchor = UI_newAnchor(game->menu,x_arrow_left,st_text_row_3);
    st_arrow_right_accessibility_daltonism_anchor = UI_newAnchor(game->menu,x_arrow_right,st_text_row_3);
    st_arrow_left_carousel_daltonism = UI_newButton(st_panel->menu, NULL, UI_ARROW,st_arrow_right_accessibility_daltonism_anchor,false,changeDaltonismLeft,NULL,NULL,1);
    st_arrow_right_carousel_daltonism = UI_newButton(st_panel->menu, NULL, UI_ARROW,st_arrow_left_accessibility_daltonism_anchor,false,changeDaltonismRight,NULL,NULL,1);
    UI_flipButton(st_arrow_left_carousel_daltonism,SDL_FLIP_HORIZONTAL);
    st_text_accessibility_daltonism_anchor = UI_newAnchor(game->menu,x_panel_carousel_text_daltonism,y_panel_carousel_text_daltonism);
    st_text_accessibility_daltonism = UI_newText(game->menu,LM_getTradById("options_menu_accessibility_daltonism_none"),st_text_accessibility_daltonism_anchor, UI_TA_CENTER, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    for(int i=0;i<4;i++)
    {
        st_accessibility_daltonism_choice[i] = malloc(sizeof(char)*255);
    }
    strcpy(st_accessibility_daltonism_choice[0],"options_menu_accessibility_daltonism_none");
    strcpy(st_accessibility_daltonism_choice[1],"options_menu_accessibility_daltonism_deuteranopia");
    strcpy(st_accessibility_daltonism_choice[2],"options_menu_accessibility_daltonism_protanopia");
    strcpy(st_accessibility_daltonism_choice[3],"options_menu_accessibility_daltonism_tritanopia");
    st_text_daltonism = UI_newText(st_panel->menu,LM_getTradById("options_menu_accessibility_daltonism"),st_title_line_anchor_col_left_row_3, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    
    st_text_screen_shake = UI_newText(st_panel->menu,LM_getTradById("options_menu_accessibility_screen_shake"),st_title_line_anchor_col_left_row_4, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_button_accessibility_screen_shake = UI_newButton(st_panel->menu, NULL, UI_CHECK_DEFAULT,st_title_line_anchor_col_right_row_4,true,NULL,screenShakeOn,screenShakeOff,1);

    st_text_inverse_x_axis = UI_newText(st_panel->menu,LM_getTradById("options_menu_accessibility_inverse_x"),st_title_line_anchor_col_left_row_5, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_button_accessibility_invert_x_axis = UI_newButton(st_panel->menu, NULL, UI_CHECK_DEFAULT,st_title_line_anchor_col_right_row_5,true,NULL,invertXAxisOn,invertXAxisOff,1);;

    st_text_inverse_y_axis = UI_newText(st_panel->menu,LM_getTradById("options_menu_accessibility_inverse_y"),st_title_line_anchor_col_left_row_6, UI_TA_LEFT, UI_TJ_CENTER,white, "./assets/fonts/RulerGold.ttf", 30);
    st_button_accessibility_invert_y_axis = UI_newButton(st_panel->menu, NULL, UI_CHECK_DEFAULT,st_title_line_anchor_col_right_row_6,true,NULL,invertYAxisOn,invertYAxisOff,1);;
    
    st_text_command_up = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_up"),st_title_line_anchor_col_left_row_2,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_down = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_down"),st_title_line_anchor_col_left_row_3,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_right = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_right"),st_title_line_anchor_col_right_row_2,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_left = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_left"),st_title_line_anchor_col_right_row_3,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_zoom_in = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_zoom_in"),st_title_line_anchor_col_left_row_4,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_zoom_out = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_zoom_out"),st_title_line_anchor_col_right_row_4,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_turret_1 = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_turret_1"),st_title_line_anchor_col_left_row_5,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_turret_2 = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_turret_2"),st_title_line_anchor_col_left_row_6,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_turret_3 = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_turret_3"),st_title_line_anchor_col_left_row_7,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_pause = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_pause"),st_title_line_anchor_col_right_row_5,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_sell = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_sell"),st_title_line_anchor_col_right_row_6,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    st_text_command_upgrade = UI_newText(st_panel->menu,LM_getTradById("options_menu_keybinds_upgrade"),st_title_line_anchor_col_right_row_7,UI_TA_LEFT,UI_TJ_CENTER,white,"./assets/fonts/RulerGold.ttf", 30);
    
    UI_button *button_right_arrow = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_right_row_2,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_left_arrow = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_right_row_3,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_zoom_out = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_right_row_4,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_zoom_in = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_left_row_4,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_pause = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_right_row_5,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_sell = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_right_row_6,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_upgrade = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_right_row_7,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_turret1 = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_left_row_5,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_turret2 = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_left_row_6,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_turret3 = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_left_row_7,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_up_arrow = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_left_row_2,true,inputKeyButton,NULL,NULL,1.5,NULL);
    UI_button *button_down_arrow = UI_newButtonIcon(st_panel->menu,UI_B_DEFAULT,st_title_command_anchor_col_left_row_3,true,inputKeyButton,NULL,NULL,1.5,NULL);

    buttonBinding *temp;
    KB_button_bindings = newList(COMPARE_PTR);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_up_arrow,GA_UP};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_down_arrow,GA_DOWN};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_left_arrow,GA_LEFT};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_right_arrow,GA_RIGHT};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_zoom_in,GA_ZOOMIN};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_zoom_out,GA_ZOOMOUT};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_turret1,GA_TURRET1};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_turret2,GA_TURRET2};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_turret3,GA_TURRET3};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_pause,GA_PAUSE};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_upgrade,GA_UPGRADE};
    appendInList(KB_button_bindings,temp);
    temp = malloc(sizeof(buttonBinding));
    *temp = (buttonBinding){button_sell,GA_SELL};
    appendInList(KB_button_bindings,temp);

    onUpdateSettings();    
}
