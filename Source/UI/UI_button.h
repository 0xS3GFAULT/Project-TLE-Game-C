/**
 * @file UI_button.h
 * @brief Manage all the buttons of the UI
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "UI.h"
#include <stdbool.h>


/** @brief creates and inits a button
    @param menu the current menu
    @param text the pointer to a text
    @param type the button type
    @param anchor the anchor of the button
    @param isSticky boolean that checks if the button is sticky or not (1 = sticky) (0 = not sticky)
    @param onClick function called when click
    @param onSetOn function called when click and when sticky
    @param onSetOff function called when click the second time and when sticky
    @param sizeFactor sizeFactor that multiplies the dimensions of the button 
    @return the button of the UI 
*/
UI_button *UI_newButton(UI_menu *menu, char **text, UI_buttonType type, UI_anchor *anchor, bool isSticky, void (*onClick)(void *button), void (*onSetOn)(void *button), void (*onSetOff)(void *button), float sizeFactor);

/** @brief flips the texture of a button
    @param button the button to flip
    @param flip the SDL_FLIP
*/
void UI_flipButton(UI_button *button, SDL_RendererFlip flip);

/** @brief updates a button
    @param self the button to update
*/
void UI_updateButton(void *self);

/** @brief handle mouse events on buttons
    @param button the button to handle
    @param isDown boolean that checks if the button is clicked (1 = clicked) (0 = not clicked)
*/
void UI_ButtonHandleMouseEvent(UI_button *button, bool isDown);
void UI_FreeButton(void *self);
UI_button *UI_newButtonIcon(UI_menu *menu, UI_buttonType type, UI_anchor *anchor, bool isSticky, void (*onClick)(void *button), void (*onSetOn)(void *button), void (*onSetOff)(void *button), float sizeFactor,char *icon);
void UI_setButtonIcon(UI_button *button, char *icon, SDL_RendererFlip flip);
void UI_buttonHideIcon(UI_button *button);
void UI_buttonShowIcon(UI_button *button);
void UI_setButtonIconFromTex(UI_button *button, SDL_Texture *texture, SDL_RendererFlip flip);