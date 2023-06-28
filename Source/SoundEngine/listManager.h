#pragma once

#include "elements.h"
#include "lists.h"

StorageList *newStorageList();
MusicStorageList *newMusicStorageList();
RequestedList *newRequestedList();
QueuedList *newQueuedList();
PlayingList *newPlayingList();

void deleteStorageList(StorageList *list);
void deleteMusicStorageList(MusicStorageList *list);
void deleteRequestedList(RequestedList *list);
void deleteQueuedList(QueuedList *list);
void deletePlayingList(PlayingList *list);

void addToStorageList(StorageList *list, StorageElement *element);
void addToMusicStorageList(MusicStorageList *list, MusicStorageElement *element);
void addToRequestedList(RequestedList *list, RequestedElement *element);
void addToQueuedList(QueuedList *list, QueuedElement *element);
void addToPlayingList(PlayingList *list, PlayingElement *element);

StorageElement *getFromStorageList(StorageList *list, const char *name);
MusicStorageElement *getFromMusicStorageList(MusicStorageList *list, const char *name);
RequestedElement *getFromRequestList(RequestedList *list, const RequestedElement *element);
PlayingElement *getFromPlayingList(PlayingList *list, int channel);

void removeFromStorageList(StorageList *list, StorageElement *element);
void removeFromMusicStorageList(MusicStorageList *list, MusicStorageElement *element);
void removeFromRequestedList(RequestedList *list, RequestedElement *element);
void removeFromQueuedList(QueuedList *list, QueuedElement *element);
void removeFromPlayingList(PlayingList *list, PlayingElement *element);