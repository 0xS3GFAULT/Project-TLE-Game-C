#include "./Game/game.h"
#include "./Game/rendererAddons.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void cpyDatagrid(map_dataGrid *dataGrid, map_dataGrid *srcDataGrid){
    dataGrid->w = srcDataGrid->w;
    dataGrid->h = srcDataGrid->h;
    for(int x = 0; x < dataGrid->h; x++){
        for(int y = 0; y < dataGrid->w; y++){
            map_cell *cell = getCellFromDataGrid(*dataGrid, y, x);
            map_cell *srcCell = getCellFromDataGrid(*srcDataGrid, y, x);
            cell->object = srcCell->object;
            cell->type = srcCell->type;
        }
    }
}

bool isPaternStable(map_dataGrid dataGrid, int x, int y){
    int matrix[3][3];
    int neyboors = -1;    
    for(int ix = -1; ix <= 1; ix++){
        for(int iy = -1; iy <= 1; iy++){
            if(x + ix < 0 || x + ix > dataGrid.w-1 || y + iy < 0 || y + iy > dataGrid.h - 1){
                matrix[iy + 1][ix + 1] = 1;
                neyboors++;
            }else{
                map_cell *cell = getCellFromDataGrid(dataGrid, x + ix, y + iy);
                matrix[iy + 1][ix + 1] = cell->type == MCT_PATH_NS ? 1 : 0;
                if(matrix[iy + 1][ix + 1]) neyboors++;
            }
        }
    }
    if(neyboors < 2) return false;
    if(neyboors > 5) return true;
    int nOfLockedNeyboors = 0;
    for(int ix = 0; ix <= 2; ix++) for(int iy = 0; iy <= 2; iy++)
        if(matrix[iy][ix] && (ix != 1 || iy != 1)){
            int n = 0;
            for(int jx = ix-1; jx <= ix+1; jx+=2) 
                if((jx != 1 || iy != 1) && jx >= 0 && jx <= 2)
                    if(matrix[iy][jx]) n++;
            for(int jy = iy-1; jy <= iy+1; jy+=2)
                if((ix != 1 || jy != 1) && jy >= 0 && jy <=2)
                    if(matrix[jy][ix]) n++;
            if(neyboors == 2 && n > 0) return false;
            if(n >= 2) nOfLockedNeyboors++;
        }
    if(neyboors == 2) return true;
    if(nOfLockedNeyboors >= neyboors - 2) return false;
    return true;
}

//return NULL if split
typedef enum{NS_NULL, NS_EVEN, NS_ODD} nodeSplitSearch;

map_node *getNextNode(list nodes, map_node *node, nodeSplitSearch splitSearch){
    map_node *nextNodeA = NULL;
    map_node *nextNodeB = NULL;
    map_node *parentNode = NULL;
    int x = node->x -1;
    int y = node->y;
    int ax = 1;
    int ay = 1;
    for(int i = 0; i < 4; i++){
        map_node *checkNode = getNodeAt(nodes, x, y);
        if(checkNode){
            if(checkNode->next != node && checkNode->nextAlt != node){
                if(nextNodeA) nextNodeB = checkNode;
                else nextNodeA = checkNode;
            }else parentNode = checkNode;
        }
        x += ax; y += ay; if(!i) ay=-1; if(i == 1) ax=-1;
    }
    if(!nextNodeA) printf("\033GENERATION CRITICAL ERROR AT %d, %d.\033[0m\n", node->x, node->y);
    if(splitSearch == NS_NULL) return nextNodeB ? NULL : nextNodeA;
    int dx = node->x - parentNode->x;
    int dy = node->y - parentNode->y;
    map_node *evenPath = NULL;
    map_node *leftPath = getNodeAt(nodes, node->x -dy, node->y +dx);
    map_node *rightPath = getNodeAt(nodes, node->x +dy, node->y -dx);
    if(leftPath) evenPath = leftPath;
    else if(rightPath == nextNodeB) evenPath = nextNodeA;
    else evenPath = nextNodeB;

    if(splitSearch == NS_EVEN) return evenPath;
    else return evenPath == nextNodeA ? nextNodeB : nextNodeA;
}

