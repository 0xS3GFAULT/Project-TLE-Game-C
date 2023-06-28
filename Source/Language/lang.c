#include <dirent.h>
#include "lang.h"
#include "../Game/game.h"

langManager *LANG_MANAGER = NULL;

int compareTradId(void *data1, void *data2){
    langBind *A = data1;
    langBind *B = data2;
    return strcmp(A->id, B->id);
}

char **getTradById(char *idToGet)
{
    langBind *request = malloc(sizeof(langBind));
    request->id = idToGet;
    langBind *result = searchDataInList(*LANG_MANAGER->binds, request);
    if(result == NULL)
    {
        request->id = malloc(sizeof(char)*(strlen(idToGet)+1));
        strcpy(request->id, idToGet);
        request->trad = malloc(sizeof(char *));
        *request->trad = malloc(sizeof(char)*(strlen(idToGet)+1));
        strcpy(*request->trad, request->id);
        appendInList(LANG_MANAGER->binds, request);
        return getTradById(idToGet);
    }
    free(request);
    return result->trad;
}

void loadLang(char *langId)
{
    char *langStr = searchDataInList(*LANG_MANAGER->availableLangIds, langId);

    if(langStr == NULL)
    {
        printf("Language \"%s\" not found.\n", langId);
        return;
    }

    char path[255];
    char strId[255];
    char strTrad[4096];
    char line_cache[4096];
    sprintf(path, "./assets/localisation/%s.csv", langId);
    FILE *csv_file = fopen(path,"r");
    if(!csv_file){
        printf("\033[1;31mLANG ERROR : %s not found.\033[0m\n", path);
    }else{
        while(fgets(line_cache,4096,csv_file) != NULL)
        {
            sscanf(line_cache,"%[^;];%[^\n]", strId, strTrad);
            char **trad = getTradById(strId);

            *trad = realloc(*trad,sizeof(char)*(strlen(strTrad)+1));
            strcpy(*trad, strTrad);
        }
        LANG_MANAGER->currentLangId = langStr;
        fclose(csv_file);
    }
}

void langClear(){
    for(int index = 0; index < LANG_MANAGER->binds->length; index++){
        langBind *b = getDataAtIndex(*LANG_MANAGER->binds, index);
        free(*b->trad);
        free(b->trad);
        free(b->id);
        free(b);
    }
    freeList(LANG_MANAGER->binds);
}

int compareString(void *data1, void *data2){
    char *A = data1;
    char *B = data2;
    return strcmp(A, B);
}

langManager *initLanguageManager() {
    LANG_MANAGER = malloc(sizeof(langManager));
    LANG_MANAGER->binds = newList(compareTradId);
    LANG_MANAGER->availableLangIds = newList(compareString);
    LANG_MANAGER->getTradById = getTradById;
    LANG_MANAGER->loadLang = loadLang;
    LANG_MANAGER->clear = langClear;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("./assets/localisation")) != NULL) {
        while ((ent = readdir (dir)) != NULL) {
            char *fileName = ent->d_name;
            if(strcmp(fileName, ".") != 0 && strcmp(fileName, "..") != 0)
            {
                char *langId = malloc(sizeof(char)*7);
                strncpy(langId, fileName, strlen(fileName)-4);
                langId[5] = '\0';
                appendInList(LANG_MANAGER->availableLangIds, langId);
            }
        }
        closedir(dir);
    } else {
        perror("Unable to open directory : ./assets/localisation");
        return NULL;
    }

    LANG_MANAGER->loadLang("en_US");
    return LANG_MANAGER;
}