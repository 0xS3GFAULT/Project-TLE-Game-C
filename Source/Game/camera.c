#include "camera.h"

static float maxZoom = 5.0f;
static int dragX = 0;
static int dragY = 0;
static int dragX_start = 0;
static int dragY_start = 0;
static int dragThreshold = 0;   

static Game *GAME = NULL;

void initCamera(Game *game)
{
    GAME = game;
    GAME->cameraX = 0;
    GAME->cameraY = 0;
    GAME->cameraScale = 1;
    GAME->cameraDragging = false;
    cameraZoom(0.1);
}

void projectRectToCamera(SDL_Rect *dest){
    dest->x -= GAME->cameraX;
    dest->y -= GAME->cameraY;
    dest->x *= GAME->cameraScale;
    dest->y *= GAME->cameraScale;
    dest->w *= GAME->cameraScale;
    dest->h *= GAME->cameraScale;
}

void projectRectToCamera_inv(SDL_Rect *dest){
    dest->x /= GAME->cameraScale;
    dest->y /= GAME->cameraScale;
    dest->x += GAME->cameraX;
    dest->y += GAME->cameraY;
    dest->w /= GAME->cameraScale;
    dest->h /= GAME->cameraScale;
}

void cameraRender(SDL_Texture *tex, SDL_Rect dest){
    projectRectToCamera(&dest);
    SDL_RenderCopy(GAME->renderer, tex, NULL, &dest);
}

void cameraRenderFrame(SDL_Texture *tex, SDL_Rect dest, int frame){
    int frameWidth, frameHeight;
    SDL_QueryTexture(tex, NULL, NULL, &frameWidth, &frameHeight);
    projectRectToCamera(&dest);
    SDL_Rect src = {frame * frameHeight, 0, frameHeight, frameHeight};
    SDL_RenderCopy(GAME->renderer, tex, &src, &dest);
}

void cameraRenderEx(SDL_Texture *tex, SDL_Rect dest, int frame, float angle, bool hFlip, bool vFlip){
    int frameWidth, frameHeight;
    SDL_QueryTexture(tex, NULL, NULL, &frameWidth, &frameHeight);
    projectRectToCamera(&dest);
    SDL_Rect src = {frame * frameHeight, 0, frameHeight, frameHeight};
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if(hFlip) flip = SDL_FLIP_HORIZONTAL;
    if(vFlip){
        if(hFlip) flip =  SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
        else flip = SDL_FLIP_VERTICAL;
    }
    SDL_RenderCopyEx(GAME->renderer, tex, &src, &dest, angle, NULL, flip);
}

void cameraRenderExUnsquared(SDL_Texture *tex, SDL_Rect dest, int frame, int nOfFrames, float angle, bool hFlip, bool vFlip){
    int frameWidth, frameHeight;
    SDL_QueryTexture(tex, NULL, NULL, &frameWidth, &frameHeight);
    frameWidth /= nOfFrames;
    projectRectToCamera(&dest);
    SDL_Rect src = {frame * frameWidth, 0, frameWidth, frameHeight};
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    if(hFlip) flip = SDL_FLIP_HORIZONTAL;
    if(vFlip){
        if(hFlip) flip =  SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL;
        else flip = SDL_FLIP_VERTICAL;
    }
    SDL_RenderCopyEx(GAME->renderer, tex, &src, &dest, angle, NULL, flip);
    //SDL print red rect
    if(GAME->key_debug != DEBUG_NULL)
    {
        SDL_SetRenderDrawColor(GAME->renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(GAME->renderer, &dest);
    }
}

void cameraCheckSize(){
    if(GAME->cameraX < 0) GAME->cameraX = 0;
    if(GAME->cameraY < 0) GAME->cameraY = 0;
    if(GAME->mapManager->currentMap){
        int mapWidth = GAME->mapManager->currentMap->width;
        int mapHeight = GAME->mapManager->currentMap->height;
        SDL_Rect camera = {0, 0, mapWidth, mapHeight};
        projectRectToCamera(&camera);
        if(camera.w < GAME->winWidth || camera.h < GAME->winHeight)
            cameraZoom(0.1f);
        if(camera.w + camera.x < GAME->winWidth)
            cameraMove(-5, 0);
        if(camera.h + camera.y < GAME->winHeight - 200)
            cameraMove(0, -5);
    }
}

void cameraZoom(float zoom){
    GAME->cameraScale += zoom;
    if(GAME->cameraScale > maxZoom) GAME->cameraScale = maxZoom;
    cameraCheckSize();
}

void cameraMove(int x, int y){
    GAME->cameraX += x;
    GAME->cameraY += y;
    cameraCheckSize();
}

void cameraZoomAt(float zoom, int x, int y){
    SDL_Rect target = {x, y, 0, 0};
    if(y > GAME->winHeight - 200) return;
    projectRectToCamera_inv(&target);
    int absX = target.x;
    int absY = target.y;
    cameraZoom(zoom);
    GAME->cameraX = absX - (x/GAME->cameraScale);
    GAME->cameraY = absY - (y/GAME->cameraScale);
    cameraCheckSize();
}

void cameraStartDrag(){
    int x = GAME->mouseX, y = GAME->mouseY;
    if(y > GAME->winHeight - 200) return;
    dragX = x;
    dragY = y;
    dragX_start = x;
    dragY_start = y;
}

void cameraDrag(){
    int x = GAME->mouseX, y = GAME->mouseY;
    if(y > GAME->winHeight - 200) return;
    if(abs(x - dragX) + abs(y - dragY) > dragThreshold || GAME->cameraDragging){
        GAME->cameraDragging = true;
        int deltaX = (dragX - x) / GAME->cameraScale;
        int deltaY = (dragY - y) / GAME->cameraScale;
        cameraMove(deltaX, deltaY);
        dragX = x;
        dragY = y;
    }
}

void cameraEndDrag(){
    GAME->cameraDragging = false;
}