void connectNodeMap(list *nodes, list *starts, bool skipOdd){
    for(int startIndex = 0; startIndex < starts->length; startIndex++){
        map_node *node = getDataAtIndex(*starts, startIndex); 
        while (!node->next && node->type != MNT_END)
        {
            node->next = getNextNode(*nodes, node, NS_NULL);
            if(!node->next){
                if(startIndex < starts->length - 1) break;
                node->next = getNextNode(*nodes, node, NS_EVEN);
                if(!skipOdd){
                    node->nextAlt = getNextNode(*nodes, node, NS_ODD);
                    list *newStarts = newList(compareNodes);
                    appendInList(newStarts, node->next);
                    connectNodeMap(nodes, newStarts, true);
                    freeList(newStarts);
                    node = node->nextAlt;
                } else {
                    node = node->next;
                    skipOdd = false;
                }
            }
            else node = node->next;
        }
    }
}

void reducePathSegment(map_node *start, map_node *end, list *nodes, list *starts){
    map_node *cache = start->next;
    start->next = end;
    while(cache != end){
        map_node *nextCache = cache->next;
        deleteInList(nodes, cache);
        if(searchDataInList(*starts, cache))
            deleteInList(starts, cache);
        free(cache);
        cache = nextCache;
    }
    start->simplified = true;
    end->simplified = true;
}

bool isNodeEndOfSplit(map_node *node, list *nodes){
    int nOfParents = 0;
    for(int index = 0; index < nodes->length; index++){
        map_node *target = getDataAtIndex(*nodes, index);
        if(target->next == node || target->nextAlt == node) nOfParents++;
    }
    return nOfParents > 1;
}

float shortestDistance(float x1,float y1,float x2,float y2,float x3,float y3){
        float px=x2-x1;
        float py=y2-y1;
        float temp=(px*px)+(py*py);
        float u=((x3 - x1) * px + (y3 - y1) * py) / (temp);
        if(u>1) u=1;
        else if(u<0) u=0;
        float x = x1 + u * px;
        float y = y1 + u * py;
        float dx = x - x3;
        float dy = y - y3;
        double dist = sqrt(dx*dx + dy*dy);
        return dist;
}

float getDeviantPathValue(map_node *start, map_node *end){
    map_node *nodeTarget = start->next;
    float deviantMax = 0;
    while(nodeTarget != end){
        float distNormal = shortestDistance(start->x, start->y, end->x, end->y, nodeTarget->x, nodeTarget->y);
        if(distNormal > deviantMax){
            deviantMax = distNormal;
        }
        nodeTarget = nodeTarget->next;
    }
    return deviantMax;
}

void simplifyPath(list *nodes, list *pathStarts, float margin){
    map_node *start = NULL;
    map_node *end = NULL;
    list *starts = newList(compareNodes);
    for(int i = 0; i < pathStarts->length; i++) appendInList(starts, getDataAtIndex(*pathStarts, i));
    for(int i = 0; i < starts->length; i++){
        printf("\tNode Reducing - %d/%d\n", i+1, starts->length);
        start = getDataAtIndex(*starts, i);
        bool endOfStartPath = false;
        while(!endOfStartPath && !start->next->simplified){
            end = start->next;
            while(getDeviantPathValue(start, end) < margin){
                if(end->nextAlt){
                    appendInList(starts, end->nextAlt);
                    appendInList(starts, end->next);
                    endOfStartPath = true;
                    break;
                }
                if(end->type == MNT_END || !end->next){
                    endOfStartPath = true;
                    break;
                }
                if(isNodeEndOfSplit(end, nodes)) break;
                end = end->next;
            }
            reducePathSegment(start, end, nodes, starts);
            start = end;
        }
    }
    for(int i = 0; i < nodes->length; i++){
        start = getDataAtIndex(*nodes, i);
        if(start->nextAlt){
            map_node *nextCache = start->next->next;
            deleteInList(nodes, start->next);
            free(start->next);
            start->next = nextCache;
            nextCache = start->nextAlt->next;
            deleteInList(nodes, start->nextAlt);
            free(start->nextAlt);
            start->nextAlt = nextCache;
        }
    }
}

