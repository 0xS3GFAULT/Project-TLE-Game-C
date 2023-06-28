#include "UI_button.h"
#include "../Game/game.h"
#include <stdbool.h>

void getButtonStrKey(UI_buttonType type, char *str)
{
    switch (type)
    {
    case UI_ARROW:
        strcpy(str, "arrow");
        break;
    case UI_B_BIG:
        strcpy(str, "button_big");
        break;
    case UI_B_BACK:
        strcpy(str, "button_back");
        break;
    case UI_B_DIAG:
        strcpy(str, "button_diag");
        break;
    case UI_B_LONG:
        strcpy(str, "button_long");
        break;
    case UI_B_MAIN:
        strcpy(str, "button_main");
        break;
    case UI_CHECK_DEFAULT:
        strcpy(str, "check");
        break;
    case UI_CHECK_ROUND:
        strcpy(str, "check_round");
        break;
    case UI_CHECK_SQUARE:
        strcpy(str, "check_square");
        break;
    case UI_CHECK_SMALL:
        strcpy(str, "check_small");
        break;
    default:
        strcpy(str, "button");
        break;
    }
}

UI_button *UI_newButton(UI_menu *menu, char **text, UI_buttonType type, UI_anchor *anchor, bool isSticky, void (*onClick)(void *button), void (*onSetOn)(void *button), void (*onSetOff)(void *button), float sizeFactor)
{
    UI_button *newButton = malloc(sizeof(UI_button));
    SDL_Color white = {255, 255, 255, 255};
    if (text)
        newButton->text = UI_newText(menu, text, anchor, UI_TA_CENTER, UI_TJ_CENTER, white, "./assets/fonts/ImprovGOLD.ttf", 50);
    else
        newButton->text = NULL;
    Game *game = menu->game;
    newButton->hidden = false;
    newButton->menu = menu;
    newButton->textureObjectIcon = NULL;
    newButton->textureObjectIconCache = NULL;
    char strKey[100];
    getButtonStrKey(type, strKey);
    char idleStr[255], hoverStr[255], enabledStr[255], disabledStr[255];
    if (type == UI_CHECK_SMALL || type == UI_CHECK_ROUND || type == UI_CHECK_SQUARE)
    {
        sprintf(idleStr, "UI_%s_off.png", strKey);
        sprintf(enabledStr, "UI_%s_on.png", strKey);
        newButton->textureHover = NULL;
        newButton->textureDisabled = NULL;
    }
    else
    {
        sprintf(hoverStr, "UI_%s_hover.png", strKey);
        sprintf(idleStr, "UI_%s_idle.png", strKey);
        sprintf(enabledStr, "UI_%s_enabled.png", strKey);
        sprintf(disabledStr, "UI_%s_disabled.png", strKey);
        newButton->textureHover = game->textureManager->getTexture(hoverStr);
        newButton->textureDisabled = game->textureManager->getTexture(disabledStr);
    }
    newButton->textureIdle = game->textureManager->getTexture(idleStr);
    newButton->texturePress = game->textureManager->getTexture(enabledStr);
    newButton->textureObject = UI_newStaticTextureObject(menu, (SDL_Rect){0, 0, 0, 0}, anchor, idleStr);
    newButton->hoverTextureObject = newButton->textureHover ? UI_newStaticTextureObject(menu, (SDL_Rect){0, 0, 0, 0}, anchor, hoverStr) : NULL;
    newButton->actionArea = UI_newActionArea(menu, (SDL_Rect){0, 0, 0, 0}, anchor, onClick);
    newButton->actionArea->disabled = true;
    newButton->isPressed = false;
    newButton->isDisabled = false;
    newButton->isSticky = isSticky;
    newButton->anchor = anchor;
    newButton->type = type;
    newButton->onSetOn = onSetOn;
    newButton->onSetOff = onSetOff;
    newButton->sizeFactor = sizeFactor;
    UI_updateButton(newButton);
    appendInList(menu->buttons, newButton);

    return newButton;
}

