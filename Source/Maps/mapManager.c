#include "./mapManager.h"
#include "../Game/game.h"
#include "../Game/textureManager.h"
#include "../Game/rendererAddons.h"
#include "../List/SmartList.h"
#include "../Game/camera.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static mapManager *MAP_MANAGER = NULL;
static float *loadingProgress = NULL;
typedef enum{MAP_NAME, MAP_DESC, MAP_NONE} mapConfigFileParam;

static mapConfigFileParam getMapConfigFileParamFromString(char *fileParamString){
    if(!strcmp("NAME", fileParamString)) return MAP_NAME;
    if(!strcmp("DESC", fileParamString)) return MAP_DESC;
    return MAP_NONE;
}

typedef struct{
    bool isAltNext;
    map_node *parent;
    int x;
    int y;
} nodeChildLinkToSolve;

void updatePreviews(){
    // list *textures = newList(compareNames);
    // DIR* repertory = opendir("./assets/maps");
    // struct dirent* readfile = NULL;
    // if(!repertory){
    //     printf("\033[1;31m MapManager_ERROR Previews : Can't open maps directory\n");
    //     perror("ERROR:");
    //     printf("\033[0m");
    //     exit(3);
    // }
    // while ((readfile = readdir(repertory)) != NULL)
    // {
    //     if(strcmp(readfile->d_name, ".") && strcmp(readfile->d_name, "..")){
    //         printf("Loading Texture - '%s'...\n", readfile->d_name);
            
    //         texture *loaded_texture = malloc(sizeof(texture));
    //         loaded_texture->name = malloc(sizeof(char) * strlen(readfile->d_name) + 1);
    //         strcpy(loaded_texture->name, readfile->d_name);

    //         char texturePath[255];
    //         sprintf(texturePath, "./assets/tex/%s", loaded_texture->name);
    //         loaded_texture->texture = IMG_LoadTexture(renderer, texturePath);
    //         if(!loaded_texture->texture) printf("\033[1;31m%s\n\033[0m", SDL_GetError());
    //         appendInList(textures, loaded_texture);
    //     }
    // }
    // closedir(repertory);
    // return textures;
}


static map_startData *generateStartData(map_node *start, map *map){
    map_startData *startData = malloc(sizeof(map_startData));
    map_cell *cell = getCellFromDataGrid(map->dataGrid, start->x, start->y);
    int x = start->x;
    int y = start->y;
    while(cell->type == MCT_PATH || cell->type == MCT_PATH_NS){
        if(start->x <= 0) cell = getCellFromDataGrid(map->dataGrid, x,--y);
        else cell = getCellFromDataGrid(map->dataGrid, --x, y);
    }
    startData->x1 = x;
    startData->y1 = y;
    x = start->x;
    y = start->y;
    cell = getCellFromDataGrid(map->dataGrid, start->x, start->y);
    while(cell->type == MCT_PATH || cell->type == MCT_PATH_NS){
        if(start->x <= 0) cell = getCellFromDataGrid(map->dataGrid, x, ++y);
        else cell = getCellFromDataGrid(map->dataGrid, ++x, y);
    }
    startData->x2 = x;
    startData->y2 = y;
    // printf("s(%d, %d) 1(%d, %d) 2(%d,%d) \n", start->x, start->y, startData->x1, startData->y1, startData->x2, startData->y2);
    return startData;
}

