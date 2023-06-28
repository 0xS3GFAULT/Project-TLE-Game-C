#include "UI.h"
#include "UI_struct.h"
#include "../Game/game.h"

UI_anchor *UI_newAnchor(UI_menu *parent, int (*getX)(void *entry), int (*getY)(void *entry))
{
    UI_anchor *anchor=malloc(sizeof(UI_anchor));
    anchor->getX = getX;
    anchor->getY = getY;
    anchor->menu = parent;
    appendInList(parent->anchors, anchor); 
    return anchor;
}

void UI_FreeAnchor(void *self){
    UI_anchor *this = self;
    free(this);
}

UI_textureObject *UI_newStaticTextureObjectStatic(UI_menu *parent, SDL_Rect rect, UI_anchor *anchor, SDL_Texture *texture)
{
    Game *game = parent->game;
    UI_textureObject *static_texture_object = malloc(sizeof(UI_textureObject));
    static_texture_object->rect = rect;
    static_texture_object->rect.x = anchor->getX(parent->game);
    static_texture_object->rect.y = anchor->getY(parent->game);
    static_texture_object->anchor = anchor;
    static_texture_object->menu = parent;
    static_texture_object->texture = texture;
    static_texture_object->hidden = false;
    static_texture_object->flip = SDL_FLIP_NONE;
    static_texture_object->nOfFrames = 1;
    static_texture_object->currentFrame = 0;
    appendInList(parent->textureObjects, static_texture_object); 
    return static_texture_object;
}

UI_textureObject *UI_newStaticTextureObject(UI_menu *parent, SDL_Rect rect, UI_anchor *anchor, char *textureName)
{
    Game *game = parent->game;
    return UI_newStaticTextureObjectStatic(parent, rect, anchor, game->textureManager->getTexture(textureName));
}

UI_textureObject *UI_newAnimatedTextureObject(UI_menu *parent,SDL_Rect rect, UI_anchor *anchor, char *textureName, int nOfFrames)
{
    UI_textureObject *animated_texture_object = malloc(sizeof(UI_textureObject));
    Game *game = parent->game;
    animated_texture_object->rect=rect;
    animated_texture_object->rect.x = anchor->getX(game);
    animated_texture_object->rect.y = anchor->getY(game);
    animated_texture_object->anchor=anchor;
    animated_texture_object->nOfFrames=nOfFrames;
    animated_texture_object->currentFrame = 0;
    animated_texture_object->texture = game->textureManager->getTexture(textureName);
    animated_texture_object->menu = parent;
    animated_texture_object->hidden = false;
    animated_texture_object->flip = SDL_FLIP_NONE;
    appendInList(parent->textureObjects, animated_texture_object); 
    return animated_texture_object;
}

void UI_UpdateTextureObject(void *self){
    UI_textureObject *this = self;
    Game *game = this->menu->game;
    if(this->nOfFrames > 1){
        char id[255];
        sprintf(id, "TO%p", self);
        if(!game->animationManager->getAnim(id))
            game->animationManager->addAnim(id, &this->currentFrame, this->nOfFrames, 1);
    }
    this->rect.x = this->anchor->getX(game);
    this->rect.y = this->anchor->getY(game);
}

void UI_RenderTextureObject(void *self)
{
    UI_textureObject *this = self;
    Game *game = this->menu->game;
    if(!this->hidden && this->texture){
        SDL_RenderCopyEx(game->renderer, this->texture, NULL, &this->rect, 0, NULL, this->flip);
    }
}

void UI_FreeTextureObject(void *self){
    UI_textureObject *this = self;
    Game *game = this->menu->game;
    char id[255];
    sprintf(id, "TO%p", self);
    game->animationManager->killAnim(id);
    free(this);
}

