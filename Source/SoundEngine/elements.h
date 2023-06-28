#pragma once
#include <SDL2/SDL_mixer.h>
#include "SE_types.h"

typedef struct StorageElement StorageElement;
typedef struct MusicStorageElement MusicStorageElement;

typedef struct RequestedElement RequestedElement;
typedef struct QueuedElement QueuedElement;
typedef struct PlayingElement PlayingElement;

struct StorageElement
{
    struct StorageElement *_next; // next element in the list
    Mix_Chunk *_audio;            // audio data
    char *_name;                  // name of the audio
    int _duration;                // duration of the audio
    SE_type _type;                // type of the audio
};

struct MusicStorageElement
{
    struct MusicStorageElement *_next; // next element in the list
    int _qty;                          // quantity of music stored in the arrays
    struct StorageElement *_music;     // array of music elements
    float *_weights;                   // array of weights for the music
    char *_name;                       // name of the music
};

struct RequestedElement
{
    RequestedElement *_next;    // next element in the list
    MusicStorageElement *_data; // data of the element
};

struct QueuedElement
{
    QueuedElement *_next;    // next element in the list
    RequestedElement *_data; // data of the element
};

struct PlayingElement
{
    PlayingElement *_next; // next element in the list
    int _channel;          // channel of the element
    StorageElement *_data; // data of the element
};