static void loadMapMetadataFromFiles(map *map){
    char path[50];
    sprintf(path, "./assets/maps/%s.map", map->id);
    FILE *map_file = fopen(path,"r");
    if(!map_file) printf("\033[1;31mFailed to open %s\033[0m\n", path);
    char line_cache[255];
    char stat_name[255];
    char stat_value[255];
    while(fgets(line_cache,255,map_file) != NULL){
        sscanf(line_cache,"%[^ ] = %[^\n]", stat_name, stat_value);
        switch(getMapConfigFileParamFromString(stat_name)){
            case MAP_NAME:
                map->name = malloc(sizeof(char)*(strlen(stat_value)+1));
                strcpy(map->name, stat_value);
                break;
            case MAP_DESC:
                map->description = malloc(sizeof(char)*(strlen(stat_value)+1));
                strcpy(map->description, stat_value);
                break;
            case MAP_NONE :
                break;
        }
    }
    fclose(map_file);

    if(loadingProgress) *loadingProgress = 0.3;

    sprintf(path, "./assets/maps/%s.nodes", map->id);
    FILE *nodes_file = fopen(path,"rb");
    if(!nodes_file) printf("\033[1;31mFailed to open %s\033[0m\n", path);
    map->nodes = newList(compareNodes);
    map->starts = newList(compareNodes);
    list *childLinkToSolve = newList(COMPARE_PTR);
    int sourceWidth;
    int sourceHeight;
    int readSuccess = fread(&sourceWidth, sizeof(int), 1, nodes_file);
    readSuccess += fread(&sourceHeight, sizeof(int), 1, nodes_file);
    if(!readSuccess) printf("\033[1;31mFailed to read %s\033[0m\n", path);
    float ratioX = (float)map->width/(float)sourceWidth;
    float ratioY = (float)map->height/(float)sourceHeight;
    //saving binary format (0:)[x](1:)[y](2:)[type](3:)[nextX](4:)[nextY](5:)[altNextX](6:)[altNextY]
    int readValue[7];
    if(loadingProgress) *loadingProgress = 0.4;
    while(fread(readValue, sizeof(int), 7, nodes_file) == 7){
        map_node *currentNode = NULL;
        nodeChildLinkToSolve *currentNext = NULL;
        currentNode = malloc(sizeof(map_node));
        appendInList(map->nodes, currentNode);
        currentNode->x = readValue[0] * ratioX;
        currentNode->y = readValue[1] * ratioY;
        switch (readValue[2]) {
            case MNT_START: 
                appendInList(map->starts, currentNode);
                currentNode->startData = generateStartData(currentNode, map);
                break;
            case MNT_END: 
                map->end = currentNode;
            default:
                currentNode->startData = NULL;
                break;
        }

        if(readValue[3] != -1) {
            currentNext = malloc(sizeof(nodeChildLinkToSolve));
            currentNext->isAltNext = false;
            currentNext->parent = currentNode;
            currentNext->x = readValue[3] * ratioX;
            currentNext->y = readValue[4] * ratioY;
            appendInList(childLinkToSolve, currentNext);
        } else currentNode->next = NULL;

        if(readValue[5] != -1) {
            currentNext = malloc(sizeof(nodeChildLinkToSolve));
            currentNext->isAltNext = true;
            currentNext->parent = currentNode;
            currentNext->x = readValue[5] * ratioX;
            currentNext->y = readValue[6] * ratioY;
            appendInList(childLinkToSolve, currentNext);
        } else currentNode->nextAlt = NULL;
    }
    fclose(nodes_file);
    if(loadingProgress) *loadingProgress = 0.5;

    //Resolving child links
    float progressDelta = 0.4/childLinkToSolve->length;
    for(int index = 0; index < childLinkToSolve->length; index++){
        nodeChildLinkToSolve *nclts = getDataAtIndex(*childLinkToSolve, index);
        map_node *parent = nclts->parent;
        map_node *child = getNodeAt(*map->nodes, nclts->x, nclts->y);
        if(nclts->isAltNext) parent->nextAlt = child;
        else parent->next = child;
        free(nclts);
        if(loadingProgress) *loadingProgress += progressDelta;
    }
    freeList(childLinkToSolve);
}


void loadMapFromFiles(const char *id, const int width, const int height, float *progress){
    loadingProgress = progress;
    if(MAP_MANAGER->currentMap) MAP_MANAGER->unloadMap();
    Game *game = MAP_MANAGER->parent;
    map *loadedMap = malloc(sizeof(map));
    loadedMap->id = malloc(sizeof(char) * (strlen(id)+1));
    strcpy(loadedMap->id, id);
    char dataMapFileName[255];
    sprintf(dataMapFileName, "map_%s_datagrid.png", loadedMap->id);
    if(loadingProgress) *loadingProgress = 0.1;
    loadedMap->dataGrid = getDataGridFromDataMap(dataMapFileName, width, height);
    if(loadingProgress) *loadingProgress = 0.2;
    loadedMap->width = width;
    loadedMap->height = height;
    loadedMap->texName = malloc(sizeof(char) * (strlen(id)+10));
    sprintf(loadedMap->texName, "map_%s.png", loadedMap->id);
    loadedMap->texture = game->textureManager->getTexture(loadedMap->texName);
    if(loadingProgress) *loadingProgress = 0.25;
    loadMapMetadataFromFiles(loadedMap);
    MAP_MANAGER->currentMap = loadedMap;
    if(loadingProgress) *loadingProgress = 1.0;
    loadingProgress = NULL;
}

