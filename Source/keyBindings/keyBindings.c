#include "keyBindings.h"
#include "../Game/game.h"
#include "../Game/camera.h"
#include "../Turrets/turrets.h"
#include "../Robots/robots.h"
#include "../Turrets/turretUsher.h"
#include "../Game/selection.h"
#include "../Game/gameManager.h"
#include "../UI/UI_pause.h"

static list *bindings = NULL;
static list* keyCodeTextureAsso = NULL;

int Comparator(void *needle, void *data) {
    keyBinding *n1 = needle;
    keyBinding *n2 = data;
    if (n1->key == n2->key && n1->input == n2->input) {
        return 0;
    }
    return 1;
}

int getKBEnumFromString(char *string)
{
    if(!strcmp(string,"GA_UP")){return GA_UP;}
    if(!strcmp(string,"GA_DOWN")){return GA_DOWN;}
    if(!strcmp(string,"GA_LEFT")){return GA_LEFT;}
    if(!strcmp(string,"GA_RIGHT")){return GA_RIGHT;}
    if(!strcmp(string,"GA_PAUSE")){return GA_PAUSE;}
    if(!strcmp(string,"GA_UPGRADE")){return GA_UPGRADE;}
    if(!strcmp(string,"GA_SELL")){return GA_SELL;}
    if(!strcmp(string,"GA_ZOOMIN")){return GA_ZOOMIN;}
    if(!strcmp(string,"GA_ZOOMOUT")){return GA_ZOOMOUT;}
    if(!strcmp(string,"GA_TURRET1")){return GA_TURRET1;}
    if(!strcmp(string,"GA_TURRET2")){return GA_TURRET2;}
    if(!strcmp(string,"GA_TURRET3")){return GA_TURRET3;}
}

list *KB_init(void *self) {
    Game *game = self;
    bindings = newList(Comparator);
    FILE *keys_file = fopen("keybindings.kb","r");
    char *line = malloc(sizeof(char)*255);
    while(fgets(line,255,keys_file)!=NULL)
    {
        
        char *action = malloc(sizeof(char)*255);
        char *char_sdlk = malloc(sizeof(char)*255);
        sscanf(line,"%[^ ] %[^\n]",action,char_sdlk);
        int int_sdlk = atoi(char_sdlk);
        int kbEnum = getKBEnumFromString(action);
        KB_add(int_sdlk,getKBEnumFromString(action));
        free(action);
        free(char_sdlk);
    }
    free(line);
    fclose(keys_file);

    keyTexture *temp = NULL;
    keyCodeTextureAsso = newList(COMPARE_PTR);
    temp = malloc(sizeof(keyTexture));
    *temp = (keyTexture){SDLK_UP,game->textureManager->getTexture("UI_icon_key_move_up.png")};
    appendInList(keyCodeTextureAsso,temp);
    temp = malloc(sizeof(keyTexture));
    *temp = (keyTexture){SDLK_DOWN,game->textureManager->getTexture("UI_icon_key_move_down.png")};
    appendInList(keyCodeTextureAsso,temp);
    temp = malloc(sizeof(keyTexture));
    *temp = (keyTexture){SDLK_LEFT,game->textureManager->getTexture("UI_icon_key_move_left.png")};
    appendInList(keyCodeTextureAsso,temp);
    temp = malloc(sizeof(keyTexture));
    *temp = (keyTexture){SDLK_RIGHT,game->textureManager->getTexture("UI_icon_key_move_right.png")};
    appendInList(keyCodeTextureAsso,temp);

    return bindings;
}

void saveKeysInFile(void *self)
{
    keyBinding *k = self;
    FILE *keys_file = fopen("keybindings.kb","a");
    char *string = malloc(sizeof(char)*255);
    switch(k->input)
    {
        case GA_UP:
            strcpy(string,"GA_UP");
            break;
        case GA_DOWN:
            strcpy(string,"GA_DOWN");
            break;
        case GA_LEFT:
            strcpy(string,"GA_LEFT");
            break;
        case GA_RIGHT:
            strcpy(string,"GA_RIGHT");
            break;
        case GA_PAUSE:
            strcpy(string,"GA_PAUSE");
            break;
        case GA_SELL:
            strcpy(string,"GA_SELL");
            break;
        case GA_UPGRADE:
            strcpy(string,"GA_UPGRADE");
            break;
        case GA_ZOOMIN:
            strcpy(string,"GA_ZOOMIN");
            break;
        case GA_ZOOMOUT:
            strcpy(string,"GA_ZOOMOUT");
            break;
        case GA_TURRET1:
            strcpy(string,"GA_TURRET1");
            break;
        case GA_TURRET2:
            strcpy(string,"GA_TURRET2");
            break;
        case GA_TURRET3:
            strcpy(string,"GA_TURRET3");
            break;
    }
    fprintf(keys_file,"%s %d\n",string,k->key);
    fclose(keys_file);
}

void KB_free() {
    FILE *keys_file = fopen("keybindings.kb","w");
    fclose(keys_file);
    forEach(bindings, saveKeysInFile);
    forEach(bindings, free);
    freeList(bindings);
}

void KB_add(SDL_Keycode key, gameInput input) {
    keyBinding *kb = malloc(sizeof(keyBinding));
    kb->key = key;
    kb->input = input;
    appendInList(bindings, kb);
}

void KB_removeKeyBinding(keyBinding *kb) {
    deleteInList(bindings, kb);
}

list *KB_getInputs(SDL_Keycode key) {
    list *r = newList(COMPARE_PTR);
    void search(void *haystack) {
        keyBinding *d = haystack;
        if (d->key == key) {
            pushInList(r, &d->input);
        }
    }
    forEach(bindings, search);
    return r;
}