UI_button *UI_newButtonIcon(UI_menu *menu, UI_buttonType type, UI_anchor *anchor, bool isSticky, void (*onClick)(void *button), void (*onSetOn)(void *button), void (*onSetOff)(void *button), float sizeFactor, char *icon)
{
    UI_button *newButton = malloc(sizeof(UI_button));
    SDL_Color white = {255, 255, 255, 255};
    newButton->text = NULL;
    Game *game = menu->game;
    newButton->hidden = false;
    newButton->menu = menu;
    char strKey[100];
    getButtonStrKey(type, strKey);
    char idleStr[255], hoverStr[255], enabledStr[255], disabledStr[255];
    if (type == UI_CHECK_SMALL || type == UI_CHECK_ROUND || type == UI_CHECK_SQUARE)
    {
        sprintf(idleStr, "UI_%s_off.png", strKey);
        sprintf(enabledStr, "UI_%s_on.png", strKey);
        newButton->textureHover = NULL;
        newButton->textureDisabled = NULL;
    }
    else
    {
        sprintf(hoverStr, "UI_%s_hover.png", strKey);
        sprintf(idleStr, "UI_%s_idle.png", strKey);
        sprintf(enabledStr, "UI_%s_enabled.png", strKey);
        sprintf(disabledStr, "UI_%s_disabled.png", strKey);
        newButton->textureHover = game->textureManager->getTexture(hoverStr);
        newButton->textureDisabled = game->textureManager->getTexture(disabledStr);
    }
    newButton->textureIdle = game->textureManager->getTexture(idleStr);
    newButton->texturePress = game->textureManager->getTexture(enabledStr);
    newButton->textureObject = UI_newStaticTextureObject(menu, (SDL_Rect){0, 0, 0, 0}, anchor, idleStr);
    newButton->hoverTextureObject = newButton->textureHover ? UI_newStaticTextureObject(menu, (SDL_Rect){0, 0, 0, 0}, anchor, hoverStr) : NULL;
    newButton->actionArea = UI_newActionArea(menu, (SDL_Rect){0, 0, 0, 0}, anchor, onClick);
    newButton->actionArea->disabled = true;
    newButton->isPressed = false;
    newButton->isDisabled = false;
    newButton->isSticky = isSticky;
    newButton->anchor = anchor;
    newButton->type = type;
    newButton->onSetOn = onSetOn;
    newButton->onSetOff = onSetOff;
    newButton->sizeFactor = sizeFactor;
    if (icon != NULL)
    {
        newButton->textureObjectIcon = UI_newStaticTextureObject(menu, (SDL_Rect){0, 0, 0, 0}, anchor, icon);
    }
    else
    {
        newButton->textureObjectIcon = NULL;
    }
    newButton->textureObjectIconCache = NULL;
    UI_updateButton(newButton);
    appendInList(menu->buttons, newButton);

    return newButton;
}

void UI_flipButton(UI_button *button,SDL_RendererFlip flip)
{
    if (button->hoverTextureObject)
        button->hoverTextureObject->flip = flip;
    if (button->textureObject)
        button->textureObject->flip = flip;
}

void UI_buttonHideIcon(UI_button *button)
{
    if(!button->textureObjectIcon) return;
    button->textureObjectIconCache = button->textureObjectIcon;
    button->textureObjectIcon = NULL;
    if(button->textureObjectIconCache)
        button->textureObjectIconCache->hidden = true;
}

void UI_buttonShowIcon(UI_button *button)
{
    if(!button->textureObjectIconCache) return;
    button->textureObjectIcon = button->textureObjectIconCache;
    button->textureObjectIconCache = NULL;
    if(button->textureObjectIcon)
        button->textureObjectIcon->hidden = false;
}

