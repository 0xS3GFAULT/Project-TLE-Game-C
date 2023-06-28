#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include <dirent.h>
#include "sound-engine.h"
#include "listManager.h"

static MusicStorageList musicStorage = {NULL, 0};
static StorageList soundStorage = {NULL, 0};
static StorageList ambientStorage = {NULL, 0};
static StorageList dialogStorage = {NULL, 0};

static RequestedList requested = {NULL, 0};

static PlayingList playing = {NULL, 0};

static float *musicWeight = NULL;

musicTrack *getTrack(const char *trackName)
{
    return getFromMusicStorageList(&musicStorage, trackName);
}

soundEffect *getEffect(const char *effectName)
{
    return getFromStorageList(&soundStorage, effectName);
}

dialog *getDialog(const char *dialogName)
{
    return getFromStorageList(&dialogStorage, dialogName);
}

ambientTrack *getAmbientTrack(const char *ambientName)
{
    return getFromStorageList(&ambientStorage, ambientName);
}

void playTrack(musicTrack *track)
{
    SE_bool somethingPlaying = playing._qty > 0;
    if (track == NULL)
        return;
    RequestedElement *requestedElement = calloc(1, sizeof(RequestedElement));
    requestedElement->_data = track;
    requestedElement->_next = NULL;
    addToRequestedList(&requested, requestedElement);
    if (!somethingPlaying)
    {
        for (int i = 0; i < track->_qty; i++)
        {
            if (track->_music[i]._type == SE_MUSIC && track->_weights[i] <= *musicWeight)
            {
                PlayingElement *playingElement = calloc(1, sizeof(PlayingElement));
                playingElement->_data = &(track->_music[i]);
                playingElement->_next = NULL;
                playingElement->_channel = Mix_PlayChannel(-1, playingElement->_data->_audio, -1);
                if(playingElement->_channel == -1)
                {
                    printf("\033[1;31mSoundEngine PlayTrack Error : %s\n\033[0m", Mix_GetError());
                    free(playingElement);
                    break;
                }
                Mix_Volume(playingElement->_channel, (int)(getMusicVolume() * getMasterVolume() * 128));
                addToPlayingList(&playing, playingElement);
            }
        }
    }
}

void playTrackEffect(int effectId)
{
    if (requested._qty == 0)
        return;
    RequestedElement *requestedElement = requested._begin;
    while (requestedElement != NULL)
    {
        for (int i = 0; i < requestedElement->_data->_qty; i++)
        {
            if (requestedElement->_data->_music[i]._type == SE_MUSIC_FX && (int)(requestedElement->_data->_weights[i]) == effectId)
            {
                PlayingElement *playingElement = calloc(1, sizeof(PlayingElement));
                playingElement->_data = &(requestedElement->_data->_music[i]);
                playingElement->_next = NULL;
                playingElement->_channel = Mix_PlayChannel(-1, playingElement->_data->_audio, -1);
                if(playingElement->_channel == -1)
                {
                    printf("\033[1;31mSoundEngine PlayTrackEffect Error : %s\n\033[0m", Mix_GetError());
                    free(playingElement);
                    break;
                }
                Mix_Volume(playingElement->_channel, (int)(getMusicVolume() * getMasterVolume() * 128));
                addToPlayingList(&playing, playingElement);
            }
        }
        requestedElement = requestedElement->_next;
    }
}

void stopTracks()
{
    RequestedElement *requestedElement = requested._begin;
    while (requestedElement != NULL)
    {
        removeFromRequestedList(&requested, requestedElement);
        requestedElement = requested._begin;
    }
}

void pauseTracks()
{
    PlayingElement *playingElement = playing._begin;
    while (playingElement != NULL)
    {
        if (playingElement->_data->_type == SE_MUSIC || playingElement->_data->_type == SE_MUSIC_FX)
        {
            Mix_Pause(playingElement->_channel);
        }
        playingElement = playingElement->_next;
    }
}

