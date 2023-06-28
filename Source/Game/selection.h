/**
 * @file selection.h
 * @brief Manage all the projectiles of the game
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once
#include "game.h"
#include "../Turrets/turrets.h"

/**
 * @brief Enumeration that holds the selection types
 */
typedef enum SelectionType {SELECT_NONE, SELECT_MAP, SELECT_TURRET, SELECT_GAMEOBJECT, SELECT_ITEM} SelectionType;

/**
 * @brief Union that holds the data of the selected object
 */
typedef union SelectedObject {
    int mapId;
    GameObject* gameObject;
    turretSelection *turretSelection;
} SelectedObject;

/**
 * @brief Structure that holds the current selection
 */
typedef struct Selection{
    SelectionType type;
    SelectedObject selected;
} Selection;