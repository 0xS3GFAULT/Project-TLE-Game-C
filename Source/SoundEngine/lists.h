#pragma once

typedef struct StorageList StorageList;
typedef struct MusicStorageList MusicStorageList;

typedef struct RequestedList RequestedList;
typedef struct QueuedList QueuedList;
typedef struct PlayingList PlayingList;

struct StorageList
{
    struct StorageElement *_begin;
    int _qty;
};

struct MusicStorageList
{
    struct MusicStorageElement *_begin;
    int _qty;
};

struct RequestedList
{
    struct RequestedElement *_begin;
    int _qty;
};

struct QueuedList
{
    struct QueuedElement *_begin;
    int _qty;
};

struct PlayingList
{
    struct PlayingElement *_begin;
    int _qty;
};