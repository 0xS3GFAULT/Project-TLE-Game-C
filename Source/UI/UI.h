#pragma once
#include "UI_struct.h"
#include <stdbool.h>

UI_anchor *UI_newAnchor(UI_menu *parent, int (*getx)(void *entry), int (*gety)(void *entry));
void UI_FreeAnchor(void *self);

UI_textureObject *UI_newAnimatedTextureObject(UI_menu *parent, SDL_Rect rect, UI_anchor *anchor, char *textureName, int nOfFrames);
UI_textureObject *UI_newStaticTextureObject(UI_menu *parent, SDL_Rect rect, UI_anchor *anchor, char *textureName);
UI_textureObject *UI_newStaticTextureObjectStatic(UI_menu *parent, SDL_Rect rect, UI_anchor *anchor, SDL_Texture *texture);
void UI_UpdateTextureObject(void *self);
void UI_RenderTextureObject(void *self);
void UI_FreeTextureObject(void *self);


UI_text *UI_newText(UI_menu *parent, char **text, UI_anchor *anchor, UI_textAlign align, UI_textJustify justify, SDL_Color color, char *fontName, int size_font);
void UI_UpdateText(void *self);
void UI_RenderText(void *self);
void UI_FreeText(void *self);

UI_actionArea *UI_newActionArea(UI_menu *parent, SDL_Rect rect, UI_anchor *anchor, void (*onClick)(void *entry));
void UI_UpdateActionArea(void *self);
void UI_actionAreaHandleMouseEvent(UI_actionArea *area, bool isDown);
void UI_FreeActionArea(void *self);