void unloadCurrentMap(){
    if(!MAP_MANAGER->currentMap) return;
    free(MAP_MANAGER->currentMap->id);
    free(MAP_MANAGER->currentMap->name);
    free(MAP_MANAGER->currentMap->description);
    free(MAP_MANAGER->currentMap->texName);
    free(MAP_MANAGER->currentMap->end);
    freeDatagrid(&MAP_MANAGER->currentMap->dataGrid);
    free(MAP_MANAGER->currentMap);
    MAP_MANAGER->currentMap = NULL;
}

void renderMap(){
    map *Map = MAP_MANAGER->currentMap;
    Game *game = MAP_MANAGER->parent;
    SDL_Rect rect={0,0,Map->width,Map->height};
    cameraRender(Map->texture, rect);
    if(game->key_debug == DEBUG_PATH){
        char debugTex[255];
        sprintf(debugTex, "map_%s_datagrid.png", Map->id);
        SDL_Texture *tex = game->textureManager->getTexture(debugTex);
        SDL_SetTextureAlphaMod(tex, 200);
        cameraRender(tex, rect);
        SDL_SetTextureAlphaMod(tex, 255);
        SDL_SetRenderDrawColor(game->renderer, 0,0,255,255);
        // for(int x = 0; x<Map->dataGrid.w; x++) for(int y = 0; y<Map->dataGrid.h; y++)
        //     if(Map->dataGrid.grid[y][x].type == MCT_PATH_NS) SDL_RenderDrawPoint(game->renderer, x, y);
        for(int startIndex = 0; startIndex < Map->starts->length; startIndex++)
            for(int seed = 0; seed < 8; seed++){
                map_node *node = getDataAtIndex(*Map->starts, startIndex);
                if(seed == 0 || seed == 4) SDL_SetRenderDrawColor(game->renderer, 134,250,25,100);
                if(seed == 1 || seed == 5) SDL_SetRenderDrawColor(game->renderer, 77,198,250,100);
                if(seed == 2 || seed == 6) SDL_SetRenderDrawColor(game->renderer, 250,214,12,100);
                if(seed == 3 || seed == 7) SDL_SetRenderDrawColor(game->renderer, 227,37,250,100);
                int pathSeed = seed;
                while(node->next){
                    DrawCircle(game->renderer,node->x - game->cameraX,node->y - game->cameraY,5);
                    DrawCircle(game->renderer,node->x - game->cameraX,node->y - game->cameraY,4);
                    DrawCircle(game->renderer,node->x - game->cameraX,node->y - game->cameraY,3);
                    DrawCircle(game->renderer,node->x - game->cameraX,node->y - game->cameraY,2);
                    DrawCircle(game->renderer,node->x - game->cameraX,node->y - game->cameraY,1);
                    DrawLine(game->renderer, node->x - game->cameraX, node->y - game->cameraY, node->next->x - game->cameraX, node->next->y - game->cameraY);
                    if(node->nextAlt){
                        DrawLine(game->renderer, node->x - game->cameraX, node->y - game->cameraY, node->nextAlt->x - game->cameraX, node->nextAlt->y - game->cameraY);
                        if(!(pathSeed%2)) node = node->next;
                        else node = node->nextAlt;
                        pathSeed/=2;
                    } else node = node->next;
                }
            }
    }
}

mapManager *initMapManager(void *game){
    mapManager *manager = malloc(sizeof(mapManager));
    manager->currentMap = NULL;
    manager->loadMap = loadMapFromFiles;
    manager->unloadMap = unloadCurrentMap;
    manager->render = renderMap;
    manager->previews = newList(COMPARE_PTR);
    manager->updatePreviews = updatePreviews;
    manager->parent = game;
    MAP_MANAGER = manager;
    return manager;
}