#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "../Game/game.h"
#include "../Core/core.h"
#include "../Game/projectileManager.h"
#include "../List/SmartList.h"
#include "../Robots/robots.h"
#include "robotAi.h"

static float THRESHOLD_MIN_VELOCITY = 0.7;
typedef struct{
    float vx;
    float vy;
} vectAi;

typedef struct{
    float v;
    float teta;
} vectRot;

float distBetweenTwoPoints(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1 - x2,2) + pow(y1 - y2,2));
}

float modulo2Pi(float angle){
    while(angle < M_PI) angle += M_PI*2;
    while(angle > M_PI) angle -= M_PI*2;
    return angle;
}

vectAi vectRotToVectAi(vectRot rot){
    vectAi ai;
    ai.vx = rot.v*sin(rot.teta);
    ai.vy = rot.v*cos(rot.teta);
    return ai;
}

vectRot vectAiToVectRot(vectAi ai){
    vectRot rot;
    rot.teta = atan2f(ai.vx, ai.vy);
    rot.v = sqrt(pow(ai.vx,2) + pow(ai.vy, 2));
    return rot;
}

vectAi crunchVectAi(vectAi vect, float max){
    vectRot vectRot = vectAiToVectRot(vect);
    if(vectRot.v > max) vectRot.v = max;
    if(vectRot.v < THRESHOLD_MIN_VELOCITY) vectRot.v = 0;
    return vectRotToVectAi(vectRot);
}

void freeVectRot(void *self){vectRot *vect = self; free(vect);}
void freeVectAi(void *self){vectAi *vect = self; free(vect);}

bool isRobotInFrontOfTarget(robot target, robot toTest){
    float alpha = atan2f(target.x - toTest.x, target.y - toTest.y);
    float currentVect = atan2f(target.speedx, target.speedy);
    float delta = fabs(modulo2Pi(alpha - currentVect));
    return delta < (M_PI + 1);
}

vectAi getVectAllyProximity(GameObject *robotObj){
    list *gameObjects = robotObj->game->gameObjects;
    list *escapeVectors = newList(COMPARE_PTR);
    for(int index = 0; index < gameObjects->length; index++){
        GameObject *targetObj = getDataAtIndex(*gameObjects, index);
        if(targetObj->type == GOT_Robot){
            if(targetObj->isAlive(targetObj)){
                robot *targetRobot = targetObj->actor;
                robot *currentRobot = robotObj->actor;
                float distBetweenAlly = distBetweenTwoPoints(currentRobot->x, currentRobot->y, targetRobot->x, targetRobot->y);
                if(distBetweenAlly < (2*currentRobot->radius) + (2*targetRobot->radius) && targetRobot != currentRobot && isRobotInFrontOfTarget(*targetRobot, *currentRobot)){
                    vectRot *ev = malloc(sizeof(vectRot));
                    ev->v = ((currentRobot->radius + targetRobot->radius)/distBetweenAlly)*currentRobot->maxSpeed;
                    ev->teta = modulo2Pi(atan2f(targetRobot->x - currentRobot->x, targetRobot->y - currentRobot->y) + M_PI);
                    appendInList(escapeVectors, ev);
                }
            }
        }
    }
    vectAi result = {0.0,0.0};
    for(int index = 0; index < escapeVectors->length; index++){
        vectRot *vectRot = getDataAtIndex(*escapeVectors, index);
        vectAi vectAi = vectRotToVectAi(*vectRot);
        result.vx += vectAi.vx;
        result.vy += vectAi.vy; 
    }
    if(escapeVectors->length){
        result.vx /= escapeVectors->length;
        result.vy /= escapeVectors->length;
    }
    forEach(escapeVectors, freeVectRot);
    freeList(escapeVectors);
    return result;
}

