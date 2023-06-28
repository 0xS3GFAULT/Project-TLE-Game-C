#include "UI_manager.h"
#include "UI_progressBar.h"
#include "../Game/game.h"
#include <stdbool.h>

static UI_menu *MENU = NULL;

void UI_MenuUpdate(){
    if(MENU->updateScript)
    {
        MENU->updateScript();
    }
    //Update Textures
    forEach(MENU->textureObjects, UI_UpdateTextureObject);
    //Update Action Area
    forEach(MENU->actionAreas, UI_UpdateActionArea);
    //Update Text
    forEach(MENU->texts, UI_UpdateText);
    //Update Buttons
    forEach(MENU->buttons, UI_updateButton);
    //Update Sliders
    forEach(MENU->sliders, UI_updateSlider);
    //Update Panels
    forEach(MENU->panels, UI_updatePanel);
    //Update Panel Buttons
    forEach(MENU->panelButtons, UI_updateButtonPanel);
}

void UI_MenuClear(){
    if(MENU->clearScript)
    {
        MENU->clearScript();
    } else {        
        //Free Anchors
        forEach(MENU->anchors, UI_FreeAnchor);
        freeList(MENU->anchors);
        //Free TextureObjects
        forEach(MENU->textureObjects, UI_FreeTextureObject);
        freeList(MENU->textureObjects);
        //Free Action Area
        forEach(MENU->actionAreas, UI_FreeActionArea);
        freeList(MENU->actionAreas);
        //Free Text
        forEach(MENU->texts, UI_FreeText);
        freeList(MENU->texts);
        //Free Buttons
        forEach(MENU->buttons, UI_FreeButton);
        freeList(MENU->buttons);
        //Free Sliders
        forEach(MENU->sliders, UI_FreeSlider);
        freeList(MENU->sliders);
        //Free ProgressBars
        forEach(MENU->progressBars, UI_FreeProgressBars);
        freeList(MENU->progressBars);
        //Free Panel Buttons
        forEach(MENU->panelButtons, UI_FreeButtonPanel);
        freeList(MENU->panelButtons);
        //Free Panels
        forEach(MENU->panels, UI_FreePanel);
        freeList(MENU->panels);
        free(MENU);
        MENU = NULL;
    }
}

void UI_MenuHandleEvent(bool isDown){
    for(int index = 0; index < MENU->actionAreas->length; index++){
        UI_actionArea *area = getDataAtIndex(*MENU->actionAreas, index);
        UI_actionAreaHandleMouseEvent(area, isDown); 
    }
    for(int index = 0; index < MENU->buttons->length; index++){
        UI_button *button = getDataAtIndex(*MENU->buttons, index);
        UI_ButtonHandleMouseEvent(button, isDown); 
    }
    for(int index = 0; index < MENU->sliders->length; index++){
        UI_slider *slider = getDataAtIndex(*MENU->sliders, index);
        UI_SliderHandleMouseEvent(slider, isDown); 
    }
    for(int index = 0; index < MENU->panelButtons->length; index++){
        UI_panelButton *button = getDataAtIndex(*MENU->panelButtons, index);
        UI_panelButtonHandleMouseEvent(button, isDown); 
    }
}

void UI_MenuRender(){
    forEach(MENU->panels, UI_renderPanel);
    forEach(MENU->panelButtons, UI_renderButtonPanel);
    forEach(MENU->progressBars, UI_renderProgressBars);
    forEach(MENU->textureObjects, UI_RenderTextureObject);
    forEach(MENU->texts, UI_RenderText);
    forEach(MENU->sliders, UI_renderSlider);
}

UI_menu *UI_initMenu(void *game){
    UI_menu *newMenu = malloc(sizeof(UI_menu));
    newMenu->game = game;
    newMenu->update = UI_MenuUpdate;
    newMenu->updateScript = NULL;
    newMenu->clear = UI_MenuClear;
    newMenu->clearScript = NULL;
    newMenu->handleEvent = UI_MenuHandleEvent;
    newMenu->render = UI_MenuRender;
    newMenu->buttons = newList(COMPARE_PTR);
    newMenu->sliders = newList(COMPARE_PTR);
    newMenu->texts = newList(COMPARE_PTR);
    newMenu->actionAreas = newList(COMPARE_PTR);
    newMenu->textureObjects = newList(COMPARE_PTR);
    newMenu->anchors = newList(COMPARE_PTR);
    newMenu->panels = newList(COMPARE_PTR);
    newMenu->panelButtons = newList(COMPARE_PTR);
    newMenu->progressBars = newList(COMPARE_PTR);
    MENU = newMenu;
    return newMenu;
}

void UI_refreshMenu(UI_menu *menu){
    MENU = menu;
}