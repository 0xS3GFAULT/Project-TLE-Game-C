#include "mapDataManagement.h"
#include "../List/SmartList.h"
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

static Uint32 getpixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp) {
        case 1:
            return *p;
            break;
        case 2:
            return *(Uint16 *)p;
            break;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) return p[0] << 16 | p[1] << 8 | p[2];
            else return p[0] | p[1] << 8 | p[2] << 16;
            break;
        case 4:
            return *(Uint32 *)p;
            break;
        default:
            return 0;
    }
}

map_dataGrid getDataGridFromDataMap(char *dataMapFileName, int width, int height){
    char dataMapPath[255];
    sprintf(dataMapPath, "./assets/tex/%s", dataMapFileName);
    printf("Loading %s...\n",dataMapPath);
    SDL_Surface *dataMap = IMG_Load(dataMapPath);
    if(!dataMap) printf("\033[1;31mFailed to get DataGrid : %s\033[0m\n", IMG_GetError());

    map_dataGrid dataGrid;
    if(width == -1) dataGrid.w = dataMap->w;
    else dataGrid.w = width;
    if(height == -1) dataGrid.h = dataMap->h;
    else dataGrid.h = height;

    float ratioX = (float) dataMap->w / (float) dataGrid.w;
    float ratioY = (float) dataMap->h / (float) dataGrid.h;
    dataGrid.grid = malloc(sizeof(map_cell*) * dataGrid.h);
    for(int y = 0; y < dataGrid.h; y++){
        map_cell *line = malloc(sizeof(map_cell) * dataGrid.w);
        for(int x = 0; x < dataGrid.w; x++){
            map_cell *cell = line + x;
            cell->object = NULL;
            Uint8 r, g, b;
            Uint32 data = getpixel(dataMap, (int)(x * ratioX), (int)(y * ratioY));
            SDL_GetRGB(data, dataMap->format, &r, &g, &b);
            if(g > 250) cell->type = MCT_EMPTY;
            else if(b > 250) cell->type = MCT_PATH_NS;
            else if(r > 250) cell->type = MCT_STATICENV;
        }
        *(dataGrid.grid + y) = line;
    }
    return dataGrid;
}

void freeDatagrid(map_dataGrid *dataGrid){
    for(int x = 0; x < dataGrid->h; x++)
        free(*(dataGrid->grid + x));
    free(dataGrid->grid);
}

int compareNodes(void *data1, void *data2){
    map_node *node1 = data1;
    map_node *node2 = data2;
    return node1->x == node2->x && node1->y == node2->y ? 0 : 1;
}

map_node *getNodeAt(list nodes, int x, int y){
    map_node *searchNode = malloc(sizeof(map_node));
    searchNode->x = x;
    searchNode->y = y;
    map_node *result = searchDataInList(nodes, searchNode);
    free(searchNode);
    return result;
}

map_cell *getCellFromDataGrid(map_dataGrid dataGrid, int x, int y){
    map_cell *line = *(dataGrid.grid + y);
    map_cell *cell = line + x;
    return cell;
}