vectAi getVectSwarmProximity(GameObject *robotObj){
    list *gameObjects = robotObj->game->gameObjects;
    list *syncVectors = newList(COMPARE_PTR);
    for(int index = 0; index < gameObjects->length; index++){
        GameObject *targetObj = getDataAtIndex(*gameObjects, index);
        if(targetObj->type == GOT_Robot){
            if(targetObj->isAlive(targetObj)){
                robot *targetRobot = targetObj->actor;
                robot *currentRobot = robotObj->actor;
                float distBetweenAlly = distBetweenTwoPoints(currentRobot->x, currentRobot->y, targetRobot->x, targetRobot->y);
                if(distBetweenAlly < 3*currentRobot->radius && isRobotInFrontOfTarget(*targetRobot, *currentRobot)){
                    vectAi *vAi = malloc(sizeof(vectAi));
                    vAi->vx = targetRobot->speedx;
                    vAi->vy = targetRobot->speedy;
                    appendInList(syncVectors, vAi);
                }
            }
        }
    }
    vectAi result = {0.0,0.0};
    for(int index = 0; index < syncVectors->length; index++){
        vectAi *vectAiPtr = getDataAtIndex(*syncVectors, index);
        vectAi vectAi = *vectAiPtr;
        result.vx += vectAi.vx;
        result.vy += vectAi.vy; 
    }
    if(syncVectors->length){
        result.vx /= syncVectors->length;
        result.vy /= syncVectors->length;
    }
    forEach(syncVectors, freeVectRot);
    freeList(syncVectors);
    return result;
}

vectAi getVectDirectNode(GameObject *robotObj){
    robot *currentRobot = robotObj->actor;
    vectAi result = vectRotToVectAi((vectRot){currentRobot->maxSpeed*5, atan2f(currentRobot->targetNode->x - currentRobot->x,currentRobot->targetNode->y - currentRobot->y)});
    return result;
}

vectAi getVectTargetNode(GameObject *robotObj){
    robot *currentRobot = robotObj->actor;
    if(!currentRobot->lastNode->nextAlt){
        vectAi result = vectRotToVectAi((vectRot){currentRobot->maxSpeed*0.6, atan2f(currentRobot->targetNode->x - currentRobot->lastNode->x,currentRobot->targetNode->y - currentRobot->lastNode->y)});
        return result;
    } else return getVectDirectNode(robotObj);
}

vectAi getVectFireRange(GameObject *robotObj){
    robot *currentRobot = robotObj->actor;
    GameObject *coreObj = robotObj->game->coreObj;
    core *core = coreObj->actor;
    int coreX = core->node->x, coreY = core->node->y;
    float delta = distBetweenTwoPoints(coreX, coreY, currentRobot->x, currentRobot->y) - currentRobot->range - (core->radius/2);
    vectRot vectToTargetDist = {delta, atan2f(coreX - currentRobot->x, coreY - currentRobot->y)};
    return vectRotToVectAi(vectToTargetDist);
}

vectAi getVectBorderProximity(GameObject *robotObj){
    robot *currentRobot = robotObj->actor;
    map_dataGrid dataGrid = robotObj->game->mapManager->currentMap->dataGrid;
    list *escapeVectors = newList(COMPARE_PTR);
    int samplingRate = 2;
    int currentX = round(currentRobot->x);
    int currentY = round(currentRobot->y);
    for(int x = currentX - (currentRobot->radius * 2); x < currentX + (currentRobot->radius * 2); x++){
        for(int y = currentY - (currentRobot->radius * 2); y < currentY + (currentRobot->radius * 2); y++){
            if(x >= 0 && x < dataGrid.w && y >= 0 && y < dataGrid.h){
                if(!((x*x + y*y) % samplingRate)){
                    map_cell *cell = getCellFromDataGrid(dataGrid, x, y);
                    if(cell->type != MCT_PATH && cell->type != MCT_PATH_NS){
                        float distInBetween = distBetweenTwoPoints(currentRobot->x, currentRobot->y, x, y);
                        vectRot *ev = malloc(sizeof(vectRot));
                        ev->v = pow((currentRobot->radius/distInBetween),samplingRate*2);
                        ev->teta = modulo2Pi(atan2f(x - currentRobot->x, y - currentRobot->y) + M_PI);
                        appendInList(escapeVectors, ev);
                    }
                }
            }
        }
    }
    vectAi result = {0.0,0.0};
    if(escapeVectors->length > samplingRate*samplingRate*samplingRate){
        result = getVectDirectNode(robotObj);
    }else{
        for(int index = 0; index < escapeVectors->length; index++){
            vectRot *vectRot = getDataAtIndex(*escapeVectors, index);
            vectAi vectAi = vectRotToVectAi(*vectRot);
            result.vx += vectAi.vx;
            result.vy += vectAi.vy;
        }

        if(escapeVectors->length){
            result.vx /= escapeVectors->length;
            result.vy /= escapeVectors->length;
        }
    }
    forEach(escapeVectors, freeVectRot);
    freeList(escapeVectors);
    return result;
}

