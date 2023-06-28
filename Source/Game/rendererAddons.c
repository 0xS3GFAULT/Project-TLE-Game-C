#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "rendererAddons.h"
#include "camera.h"

void DebugUpdate(void *self){
   GameObject *thisGameObject = self;
   debugObject *this = thisGameObject->actor;
   this->TTL--;
   if(this->TTL <= 0 || thisGameObject->game->key_debug == DEBUG_NULL)
      thisGameObject->delete(thisGameObject);
}

void DebugRender(void *self) {
   GameObject *thisGameObject = self;
   debugObject *this = thisGameObject->actor;
   SDL_Color rouge = {255,0,0,255};
   SDL_SetRenderDrawColor(thisGameObject->game->renderer,rouge.r,rouge.g,rouge.b,rouge.a);
   DrawCircle(thisGameObject->game->renderer,this->x - thisGameObject->game->cameraX,thisGameObject->game->cameraY,10);
}

void DebugDelete(void *self){
    GameObject *thisGameObject = self;
    debugObject *this = thisGameObject->actor;
    deleteInList(thisGameObject->game->gameObjects, thisGameObject);
    free(this);
    free(thisGameObject);
}

bool DebugIsAlive(void *self){
    GameObject *thisGameObject = self;
    debugObject *this = thisGameObject->actor;
    return this->TTL > 0;
}

GameObject *newGameObject_Debug(Game *GAME, int xpos, int ypos, int TTL, debugObjectType type){
   debugObject *newDebugObject = malloc(sizeof(debugObject));
   newDebugObject->TTL = TTL;
   newDebugObject->type = type;
   newDebugObject->x = xpos;
   newDebugObject->y = ypos;
   GameObject *newGameObject = malloc(sizeof(GameObject));
   newGameObject->game = GAME;
   newGameObject->isAlive = DebugIsAlive;
   newGameObject->delete = DebugDelete;
   newGameObject->render = DebugRender;
   newGameObject->update = DebugUpdate;
   newGameObject->type = GOT_DEBUG;
   newGameObject->actor = newDebugObject;
   appendInList(GAME->gameObjects, newGameObject);
   return newGameObject;
}

void DrawCircle(SDL_Renderer * renderer, int32_t centreX, int32_t centreY, int32_t radius)
{

   SDL_Rect dest = {centreX, centreY, radius*2, radius*2};
   projectRectToCamera(&dest);
   const int32_t diameter = dest.w;

   int32_t x = ((dest.w/2) - 1);
   int32_t y = 0;
   int32_t tx = 1;
   int32_t ty = 1;
   int32_t error = (tx - diameter);

   while (x >= y)
   {
      SDL_RenderDrawPoint(renderer, dest.x + x, dest.y - y);
      SDL_RenderDrawPoint(renderer, dest.x + x, dest.y + y);
      SDL_RenderDrawPoint(renderer, dest.x - x, dest.y - y);
      SDL_RenderDrawPoint(renderer, dest.x - x, dest.y + y);
      SDL_RenderDrawPoint(renderer, dest.x + y, dest.y - x);
      SDL_RenderDrawPoint(renderer, dest.x + y, dest.y + x);
      SDL_RenderDrawPoint(renderer, dest.x - y, dest.y - x);
      SDL_RenderDrawPoint(renderer, dest.x - y, dest.y + x);

      if (error <= 0)
      {
         ++y;
         error += ty;
         ty += 2;
      }

      if (error > 0)
      {
         --x;
         tx += 2;
         error += (tx - diameter);
      }
   }
}

void DrawLine(SDL_Renderer * renderer, int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
   SDL_Rect start = {x1, y1, 0, 0};
   SDL_Rect end = {x2, y2, 0, 0};
   projectRectToCamera(&start);
   projectRectToCamera(&end);
   SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}