UI_text *UI_newText(UI_menu *parent, char **text, UI_anchor *anchor, UI_textAlign align, UI_textJustify justify, SDL_Color color, char *fontName, int size_font)
{
    UI_text *text_object = malloc(sizeof(UI_text));
    text_object->text = text;
    text_object->textCache = malloc(sizeof(char) * (strlen("DEFAULT")+1));
    strcpy(text_object->textCache, "DEFAULT");
    text_object->anchor = anchor;
    text_object->textAlign=align;
    text_object->textJustify=justify;
    text_object->color = color;
    text_object->colorCache = color;
    text_object->hidden = false;
    text_object->menu = parent;
    text_object->texture = NULL;
    text_object->rect = (SDL_Rect) {0,0,0,0};
    text_object->font = TTF_OpenFont(fontName, size_font);
    if(!text_object->font) printf("TTF OpenFont : %s\n", TTF_GetError());
    UI_UpdateText(text_object);
    appendInList(parent->texts, text_object);
    return text_object;
}

void UI_UpdateText(void *self){
    UI_text *this = self;
    Game *game = this->menu->game;
    if(strcmp(*this->text, this->textCache)){
        free(this->textCache);
        this->textCache = malloc(sizeof(char) * (strlen(*this->text)+1));
        strcpy(this->textCache, *this->text);
        TTF_SizeText(this->font, *this->text, &this->rect.w, &this->rect.h);
        SDL_Surface *surface = TTF_RenderUTF8_Solid(this->font, *this->text, this->color);
        if(!surface) printf("TTF RenderUTF8_Solid : %s\n", TTF_GetError());
        this->texture = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);
    }
    if(this->color.r != this->colorCache.r || this->color.g != this->colorCache.g || this->color.b != this->colorCache.b || this->color.a != this->colorCache.a){
        SDL_Surface *surface = TTF_RenderUTF8_Solid(this->font, *this->text, this->color);
        if(!surface) printf("TTF RenderUTF8_Solid : %s\n", TTF_GetError());
        if(this->texture) SDL_DestroyTexture(this->texture);
        this->texture = SDL_CreateTextureFromSurface(game->renderer, surface);
        SDL_FreeSurface(surface);
        this->colorCache = this->color;
    }
    if(this->anchor){
        int xA = this->anchor->getX(game);
        int yA = this->anchor->getY(game);
        this->rect.x = xA - ((float) this->textAlign/2)*this->rect.w;
        this->rect.y = yA - ((float) this->textJustify/2)*this->rect.h - 6;
    }
}

void UI_RenderText(void *self)
{
    UI_text *this = self;
    Game *game = this->menu->game;
    if(this->texture && !this->hidden){
        SDL_RenderCopy(game->renderer,this->texture,NULL,&this->rect);
    }
}

void UI_FreeText(void *self){
    UI_text *this = self;
    free(this->textCache);
    SDL_DestroyTexture(this->texture);
    TTF_CloseFont(this->font);
    free(this);
}

UI_actionArea *UI_newActionArea(UI_menu *parent, SDL_Rect rect, UI_anchor *anchor, void (*onClick)(void *entry))
{
    UI_actionArea *action_area=malloc(sizeof(UI_actionArea));
    action_area->rect = rect;
    action_area->rect.x = anchor->getX(parent->game);
    action_area->rect.y = anchor->getY(parent->game);
    action_area->anchor = anchor;
    action_area->onClick = onClick;
    action_area->disabled = false;
    action_area->menu = parent;
    appendInList(parent->actionAreas, action_area); 
    return action_area;
}

void UI_UpdateActionArea(void *self){
    UI_actionArea *this = self;
    Game *game = this->menu->game;
    int x = game->mouseX;
    int y = game->mouseY;
    this->rect.x = this->anchor->getX(game);
    this->rect.y = this->anchor->getY(game);
    if(!this->disabled)
        if((x>=this->rect.x && x<=this->rect.x+this->rect.w) && (y>=this->rect.y && y<=this->rect.y+this->rect.h))
            game->currentCursor = game->cursorHand;
}

void UI_actionAreaHandleMouseEvent(UI_actionArea *area, bool isDown){
    Game *game = area->menu->game;
    int x = game->mouseX;
    int y = game->mouseY;
    if(!area->disabled)
        if((x>=area->rect.x && x<=area->rect.x+area->rect.w) && (y>=area->rect.y && y<=area->rect.y+area->rect.h)){
            if(isDown) area->isPressed = true;
            if(!isDown && area->isPressed && area->onClick) area->onClick(area);
        }
    if(!isDown) area->isPressed = false;
}

void UI_FreeActionArea(void *self){
    UI_actionArea *this = self;
    free(this);
}