void resumeTracks()
{
    PlayingElement *playingElement = playing._begin;
    while (playingElement != NULL)
    {
        if (playingElement->_data->_type == SE_MUSIC || playingElement->_data->_type == SE_MUSIC_FX)
        {
            if (Mix_Paused(playingElement->_channel) == 1)
                Mix_Resume(playingElement->_channel);
            else
            {
                Mix_HaltChannel(playingElement->_channel);
                PlayingElement *tmp = playingElement;
                playingElement = playingElement->_next;
                removeFromPlayingList(&tmp, playingElement);
                continue;
            }
        }
        playingElement = playingElement->_next;
    }
}

soundEffectInstance *playEffect(soundEffect *effect)
{
    if (effect == NULL)
        return NULL;
    soundEffectInstance *instance = malloc(sizeof(soundEffectInstance));
    instance->_data = effect;
    instance->_channel = Mix_PlayChannel(-1, effect->_audio, 0);
    if(instance->_channel == -1)
    {
        printf("\033[1;31mSoundEngine PlayEffect Error : %s\n\033[0m", Mix_GetError());
        free(instance);
        return NULL;
    }
    Mix_Volume(instance->_channel, (int)(getEffectVolume() * getMasterVolume() * 128));
    instance->_next = playing._begin;
    playing._begin = instance;
    playing._qty++;
    return instance;
}

void stopEffect(soundEffectInstance **instance)
{
    if (*instance == NULL)
        return;
    if (Mix_Playing((*instance)->_channel))
        Mix_HaltChannel((*instance)->_channel);
    removeFromPlayingList(&playing, *instance);
    *instance = NULL;
}

dialogInstance *playDialog(dialog *dialog)
{
    dialogInstance *instance = malloc(sizeof(dialogInstance));
    instance->_data = dialog;
    instance->_channel = Mix_PlayChannel(-1, dialog->_audio, 0);
    if(instance->_channel == -1)
    {
        printf("\033[1;31mSoundEngine PlayDialog Error : %s\n\033[0m", Mix_GetError());
        free(instance);
        return NULL;
    }
    Mix_Volume(instance->_channel, (int)(getDialogVolume() * getMasterVolume() * 128));
    instance->_next = playing._begin;
    playing._begin = instance;
    playing._qty++;
    return instance;
}

void stopDialog(dialogInstance **instance)
{
    if (*instance == NULL)
        return;
    if (Mix_Playing((*instance)->_channel))
        Mix_HaltChannel((*instance)->_channel);
    removeFromPlayingList(&playing, *instance);
    free(*instance);
    *instance = NULL;
}

ambientTrackInstance *playAmbientTrack(ambientTrack *track)
{
    ambientTrackInstance *instance = malloc(sizeof(ambientTrackInstance));
    instance->_data = track;
    instance->_channel = Mix_PlayChannel(-1, track->_audio, 0);
    if(instance->_channel == -1)
    {
        printf("\033[1;31mSoundEngine PlayAmbientTrack Error : %s\n\033[0m", Mix_GetError());
        free(instance);
        return NULL;
    }
    Mix_Volume(instance->_channel, (int)(getAmbientVolume() * getMasterVolume() * 128));
    instance->_next = playing._begin;
    playing._begin = instance;
    playing._qty++;
    return instance;
}

void stopAmbientTrack(ambientTrackInstance **instance)
{
    if (*instance == NULL)
        return;
    if (Mix_Playing((*instance)->_channel))
        Mix_HaltChannel((*instance)->_channel);
    removeFromPlayingList(&playing, *instance);
    free(*instance);
    *instance = NULL;
}

void pause()
{
    Mix_Pause(-1);
}

void resume()
{
    PlayingElement *Temp = playing._begin, *tmp = NULL;
    while (Temp != NULL)
    {
        if (Mix_Paused(Temp->_channel))
        {
            Mix_Resume(Temp->_channel);
            Temp = Temp->_next;
        }
        else
        {
            Mix_HaltChannel(Temp->_channel);
            tmp = Temp;
            Temp = Temp->_next;
            removeFromPlayingList(&playing, tmp);
            free(tmp);
        }
    }
}

