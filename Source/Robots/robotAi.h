#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Game/game.h"

/** @brief updates the path for a given robot
    @param robot the robot concerned*/
void updateRobotPathAi(GameObject *robot);

/** @brief updates the shot behaviour for a given robot
    @param robot the robot concerned*/
void updateShotBehavior(GameObject *robotObj);