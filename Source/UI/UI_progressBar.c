#include "UI_progressBar.h"


void getProgressBarKey(UI_progresBarType type, char *str)
{
    switch (type)
    {
        case UI_PGB_HEALTH:
            strcpy(str, "Health");
            break;
        case UI_PGB_SHIELD:
            strcpy(str, "Shield");
            break;
    }
}

UI_progressBar *UI_newProgressBar(Game *game,int width,int height,char *behindTexture, char *frontTexture,UI_anchor *anchor,int sizeFactor,bool vertical,float *value,UI_progresBarType type)
{
    UI_progressBar *progressBar = malloc(sizeof(UI_progressBar));
    progressBar->menu = game->menu;
    progressBar->type = type;
    progressBar->hidden = false;
    if(!vertical)
    {
        progressBar->width = height;
        progressBar->height = width;
    }
    progressBar->width = width;
    progressBar->height = height;
    progressBar->value = value;
    char id[200];
    getProgressBarKey(type,id);
    char idle[255];
    char enabled[255];
    sprintf(idle,"UI_progressBar_%s.png",id);
    sprintf(enabled,"UI_progressBar_%s_active.png",id);
    progressBar->behind_texture = game->textureManager->getTexture(idle);
    progressBar->front_texture = game->textureManager->getTexture(enabled);
    progressBar->anchor = anchor;
    progressBar->vertical = vertical;
    progressBar->sizeFactor = sizeFactor;
    appendInList(game->menu->progressBars, progressBar);
    return progressBar; 
}

void UI_renderProgressBars(void *self){
    UI_progressBar *progressBar = (UI_progressBar*)self;
    Game *game = progressBar->menu->game;
    UI_progressBar *this = progressBar;
    if(!this->hidden){
        if(!this->vertical)
        {
            int temp = this->width;
            this->width = this->height;
            this->height = temp;
        }
        int x = this->anchor->getX(game);
        int y = this->anchor->getY(game);
        int newHeight = this->height*(*this->value);
        if(newHeight%2!=0) newHeight--;
        int newWidth = this->width*(*this->value);
        if(newWidth%2!=0) newWidth--;
        int newX = x+this->width-newWidth;
        int newY = y+this->height-newHeight;
        SDL_Rect src_rect = (SDL_Rect){7,7,2,2};
        SDL_Rect dst_rect = (SDL_Rect){x,y,this->width,this->height};
        SDL_RenderCopy(game->renderer, this->behind_texture, &src_rect,&dst_rect);
        
        int z=8*this->sizeFactor;

        dst_rect=(SDL_Rect){x,y,this->width,z};
        src_rect=(SDL_Rect){7,0,2,8};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);
        
        dst_rect=(SDL_Rect){x+this->width-z,y,z,this->height};
        src_rect=(SDL_Rect){8,7,8,2};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);
        
        dst_rect=(SDL_Rect){x,y+this->height-z,this->width,z};
        src_rect=(SDL_Rect){7,8,2,8};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);
        
        dst_rect=(SDL_Rect){x,y,z,this->height};
        src_rect=(SDL_Rect){0,7,8,2};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);

        dst_rect = (SDL_Rect){x,y,z,z};
        src_rect = (SDL_Rect){0,0,8,8};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);

        dst_rect = (SDL_Rect){x+this->width-z,y,z,z};
        src_rect = (SDL_Rect){8,0,8,8};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);
        
        dst_rect = (SDL_Rect){x,y+this->height-z,z,z};
        src_rect = (SDL_Rect){0,8,8,8};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);

        dst_rect = (SDL_Rect){x+this->width-z,y+this->height-z,z,z};
        src_rect = (SDL_Rect){8,8,8,8};
        SDL_RenderCopy(game->renderer, this->behind_texture,&src_rect,&dst_rect);

        //-----------------------------------------------------------------------------
        if(this->vertical)
        {
            src_rect = (SDL_Rect){7,7,2,2};
            dst_rect = (SDL_Rect){x,newY,this->width,newHeight};
            SDL_RenderCopy(game->renderer, this->front_texture, &src_rect,&dst_rect);

            dst_rect=(SDL_Rect){x+this->width-z,newY,z,newHeight};
            src_rect=(SDL_Rect){8,7,8,2};
            SDL_RenderCopy(game->renderer, this->front_texture,&src_rect,&dst_rect);
            
            dst_rect=(SDL_Rect){x,newY,z,newHeight};
            src_rect=(SDL_Rect){0,7,8,2};
            SDL_RenderCopy(game->renderer, this->front_texture,&src_rect,&dst_rect);
        }

        else
        {
            src_rect = (SDL_Rect){7,7,2,2};
            dst_rect = (SDL_Rect){x,y,newWidth,this->height};
            SDL_RenderCopy(game->renderer, this->front_texture, &src_rect,&dst_rect);

            dst_rect=(SDL_Rect){x,y,newWidth,z};
            src_rect=(SDL_Rect){7,0,2,8};
            SDL_RenderCopy(game->renderer, this->front_texture,&src_rect,&dst_rect);
            
            dst_rect=(SDL_Rect){x,y+this->height-z,newWidth,z};
            src_rect=(SDL_Rect){7,8,2,8};
            SDL_RenderCopy(game->renderer, this->front_texture,&src_rect,&dst_rect);
        }
        
        if(!this->vertical)
        {
            int temp = this->width;
            this->width = this->height;
            this->height = temp;
        }
    }
}

void UI_FreeProgressBars(void *self)
{
    UI_progressBar *progressBar = (UI_progressBar*)self;
    free(progressBar);
}