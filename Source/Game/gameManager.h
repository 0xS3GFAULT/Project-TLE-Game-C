/**
 * @file gameManager.h
 * @brief Manage the game Manager
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @brief Structure that holds the data of the game (currencies)
 */
typedef struct gameModeData
{
    int currencyA;
    int currencyB;
    int currencyC;
} gameModeData;

/** @brief Updates the game manager
*/
void updateGameManager();

/** @brief Launches the main menu
*  @param GAME the current main game system 
*/
void launchMainMenu(void *GAME);

/** @brief Launches the endless mode
*  @param GAME the current main game system
*/
void launchEndlessMode(void *GAME);

/** @brief Launches the sandbox mode
*  @param GAME the current main game system
*/
void launchSandboxMode(void *game);

/** @brief Updates the game states
*  @param GAME the current main game system to be updated
*/
void updateCurrentGameState(void *GAME);

/** @brief Gets the data of the current mode of the game
   @return the game mode data
*/
gameModeData getGameModeData(void);

/** @brief Sets the data of the current mode of the game
*  @param data the game mode data to set to the current one
*/
void setGameModeData(gameModeData data);

/** @brief Quit current game mode
*/
void endCurrentMode();
void endEndlessMode();
bool isGameModeActive();