void genreatePathFromDataGrid(map_dataGrid *dataGrid, map *map, float margin){
    //reduceGraphNode
    printf("Extracting DataGrid...\n");
    map_dataGrid gridCpy = getDataGridFromDataMap(map->texName, dataGrid->w, dataGrid->h);
    map_dataGrid *gridCache = &gridCpy;
    
    printf("Reducing DataGrid...\n");
    bool isReduced = false;
    while(!isReduced){
        cpyDatagrid(gridCache, dataGrid);
        isReduced = true;
        for(int y = 0; y < gridCache->h; y++){
            for(int x = 0; x < gridCache->w; x++){
                map_cell *cell = getCellFromDataGrid(*gridCache, x, y);
                if(cell->type == MCT_PATH_NS){
                    if(!isPaternStable(*dataGrid, x, y) && !isPaternStable(*gridCache, x, y)){
                        cell->type = MCT_PATH;
                        isReduced = false;
                    }
                }
            }
        }
        cpyDatagrid(dataGrid, gridCache);
    }
    freeDatagrid(gridCache);
    
    map->nodes = newList(compareNodes);
    map->starts = newList(compareNodes);
    //grid to node

    printf("Converting DataGrid to Nodes...\n");
    for(int y = 0; y < dataGrid->h; y++){
        for(int x = 0; x < dataGrid->w; x++){
            map_cell *cell = getCellFromDataGrid(*dataGrid, x, y);
            if(cell->type == MCT_PATH_NS){
                cell->type = MCT_PATH;        
                map_node *node = malloc(sizeof(map_node));
                node->x = x;
                node->y = y;
                node->parent = map;
                node->next = NULL;
                node->nextAlt = NULL;
                node->simplified = false;
                if(x == dataGrid->w-1) node->type = MNT_END;
                else if(x == 0 || y == 0 || y == dataGrid->h-1){
                    node->type = MNT_START;
                    pushInList(map->starts, node);
                }
                else node->type = MNT_PATH;
                pushInList(map->nodes, node);
            }
        }
    }
    //connect nodes
    printf("Linking Nodes into path...\n");
    connectNodeMap(map->nodes, map->starts, false);
    //reduce
    printf("Melting Path...\n");
    simplifyPath(map->nodes, map->starts, margin);
}

void saveInFile(const char *mapId, map *Map){
    char fileName[255];
    sprintf(fileName, "./assets/maps/%s.nodes", mapId);
    FILE *file = fopen(fileName, "wb");
    if(!file) printf("\033ERROR : Unable to create nor edit %s\033[0m\n", fileName);
    else{
        fwrite(&Map->width, sizeof(int), 1, file);
        fwrite(&Map->height, sizeof(int), 1, file);
        //saving binary format : [x][y][type][nextX][nextY][altNextX][altNextY]
        int defaultCoord = -1;
        for(int nodeIndex = 0; nodeIndex < Map->nodes->length; nodeIndex++){
            map_node *node = getDataAtIndex(*Map->nodes, nodeIndex);
            fwrite(&node->x, sizeof(node->x), 1, file);
            fwrite(&node->y, sizeof(node->y), 1, file);
            fwrite(&node->type, sizeof(node->type), 1, file);
            if(node->next){
                fwrite(&node->next->x, sizeof(node->next->x), 1, file);
                fwrite(&node->next->y, sizeof(node->next->y), 1, file);
            }else{
                fwrite(&defaultCoord, sizeof(defaultCoord), 1, file);
                fwrite(&defaultCoord, sizeof(defaultCoord), 1, file);
            }
            if(node->nextAlt){
                fwrite(&node->nextAlt->x, sizeof(node->nextAlt->x), 1, file);
                fwrite(&node->nextAlt->y, sizeof(node->nextAlt->y), 1, file);
            }else{
                fwrite(&defaultCoord, sizeof(defaultCoord), 1, file);
                fwrite(&defaultCoord, sizeof(defaultCoord), 1, file);
            }
        }
    }
    fclose(file);
}

int main(int argc, const char * argv[]) {
    Game *game = initGame("TLE: NodeMap Generator", 1200, 800, false);
    game->key_debug = DEBUG_PATH;
    map *Map = malloc(sizeof(map));
    Map->texName = malloc(sizeof(char)*255);
    sprintf(Map->texName, "map_%s_datagrid.png", argv[1]);
    Map->dataGrid = getDataGridFromDataMap(Map->texName, -1, -1);
    Map->width = Map->dataGrid.w;
    Map->height = Map->dataGrid.h;
    genreatePathFromDataGrid(&Map->dataGrid, Map, atof(argv[2]));
    
    //RENDER
    printf("Saving file...\n");
    saveInFile(argv[1], Map);
    printf("File saved.\n");
    free(Map);
    game->mapManager->loadMap(argv[1], 1200, 800, NULL);
    game->mapManager->render();
    SDL_RenderPresent(game->renderer);
    SDL_Delay(10000);
    game->mapManager->unloadMap();
    game->clean();
    return 0;
}