void refreshVolume(float master, float music, float effect, float ambient, float dialog)
{
    PlayingElement *Temp = playing._begin;
    while (Temp != NULL)
    {
        switch (Temp->_data->_type)
        {
        case SE_MUSIC:
        case SE_MUSIC_FX:
            Mix_Volume(Temp->_channel, (int)(master * music * 128));
            break;
        case SE_SFX:
            Mix_Volume(Temp->_channel, (int)(master * effect * 128));
            break;
        case SE_AMBIENT:
            Mix_Volume(Temp->_channel, (int)(master * ambient * 128));
            break;
        case SE_DIALOG:
            Mix_Volume(Temp->_channel, (int)(master * dialog * 128));
            break;
        default:
            break;
        }
        Temp = Temp->_next;
    }
}

void MusicCallback(int channel)
{
    QueuedList queued = {NULL, 0};
    RequestedElement *TMPreq = requested._begin;
    while (TMPreq != NULL)
    {
        QueuedElement *tmp = calloc(1, sizeof(QueuedElement));
    }
    PlayingElement *Played = getFromPlayingList(&playing, channel);
    if (Played == NULL)
        return;
    StorageElement *data = Played->_data;
    if ((data->_type != SE_MUSIC && data->_type != SE_AMBIENT) ||
        getFromRequestList(&requested, data) == NULL)            
    {
        removeFromPlayingList(&playing, Played);
        free(Played);
        return;
    }
    Played->_channel = Mix_PlayChannel(-1, Played->_data->_audio, 0); 
    switch (data->_type)
    {
    case SE_MUSIC:
    case SE_MUSIC_FX:
        Mix_Volume(Played->_channel, (int)(getMusicVolume() * getMasterVolume() * 128));
        break;
    case SE_SFX:
        Mix_Volume(Played->_channel, (int)(getEffectVolume() * getMasterVolume() * 128));
        break;
    case SE_AMBIENT:
        Mix_Volume(Played->_channel, (int)(getAmbientVolume() * getMasterVolume() * 128));
        break;
    case SE_DIALOG:
        Mix_Volume(Played->_channel, (int)(getDialogVolume() * getMasterVolume() * 128));
        break;
    default:
        break;
    }
}

