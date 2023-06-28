/**
 * @file animationManager.h
 * @brief manage all the animated textures
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "../List/SmartList.h"

typedef struct animation animation;

/** @brief Structure holding the animation manager */
typedef struct
{
    /** @brief Add an animation and push its id 
    *  @param id the id of the animation
    *  @param toIncrement the id of the animation to increment
    *  @param nOfFrames the number of frames of the animation
    *  @param delay the delay between each frame
    */
    void (*addAnim)(char* id, int* toIncrement, int nOfFrames, int delay);

    /** @brief Kill an animation and free its memory
    *   @param id the id of the animation
    */
    void (*killAnim)(char* id);

    /** @brief Kill an animation and free its memory
    *   @param id the id of the animation
    *   @return the animation found 
    */
    animation *(*getAnim)(char* id);
    
    /** list of current animations */
    list *animList;
} animationManager;

/** @brief Structure holding an animation */
struct animation
{
    /** @brief Update the animation passed as a parameter
    *  @param self the animation to update
    */
    void (*update)(void *self);
    /** id of an animation */
    char* id;
    /** counter that increments according to the number of frames */
    int* toIncrement;
    /** number of frames */
    int nOfFrames;
    /** delay between each frame */
    int delay;
    /** timer which makes the animation to wait */
    int timer;
     /** the animation manager */
    animationManager *manager;
};

 /** @brief Create and init the animation manager
 *   @return the animation manager created
 */
animationManager *initAnimManager();