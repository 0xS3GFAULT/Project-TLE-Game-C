/**
 * @file lang.h
 * @brief Manage all the langs
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../List/SmartList.h"

#ifndef LANGMANAGER_H
#define LANGMANAGER_H

/** @brief a language Manager*/
typedef struct {
    list *binds;
    /** @brief gives the traduction for an element
        @param idToGet the id of the element translated
        @returns a pointer to the translation
    */
    char **(*getTradById)(char *idToGet);
    /** @brief loads a language
        @param langId the id of the language loaded
    */
    void (*loadLang)(char *langId);
    /** @brief deletes the language Manager*/
    void (*clear)(void);
    char *currentLangId;
    list *availableLangIds;
} langManager;

/** @brief an association between a traduction standard id and the traduction itself*/
typedef struct {
    char *id;
    char **trad;
} langBind;

/** @brief initializes the language Manager
    @return the new language Manager
    */
langManager *initLanguageManager();

#endif