void loadMusicTrack(const char *DirName, const char *MusicName)
{
    char *File = calloc(strlen(DirName) + strlen(MusicName) + 6, sizeof(char));
    strcpy(File, DirName);
    strcat(File, MusicName);
    strcat(File, ".json");
    json_object *json = json_object_from_file(File);
    free(File);

    json_object *jsonMusic = json_object_object_get(json, "music");
    json_object *jsonEffect = json_object_object_get(json, "sfx");
    size_t nbMusic = json_object_array_length(jsonMusic);
    size_t nbEffect = json_object_array_length(jsonEffect);

    MusicStorageElement *music = calloc(1, sizeof(MusicStorageElement));
    music->_qty = nbMusic + nbEffect;
    music->_music = calloc(music->_qty, sizeof(StorageElement));
    music->_weights = calloc(music->_qty, sizeof(float));
    music->_name = calloc(strlen(MusicName) + 1, sizeof(char));
    strcpy(music->_name, MusicName);
    music->_next = NULL;

    for (int i = 0; i < nbMusic; i++)
    {
        json_object *jsonMusicElement = json_object_array_get_idx(jsonMusic, i);
        json_object *jsonMusicName = json_object_object_get(jsonMusicElement, "name");
        json_object *jsonMusicWeight = json_object_object_get(jsonMusicElement, "weight");
        music->_music[i]._type = SE_MUSIC;
        music->_music[i]._name = calloc(strlen(json_object_get_string(jsonMusicName)), sizeof(char));
        strcpy(music->_music[i]._name, json_object_get_string(jsonMusicName));
        music->_weights[i] = json_object_get_double(jsonMusicWeight);
        char *path = calloc(strlen(DirName) + strlen(music->_music[i]._name) + 1, sizeof(char));
        strcpy(path, DirName);
        strcat(path, music->_music[i]._name);
        music->_music[i]._audio = Mix_LoadWAV(path);
        free(path);
        music->_music[i]._next = NULL;
    }
    for (int i = 0; i < nbEffect; i++)
    {
        json_object *jsonEffectElement = json_object_array_get_idx(jsonEffect, i);
        json_object *jsonEffectName = json_object_object_get(jsonEffectElement, "name");
        json_object *jsonEffectId = json_object_object_get(jsonEffectElement, "Id");
        music->_music[i + nbMusic]._type = SE_MUSIC_FX;
        music->_music[i + nbMusic]._name = calloc(strlen(json_object_get_string(jsonEffectName)) + 1, sizeof(char));
        strcpy(music->_music[i + nbMusic]._name, json_object_get_string(jsonEffectName));
        music->_weights[i + nbMusic] = json_object_get_int(jsonEffectId);
        char *path = calloc(strlen(DirName) + strlen(music->_music[i + nbMusic]._name) + 1, sizeof(char));
        strcpy(path, DirName);
        strcat(path, music->_music[i + nbMusic]._name);
        music->_music[i + nbMusic]._audio = Mix_LoadWAV(path);
        free(path);
        music->_music[i + nbMusic]._next = NULL;
    }
    addToMusicStorageList(&musicStorage, music);
}

void loadSFX(const char *DirName)
{
    DIR *dir = opendir(DirName);
    struct dirent *ent;
    if (dir == NULL)
        return;
    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        char *path = calloc(strlen(DirName) + strlen(ent->d_name) + 10, sizeof(char));
        sprintf(path, "%s/%s", DirName, ent->d_name);
        StorageElement *sfx = calloc(1, sizeof(StorageElement));
        sfx->_type = SE_SFX;
        sfx->_name = calloc(strlen(ent->d_name) + 1, sizeof(char));
        strcpy(sfx->_name, ent->d_name);
        if (strstr(ent->d_name, ".wav") == NULL)
        {
            free(path);
            free(sfx->_name);
            free(sfx);
            continue;
        }
        sfx->_audio = Mix_LoadWAV(path);
        if(sfx->_audio == NULL)
        {
            printf("\033[1;31mSoundEngine LoadSFX Error : (path:%s) %s\n\033[0m", path, Mix_GetError());
            free(path);
            free(sfx->_name);
            free(sfx);
            continue;
        }
        free(path);
        sfx->_next = NULL;
        addToStorageList(&soundStorage, sfx);
    }
    closedir(dir);
}

void loadDialogs(const char *DirName)
{
    DIR *dir = opendir(DirName);
    struct dirent *ent;
    if (dir == NULL)
        return;
    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        char *path = calloc(strlen(DirName) + strlen(ent->d_name) + 1, sizeof(char));
        sprintf(path, "%s/%s", DirName, ent->d_name);
        StorageElement *dialog = calloc(1, sizeof(StorageElement));
        dialog->_type = SE_DIALOG;
        dialog->_name = calloc(strlen(ent->d_name) + 1, sizeof(char));
        strcpy(dialog->_name, ent->d_name);
        if (strstr(ent->d_name, ".wav") == NULL)
        {
            free(path);
            free(dialog->_name);
            free(dialog);
            continue;
        }
        dialog->_audio = Mix_LoadWAV(path);
        if(dialog->_audio == NULL)
        {
            printf("\033[1;31mSoundEngine LoadDialog Error : (path:%s) %s\n\033[0m", path, Mix_GetError());
            free(path);
            free(dialog->_name);
            free(dialog);
            continue;
        }
        free(path);
        dialog->_next = NULL;
        addToStorageList(&soundStorage, dialog);
    }
    closedir(dir);
}

