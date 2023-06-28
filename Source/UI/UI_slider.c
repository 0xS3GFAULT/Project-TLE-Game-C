#include "UI_slider.h"
#include "../Game/game.h"



UI_slider *UI_newSlider(UI_menu *menu, float *value, int width, float sizeCoef, UI_anchor *anchorCL, bool updateOnRelease, void (*onUpdate)(void *entry))
{
    UI_slider *newSlider = malloc(sizeof(UI_slider));
    Game *game = menu->game;
    newSlider->value = value;
    newSlider->width = width;
    newSlider->unitWidth = 3;
    newSlider->unitHeight = 8;
    newSlider->sizeCoef = sizeCoef;
    newSlider->anchorCL = anchorCL;
    newSlider->updateOnRelease = updateOnRelease;
    newSlider->onUpdate = onUpdate;
    newSlider->hidden = false;
    newSlider->isPressed = false;
    newSlider->textureGrabberEnabled = game->textureManager->getTexture("UI_range_grabber_enabled.png");
    newSlider->textureGrabberIdle = game->textureManager->getTexture("UI_range_grabber_idle.png");
    newSlider->textureGrabber = newSlider->textureGrabberIdle;
    newSlider->textureEmpty = game->textureManager->getTexture("UI_range_empty.png");
    newSlider->textureFull = game->textureManager->getTexture("UI_range_full.png");
    newSlider->menu = menu;
    appendInList(menu->sliders, newSlider);
    return newSlider;
}

void UI_updateSlider(void *self){
    UI_slider *slider = self;
    Game *game = slider->menu->game;
    int getX = slider->anchorCL->getX(game);
    float totalWidth = (slider->width+1)*slider->unitWidth*slider->sizeCoef;
    if(slider->isPressed == true)
    { 
        slider->textureGrabber = slider->textureGrabberEnabled;
        if(game->mouseX <= getX) *slider->value=0.0;
        else if(game->mouseX >= totalWidth+getX) *slider->value=1.0;
        else *slider->value = (game->mouseX-getX) / totalWidth;
    } else
        slider->textureGrabber = slider->textureGrabberIdle;
    if(!slider->hidden)
        if((game->mouseX >= slider->anchorCL->getX(game) && game->mouseX <= slider->anchorCL->getX(game)+totalWidth) && (game->mouseY>=slider->anchorCL->getY(game)-(5*slider->sizeCoef) && game->mouseY<=slider->anchorCL->getY(game)+(5*slider->sizeCoef)))
            game->currentCursor = game->cursorHand;
}

void UI_renderSlider(void *self){
    UI_slider *this = self;
    Game *game = this->menu->game;
    if(!this->hidden){
        int x = this->anchorCL->getX(game);
        int y = this->anchorCL->getY(game) - ((float) 1/2)*this->unitHeight*this->sizeCoef;
        float totalWidth = (this->width+1)*this->unitWidth*this->sizeCoef;
        SDL_Rect emptyRect = {x, y, totalWidth, this->unitHeight*this->sizeCoef};
        SDL_Rect fullRect = {x + this->sizeCoef/2, y, totalWidth * *this->value, this->unitHeight*this->sizeCoef};
        SDL_RenderCopy(game->renderer, this->textureEmpty, NULL, &emptyRect);
        SDL_RenderCopy(game->renderer, this->textureFull, NULL, &fullRect);

        int texWidth, texHeight;
        SDL_QueryTexture(this->textureGrabber, NULL, NULL, &texWidth, &texHeight);
        SDL_Rect button = {x - ((texWidth*this->sizeCoef)/4) + (totalWidth * *this->value), y, texWidth*this->sizeCoef/2, texHeight*this->sizeCoef/2};
        SDL_RenderCopy(game->renderer, this->textureGrabber, NULL, &button);
    }
}

void UI_SliderHandleMouseEvent(UI_slider *slider, bool isPressed){
    Game *game = slider->menu->game;
    if(isPressed && !slider->hidden){
        float totalWidth = (slider->width+1)*slider->unitWidth*slider->sizeCoef;
        if((game->mouseX >= slider->anchorCL->getX(game) && game->mouseX <= slider->anchorCL->getX(game)+totalWidth) && (game->mouseY>=slider->anchorCL->getY(game)-(5*slider->sizeCoef) && game->mouseY<=slider->anchorCL->getY(game)+(5*slider->sizeCoef)))
            slider->isPressed = true;
    }
    else slider->isPressed = false;
}

void UI_FreeSlider(void *self){
    UI_slider *slider = self;
    free(slider);
}