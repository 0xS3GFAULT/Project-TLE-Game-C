/**
 * @file mapDataManagement.h
 * @brief Manages the maps
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../List/SmartList.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef MAPDATAMANAGER_H
#define MAPDATAMANAGER_H

typedef enum{MCT_PATH, MCT_PATH_NS, MCT_EMPTY, MCT_TURRET, MCT_ENV, MCT_STATICENV} map_cellType;

typedef struct
{
    map_cellType type;
    void *object;
} map_cell;

typedef struct
{
    int w;
    int h;
    map_cell **grid;
    SDL_Texture *texture;
} map_dataGrid;

typedef enum{MNT_START, MNT_PATH, MNT_END} map_nodeType;

typedef struct map_node map_node;

typedef struct
{
    char *id;
    char *name;
    char *description;
    int width;
    int height;
    map_dataGrid dataGrid;
    SDL_Texture *texture;
    char *texName;
    list *nodes;
    list *starts;
    map_node *end;
} map;


typedef struct 
{
    int x1;
    int y1;
    int x2;
    int y2;
} map_startData;

struct map_node
{
    int x;
    int y;
    map_nodeType type;
    map_startData *startData;
    map_node *next;
    map_node *nextAlt;
    map *parent;
    void *object;
    bool simplified;
};

int compareNodes(void *data1, void *data2);
void freeDatagrid(map_dataGrid *dataGrid);
map_cell *getCellFromDataGrid(map_dataGrid dataGrid, int x, int y);
map_node *getNodeAt(list nodes, int x, int y);
map_dataGrid getDataGridFromDataMap(char *dataMapFileName, int width, int height);

#endif