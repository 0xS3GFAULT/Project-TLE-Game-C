#include <stdlib.h>
#include "listManager.h"

#ifndef NULL
#define NULL (void *)0
#endif

StorageList *newStorageList()
{
    StorageList *list = malloc(sizeof(StorageList));
    list->_begin = NULL;
    list->_qty = 0;
    return list;
}

MusicStorageList *newMusicStorageList()
{
    MusicStorageList *list = malloc(sizeof(MusicStorageList));
    list->_begin = NULL;
    list->_qty = 0;
    return list;
}

RequestedList *newRequestedList()
{
    RequestedList *list = malloc(sizeof(RequestedList));
    list->_begin = NULL;
    list->_qty = 0;
    return list;
}

QueuedList *newQueuedList()
{
    QueuedList *list = malloc(sizeof(QueuedList));
    list->_begin = NULL;
    list->_qty = 0;
    return list;
}

PlayingList *newPlayingList()
{
    PlayingList *list = malloc(sizeof(PlayingList));
    list->_begin = NULL;
    list->_qty = 0;
    return list;
}

void deleteStorageList(StorageList *list)
{
    StorageElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        StorageElement *Next = Temp->_next;
        free(Temp->_name);
        free(Temp);
        Temp = Next;
    }
    list->_begin = NULL;
    list->_qty = 0;
}

void deleteMusicStorageList(MusicStorageList *list)
{
    MusicStorageElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        MusicStorageElement *Next = Temp->_next;
        for (int a = 0; a < Temp->_qty; a++)
        {
            Mix_FreeChunk(Temp->_music[a]._audio);
            free(Temp->_music[a]._name);
        }
        free(Temp->_name);
        free(Temp);
        Temp = Next;
    }
    list->_begin = NULL;
    list->_qty = 0;
}

void deleteRequestedList(RequestedList *list)
{
    RequestedElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        RequestedElement *Next = Temp->_next;
        free(Temp);
        Temp = Next;
    }
    list->_begin = NULL;
    list->_qty = 0;
}

void deleteQueuedList(QueuedList *list)
{
    QueuedElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        QueuedElement *Next = Temp->_next;
        free(Temp);
        Temp = Next;
    }
    list->_begin = NULL;
    list->_qty = 0;
}

void deletePlayingList(PlayingList *list)
{
    PlayingElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        PlayingElement *Next = Temp->_next;
        free(Temp);
        Temp = Next;
    }
    list->_begin = NULL;
    list->_qty = 0;
}

void addToStorageList(StorageList *list, StorageElement *element)
{
    element->_next = list->_begin;
    list->_begin = element;
    list->_qty++;
}

void addToMusicStorageList(MusicStorageList *list, MusicStorageElement *element)
{
    element->_next = list->_begin;
    list->_begin = element;
    list->_qty++;
}

void addToRequestedList(RequestedList *list, RequestedElement *element)
{
    element->_next = list->_begin;
    list->_begin = element;
    list->_qty++;
}

void addToQueuedList(QueuedList *list, QueuedElement *element)
{
    element->_next = list->_begin;
    list->_begin = element;
    list->_qty++;
}

void addToPlayingList(PlayingList *list, PlayingElement *element)
{
    element->_next = list->_begin;
    list->_begin = element;
    list->_qty++;
}

StorageElement *getFromStorageList(StorageList *list, const char *name)
{
    StorageElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        if (!strcmp(Temp->_name, name))
        {
            return Temp;
        }
        Temp = Temp->_next;
    }
    return NULL;
}

MusicStorageElement *getFromMusicStorageList(MusicStorageList *list, const char *name)
{
    MusicStorageElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        if (!strcmp(Temp->_name, name))
        {
            return Temp;
        }
        Temp = Temp->_next;
    }
    return NULL;
}

RequestedElement *getFromRequestList(RequestedList *list, const RequestedElement *element)
{
    RequestedElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        if (Temp == element)
        {
            return Temp;
        }
        Temp = Temp->_next;
    }
    return NULL;
}

PlayingElement *getFromPlayingList(PlayingList *list, int channel)
{
    PlayingElement *Temp = list->_begin;
    while (Temp != NULL)
    {
        if (Temp->_channel == channel)
        {
            return Temp;
        }
        Temp = Temp->_next;
    }
    return NULL;
}

void removeFromStorageList(StorageList *list, StorageElement *element)
{
    if (list->_begin == element)
    {
        list->_begin = element->_next;
        Mix_FreeChunk(element->_audio);
        free(element->_name);
        free(element);
        list->_qty--;
        return;
    }
    StorageElement *Temp = list->_begin->_next, *Prec = list->_begin;
    while (Temp != NULL)
    {
        if (Temp == element)
        {
            Prec->_next = Temp->_next;
            Mix_FreeChunk(Temp->_audio);
            free(Temp->_name);
            free(Temp);
            list->_qty--;
            return;
        }
        Prec = Temp;
        Temp = Temp->_next;
    }
}

void removeFromMusicStorageList(MusicStorageList *list, MusicStorageElement *element)
{
    if (list->_begin == element)
    {
        list->_begin = element->_next;
        for (int a = 0; a < element->_qty; a++)
        {
            Mix_FreeChunk(element->_music[a]._audio);
            free(element->_music[a]._name);
        }
        free(element->_name);
        free(element);
        list->_qty--;
        return;
    }
    MusicStorageElement *Temp = list->_begin->_next, *Prec = list->_begin;
    while (Temp != NULL)
    {
        if (Temp == element)
        {
            Prec->_next = Temp->_next;
            for (int a = 0; a < Temp->_qty; a++)
            {
                Mix_FreeChunk(Temp->_music[a]._audio);
                free(Temp->_music[a]._name);
            }
            free(Temp->_name);
            free(Temp);
            list->_qty--;
            return;
        }
        Prec = Temp;
        Temp = Temp->_next;
    }
}

void removeFromRequestedList(RequestedList *list, RequestedElement *element)
{
    if (list->_begin == element)
    {
        list->_begin = element->_next;
        free(element);
        list->_qty--;
        return;
    }
    RequestedElement *Temp = list->_begin->_next, *Prec = list->_begin;
    while (Temp != NULL)
    {
        if (Temp == element)
        {
            Prec->_next = Temp->_next;
            free(Temp);
            list->_qty--;
            return;
        }
        Prec = Temp;
        Temp = Temp->_next;
    }
}

void removeFromQueuedList(QueuedList *list, QueuedElement *element)
{
    if (list->_begin == element)
    {
        list->_begin = element->_next;
        free(element);
        list->_qty--;
        return;
    }
    QueuedElement *Temp = list->_begin->_next, *Prec = list->_begin;
    while (Temp != NULL)
    {
        if (Temp == element)
        {
            Prec->_next = Temp->_next;
            free(Temp);
            list->_qty--;
            return;
        }
        Prec = Temp;
        Temp = Temp->_next;
    }
}

void removeFromPlayingList(PlayingList *list, PlayingElement *element)
{
    if (list->_begin == element)
    {
        list->_begin = element->_next;
        free(element);
        list->_qty--;
        return;
    }
    PlayingElement *Temp = list->_begin->_next, *Prec = list->_begin;
    while (Temp != NULL)
    {
        if (Temp == element)
        {
            Prec->_next = Temp->_next;
            free(Temp);
            list->_qty--;
            return;
        }
        Prec = Temp;
        Temp = Temp->_next;
    }
}