/**
 * @file turretAi.h
 * @brief Manages turret Ai
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Game/game.h"

/** @brief updates the behavior of normal functionning turrets 
    @param turret the turret updtated
*/
void updateTurretAi(GameObject *turret);
/** @brief updates the behavior of artillery turrets 
    @param turretObject the turret Object corresponding to the turret
*/
void updateArtilleryAi(GameObject *turretObject);