bool isRobotAfterTargetNode(robot robot){
    int x = robot.x;
    int y = robot.y;
    int Ax = robot.targetNode->x;
    int Ay = robot.targetNode->y;
    vectRot ori;
    ori.teta = modulo2Pi(atan2f(robot.targetNode->x - robot.lastNode->x, robot.targetNode->y - robot.lastNode->y) + (M_PI * 0.5));
    ori.v = 100;
    vectAi oriRef = vectRotToVectAi(ori);
    int Bx = Ax + oriRef.vx;
    int By = Ay + oriRef.vy;
    return (Bx - Ax) * (y - Ay) - (By - Ay) * (x - Ax) >= 0;
}

void updateRobotPathAi(GameObject *robotObj){
    robot *robot = robotObj->actor;
    vectAi newSpeedVect = {0,0}; 
    GameObject *coreObj = robotObj->game->coreObj;
    core *core = coreObj->actor;
    int coreX = core->node->x, coreY = core->node->y;
    float coreDistance = distBetweenTwoPoints(coreX, coreY, robot->x, robot->y);
    vectAi allyProximity = getVectAllyProximity(robotObj);
    vectAi borderProximity = getVectBorderProximity(robotObj);
    if(coreDistance < robot->range*2){
        vectAi vectFireRange = getVectFireRange(robotObj);
        newSpeedVect.vx = allyProximity.vx + borderProximity.vx + robot->speedx;
        newSpeedVect.vy = allyProximity.vy + borderProximity.vy + robot->speedy;
        newSpeedVect.vx /= 6;
        newSpeedVect.vy /= 6;
        newSpeedVect.vx += vectFireRange.vx;
        newSpeedVect.vy += vectFireRange.vy;
    }else if(robot->targetNode->next){
        vectAi swarmProximity = getVectSwarmProximity(robotObj);
        vectAi targetNode = getVectTargetNode(robotObj);
        newSpeedVect.vx = allyProximity.vx + borderProximity.vx + swarmProximity.vx + targetNode.vx;
        newSpeedVect.vy = allyProximity.vy + borderProximity.vy + swarmProximity.vy + targetNode.vy;
        if(isRobotAfterTargetNode(*robot)){
            robot->lastNode = robot->targetNode;
            if(robot->targetNode->nextAlt){
                if(robot->seed % 2) robot->targetNode = robot->targetNode->nextAlt;
                else robot->targetNode = robot->targetNode->next;
                robot->seed /= 2;
            }else robot->targetNode = robot->targetNode->next;
        }
    }
    newSpeedVect = crunchVectAi(newSpeedVect, robot->maxSpeed);
    robot->speedy = newSpeedVect.vy;
    robot->speedx = newSpeedVect.vx;
}

void updateShotBehavior(GameObject *robotObj){
    robot *robot = robotObj->actor;
    core *core = robotObj->game->coreObj->actor;
    int coreDistance = distBetweenTwoPoints(core->node->x, core->node->y, robot->x, robot->y);
    if(robot->range == 0){
        if(coreDistance < (core->radius*1.5)){
            robot->life = 0;
        }
    }
    if(coreDistance < robot->range*1.5){
        if(robot->delayCounter > 0){
            robot->delayCounter--;
        }else{
            robot->delayCounter = robot->delay;
            float rotation = modulo2Pi(atan2f(core->node->x - robot->x, core->node->y - robot->y) + M_PI);
            int spawnX = robot->x - (sinf(rotation) * (robot->width/2));
            int spawnY = robot->y - (cosf(rotation) * (robot->height/2));
            robotObj->game->projectileManager->newProjectile(robotObj->game, robot->projectileName, spawnX, spawnY, rotation, robotObj, NULL);
        }
    }
}