void loadAmbients(const char *DirName)
{
    DIR *dir = opendir(DirName);
    struct dirent *ent;
    if (dir == NULL)
        return;
    while ((ent = readdir(dir)) != NULL)
    {
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        char *path = calloc(strlen(DirName) + strlen(ent->d_name) + 1, sizeof(char));
        sprintf(path, "%s/%s", DirName, ent->d_name);
        StorageElement *ambient = calloc(1, sizeof(StorageElement));
        ambient->_type = SE_AMBIENT;
        ambient->_name = calloc(strlen(ent->d_name) + 1, sizeof(char));
        strcpy(ambient->_name, ent->d_name);
        if (strstr(ent->d_name, ".wav") == NULL)
        {
            free(path);
            free(ambient->_name);
            free(ambient);
            continue;
        }
        ambient->_audio = Mix_LoadWAV(path);
        if(ambient->_audio == NULL)
        {
            printf("\033[1;31mSoundEngine LoadAmbient Error : (path:%s) %s\n\033[0m", path, Mix_GetError());
            free(path);
            free(ambient->_name);
            free(ambient);
            continue;
        }
        free(path);
        ambient->_next = NULL;
        addToStorageList(&soundStorage, ambient);
    }
    closedir(dir);
}

SoundEngine *initSoundEngine(float *musicWeightControl)
{
    if (musicWeight != NULL)
        return NULL;

    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MID | MIX_INIT_OPUS);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_AllocateChannels(MAX_CHANNEL_ALLOCATED);

    json_object *json = json_object_from_file("assets/sound/config.json");
    json_object *fxPath = json_object_object_get(json, "effects");
    json_object *dialogPath = json_object_object_get(json, "dialogs");
    json_object *ambientPath = json_object_object_get(json, "ambients");
    json_object *musicPath = json_object_object_get(json, "musics");

    loadSFX(json_object_get_string(fxPath));

    loadDialogs(json_object_get_string(dialogPath));

    loadAmbients(json_object_get_string(ambientPath));

    int nbMusic = json_object_array_length(musicPath);
    for (int i = 0; i < nbMusic; i++)
    {
        json_object *music = json_object_array_get_idx(musicPath, i);
        json_object *musicName = json_object_object_get(music, "name");
        json_object *musicPath = json_object_object_get(music, "path");
        char *path = calloc(strlen(json_object_get_string(musicPath)) + 4, sizeof(char));
        strcpy(path, "./");
        strcat(path, json_object_get_string(musicPath));
        strcat(path, "/");
        loadMusicTrack(path, json_object_get_string(musicName));
        free(path);
    }
    printf("%d music loaded\n", musicStorage._qty);
    printf("%d SFX loaded\n", soundStorage._qty);
    printf("%d dialogs loaded\n", dialogStorage._qty);
    printf("%d ambients loaded\n", ambientStorage._qty);

    printf("music name : '%s'\n", musicStorage._begin->_name);

    SoundEngine *engine = malloc(sizeof(SoundEngine));

    engine->getTrack = getTrack;
    engine->getEffect = getEffect;
    engine->getDialog = getDialog;
    engine->getAmbientTrack = getAmbientTrack;

    engine->playTrack = playTrack;
    engine->playTrackEffect = playTrackEffect;
    engine->playEffect = playEffect;
    engine->playDialog = playDialog;
    engine->playAmbientTrack = playAmbientTrack;

    engine->stopTracks = stopTracks;
    engine->stopEffect = stopEffect;

    musicWeight = musicWeightControl;

    return engine;
}

void destroySoundEngine(SoundEngine *engine)
{
    deleteMusicStorageList(&musicStorage);
    deleteStorageList(&soundStorage);
    deleteStorageList(&ambientStorage);
    deleteStorageList(&dialogStorage);
    deleteRequestedList(&requested);
    deletePlayingList(&playing);
    musicWeight = NULL;
    free(engine);
    Mix_Quit();
}