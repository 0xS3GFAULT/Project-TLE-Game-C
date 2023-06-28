#pragma once
#include "UI.h"
#include "../Game/game.h"
#include "../Core/core.h"

UI_progressBar *UI_newProgressBar(Game *game,int width,int height,char *behindTexture, char *frontTexture,UI_anchor *anchor,int sizeFactor,bool vertical,float *value,UI_progresBarType type);
void UI_renderProgressBars(void *self);
void UI_FreeProgressBars(void *self);