void UI_updateButton(void *self)
{
    UI_button *this = self;
    Game *game = this->menu->game;
    if(this->textureObjectIcon)
        this->textureObjectIcon->hidden = this->hidden;

    int texHeight, texWidth;

    SDL_QueryTexture(this->textureObject->texture, NULL, NULL, &texWidth, &texHeight);
    SDL_Rect rect = (SDL_Rect){this->anchor->getX(game) - ((texWidth * this->sizeFactor) / 2), this->anchor->getY(game) - ((texHeight * this->sizeFactor) / 2), texWidth * this->sizeFactor, texHeight * this->sizeFactor};
    this->textureObject->rect = rect;
    this->actionArea->rect = rect;

    if (this->hoverTextureObject)
    {
        SDL_QueryTexture(this->hoverTextureObject->texture, NULL, NULL, &texWidth, &texHeight);
        rect = (SDL_Rect){this->anchor->getX(game) - ((texWidth * this->sizeFactor) / 2), this->anchor->getY(game) - ((texHeight * this->sizeFactor) / 2), texWidth * this->sizeFactor, texHeight * this->sizeFactor};
        this->hoverTextureObject->rect = rect;
    }

    if (this->textureObjectIcon != NULL)
    {
        SDL_QueryTexture(this->textureIdle, NULL, NULL, &texWidth, &texHeight);
        int icon_width, icon_height;
        SDL_QueryTexture(this->textureObjectIcon->texture, NULL, NULL, &icon_width, &icon_height);
        int L = fmin(texHeight,texWidth);
        icon_width*=this->sizeFactor;
        icon_height*=this->sizeFactor;
        int cx,cy;
        cx = this->anchor->getX(game);
        cy = this->anchor->getY(game);
        rect = (SDL_Rect){cx-icon_width/2,cy-icon_height/2,icon_width,icon_height};
        this->textureObjectIcon->rect = rect;
    }

    if (!this->hidden)
    {
        this->textureObject->hidden = false;
        if (this->text)
            this->text->hidden = false;
        if (this->hoverTextureObject)
            this->hoverTextureObject->hidden = true;
        if (!this->isDisabled)
        {
            bool isHover = (game->mouseX >= this->actionArea->rect.x && game->mouseX <= this->actionArea->rect.x + this->actionArea->rect.w) && (game->mouseY >= this->actionArea->rect.y && game->mouseY <= this->actionArea->rect.y + this->actionArea->rect.h);
            if (isHover)
            {
                game->currentCursor = game->cursorHand;
                if (this->hoverTextureObject)
                    this->hoverTextureObject->hidden = false;
            }
        }
        if (this->isPressed)
            this->textureObject->texture = this->texturePress;
        else if (this->isDisabled && this->textureDisabled)
            this->textureObject->texture = this->textureDisabled;
        else
            this->textureObject->texture = this->textureIdle;
    }
    else
    {
        this->textureObject->hidden = true;
        if (this->text)
            this->text->hidden = true;
        if (this->hoverTextureObject)
            this->hoverTextureObject->hidden = true;
    }
}

void UI_ButtonHandleMouseEvent(UI_button *button, bool isDown)
{
    Game *game = button->menu->game;
    int x = game->mouseX;
    int y = game->mouseY;
    bool isHover = (x >= button->actionArea->rect.x && x <= button->actionArea->rect.x + button->actionArea->rect.w) && (y >= button->actionArea->rect.y && y <= button->actionArea->rect.y + button->actionArea->rect.h);

    if (!button->hidden && !button->isDisabled)
    {
        if (button->isSticky)
        {
            if (!isDown && isHover)
            {
                bool cacheIsPressed = button->isPressed;
                if (button->actionArea->onClick)
                    button->actionArea->onClick(button);
                if(cacheIsPressed == button->isPressed)
                    button->isPressed = !button->isPressed;
                if (button->onSetOn && button->isPressed)
                    button->onSetOn(button);
                if (button->onSetOff && !button->isPressed)
                    button->onSetOff(button);
            }
        }
        else
        {
            if (isHover)
            {
                if (isDown)
                    button->isPressed = true;
                if (!isDown && button->isPressed)
                    button->actionArea->onClick(button);
            }
            if (!isDown)
                button->isPressed = false;
        }
    }
}

void UI_FreeButton(void *self)
{
    UI_button *this = self;
    free(this);
}

void UI_setButtonIcon(UI_button *button, char *icon, SDL_RendererFlip flip)
{
    Game *game = button->menu->game;
    if(icon!=NULL)
    {
        button->textureObjectIcon = UI_newStaticTextureObject(game->menu, (SDL_Rect){0, 0, 0, 0}, button->anchor, icon);
        button->textureObjectIcon->flip = flip;
    } 
}

void UI_setButtonIconFromTex(UI_button *button, SDL_Texture *texture, SDL_RendererFlip flip)
{
    Game *game = button->menu->game;
    if(texture!=NULL)
    {
        button->textureObjectIcon = UI_newStaticTextureObjectStatic(game->menu, (SDL_Rect){0, 0, 0, 0}, button->anchor, texture);
        button->textureObjectIcon->flip = flip;
    } 
}