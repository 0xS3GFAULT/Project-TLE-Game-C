#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "./Game/game.h"
#include "./List/SmartList.h"
#include "./Turrets/turrets.h"
#include "./Robots/robots.h"

#define MAIN_C

static int *speed = NULL;
Game *game = NULL;

int main(int argc, const char * argv[]) {
    int FPS = 60;
    game = initGame("The Last Engineer", 1536,864, false);
    speed = &game->speedMultiplicator;
    int frameDelta;
    Uint32 frameStart;
    int frameTime;
    
    while(game->isRunning){

        frameStart = SDL_GetTicks();
        game->handleEvents();
        for(int i=0; i<*speed; i++){
            game->update();
        }
        game->render();

        frameTime = SDL_GetTicks() - frameStart;
        frameDelta = 1000 / (FPS * (*speed));
        if(frameDelta > frameTime) SDL_Delay(frameDelta - frameTime);
    }
    game->clean();
    return 0;
}