list *KB_getKeys(gameInput input) {
    list *r = newList(COMPARE_PTR);
    void search(void *haystack) {
        keyBinding *d = haystack;
        if (d->input == input) {
            pushInList(r, &d->key);
        }
    }
    forEach(bindings, search);
    return r;
}

keyBinding *KB_getKeyBinding(SDL_Keycode key, gameInput input) {
    keyBinding *kb = malloc(sizeof(keyBinding));
    kb->key = key;
    kb->input = input;
    keyBinding *r = searchDataInList(*bindings, kb);
    free(kb);
    if (r != NULL) {
        return r;
    } else {
        return NULL;
    }
}

static void printing(void *kb) {
    keyBinding *this = kb;
    if (kb != NULL) {
        printf("%d %d\n", this->key, this->input);
    } else {
        printf("KB is null\n");
    }
}

void printList(list *r) {
    forEach(r, printing);
}

void printed() {
    printList(bindings);
}

static void (*callback)(SDL_Keycode code,void *data) = NULL;
static void *data = NULL;

void KB_getInput(void (*cb)(SDL_Keycode code,void *data),void *d)
{
    if(callback) callback(-1,data);
    callback = cb;
    data = d;
}

GameObject *getObjectUnderMouse(Game *game){
    int mx = game->mouseX;
    int my = game->mouseY;
    GameObject *obj = NULL;
    void getObjUnderMouse(void *self){
        GameObject *o = self;
        if(o->type == GOT_Turret){
            turret *t = o->actor;
            SDL_Rect turretArea = (SDL_Rect){t->x,t->y,t->width,t->height};
            projectRectToCamera(&turretArea);
            if(SDL_PointInRect(&(SDL_Point){mx,my},&turretArea)){
                obj = o;
            }
        }
        if(o->type == GOT_Robot){
            robot *r = o->actor;
            SDL_Rect robotArea = (SDL_Rect){r->x -(r->width/2)-10,r->y-(r->height/2)-10,r->width+10,r->height+10};
            projectRectToCamera(&robotArea);
            if(SDL_PointInRect(&(SDL_Point){mx,my},&robotArea)){
                obj = o;
            }
        }
    }
    forEach(game->gameObjects,getObjUnderMouse);
    return obj;
}

void KB_handleKeyCode(SDL_Keycode code, Game *game)
{
    if(callback)
    {
        callback(code,data);
        callback = NULL;
        data = NULL;
        return;
    }
    list *inputs = KB_getInputs(code);

    void handleInput(void *input) {
        GA_type *i = input;
        if(*i == GA_TURRET1 || *i == GA_TURRET2 || *i == GA_TURRET3)
        {
            if(!isGameModeActive()) return;
            list *tsl = generateTurretsSelection(game);
            Selection *curSel = game->selection;
            if(curSel && curSel->type == SELECT_TURRET){
                turretSelection *current = curSel->selected.turretSelection;
                switch (*i)
                {
                    case GA_TURRET1:;
                        turretSelection *t0 = getDataAtIndex(*tsl,0);
                        if(!strcmp(current->turretId, t0->turretId)){
                            free(curSel);
                            game->selection = NULL;
                            return;
                        }
                        break;
                    case GA_TURRET2:;
                        turretSelection *t1 = getDataAtIndex(*tsl,1);
                        if(!strcmp(current->turretId, t1->turretId)){
                            free(curSel);
                            game->selection = NULL;
                            return;
                        }
                        break;
                    case GA_TURRET3:;
                        turretSelection *t2 = getDataAtIndex(*tsl,2);
                        if(!strcmp(current->turretId, t2->turretId)){
                            free(curSel);
                            game->selection = NULL;
                            return;
                        }
                        break;
                }
            }
            if(curSel) free(curSel);
            Selection *selection = malloc(sizeof(Selection));
            selection->type = SELECT_TURRET;
            switch (*i)
            {
            case GA_TURRET1:
                selection->selected.turretSelection = getDataAtIndex(*tsl, 0);
                break;
            case GA_TURRET2:
                selection->selected.turretSelection = getDataAtIndex(*tsl, 1);
                break;
            case GA_TURRET3:
                selection->selected.turretSelection = getDataAtIndex(*tsl, 2);
                break;
            default:
                break;
            }
            game->selection = selection;
        }
        GameObject *obj = getObjectUnderMouse(game);
        Selection *selection = game->selection;
        GameObject *selected = NULL;
        if(selection && selection->type == SELECT_GAMEOBJECT)
            selected = selection->selected.gameObject;
        switch (*i)
        {
        case GA_DOWN:
            if(!game->pause) cameraMove(0,10);
            break;
        case GA_UP:
            if(!game->pause) cameraMove(0,-10);
            break;
        case GA_LEFT:
            if(!game->pause) cameraMove(-10,0);
            break;
        case GA_RIGHT:
            if(!game->pause) cameraMove(10,0);
            break;
        case GA_PAUSE:
            if(game->mapManager->currentMap)
            {
                game->pause = !game->pause;
                if(game->pause)
                {
                    catchPause();
                }
                else
                {
                    throwPause();
                }
            }
            break;
        case GA_SELL:
            if(selected && selected->type == GOT_Turret)
                sellTurret(selected);
            else sellTurret(obj);
            break;
        case GA_UPGRADE:
            if(selected && selected->type == GOT_Turret)
                upgradeTurret(selected);
            else upgradeTurret(obj);
            break;
        case GA_ZOOMIN:
            if(!game->pause) cameraZoom(0.2);
            break;
        case GA_ZOOMOUT:
            if(!game->pause) cameraZoom(-0.2);
            break;
        default:
            break;
        }
    }
    forEach(inputs, handleInput);
}

list *KB_getKCTA()
{
    return keyCodeTextureAsso;
}