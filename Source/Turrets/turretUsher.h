/**
 * @file turretUsher.h
 * @brief Manages the turret selection
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include "turrets.h"
#include "../Game/game.h"
#include "../Game/camera.h"

/** @brief renders the turret selection depending on if it can be placed or not regarding its position and cost
    @param game the current game
*/
void renderTurretSelection(Game *game);
/** @brief reacts to the users input by blocking the placement of a turret if the conditions are not met
    @param game the current game
    @param event the user's input event
*/
void handleInputTurretSelection(Game *game, SDL_Event *event);