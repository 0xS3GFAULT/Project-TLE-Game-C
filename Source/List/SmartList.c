/**
 * @file SmartList.c
 * @brief Functions for the management of lists
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SmartList.h"

int COMPARE_PTR(void *data1, void *data2){
    return data1 == data2 ? 0 : 1;
}

static void PRINT_POINTER_DEBUG(void *self){
    printf("\033[1;31mSmartList_DEBUG : ITEM DATA %p\n\033[0m", self);
}

static chainItem *newChainItem(void *data){
    chainItem *newItem = malloc(sizeof(chainItem));
    newItem->data = data;
    newItem->previous = NULL;
    newItem->next = NULL;
    return newItem;
}

list *newList(compareTwoPointersFunction comparatorFunction){
    list *theNewList = malloc(sizeof(list));
    theNewList->first = NULL;
    theNewList->last = NULL;
    theNewList->length = 0;
    theNewList->comparator = comparatorFunction;
    theNewList->memLength = 2;
    theNewList->indexMap = malloc(sizeof(chainItem *)*2);
    theNewList->indexMap[0] = NULL;
    return theNewList;
}

void forEach(list *list, void (*function)(void *data)){
    chainItem *item = list->first;
    while(item != NULL){
        chainItem *next = item->next;
        function(item->data);
        item = next;
    }
}

static void updateIndexMap(list *list, int indexOfChange){
    if(indexOfChange < 0) indexOfChange = 0;
    if(list->length > list->memLength){
        list->memLength *= 2;
        list->indexMap = realloc(list->indexMap, list->memLength*sizeof(chainItem *));
    }
    for(int i = indexOfChange; i < list->length; i++){
        if(i == 0) list->indexMap[i] = list->first;
        else list->indexMap[i] = list->indexMap[i-1]->next;
    }
}

static chainItem *getItemAtIndex(list list, int index){
    if(index < 0 || index > list.length - 1) return NULL;
    return list.indexMap[index];
}

void *getDataAtIndex(list list, int index){
    if(index < 0)
        printf("\033[1;31mSmartList_ERROR : Get Data at negative index\n\033[0m");
    chainItem *item = getItemAtIndex(list, index);
    if(item) return getItemAtIndex(list, index)->data;
    else return NULL;
}

/**
 * @brief Put an item at specified index and update the list architecture to support it
 * 
 * @param[in] list 
 * @param[in] item 
 * @param[in] index 
 */
void putItemAtIndex(list *list, chainItem *item, int index){
    if(index < 0)
        printf("\033[1;31mSmartList_ERROR : Put Item at negative index\n\033[0m");
    if(item == NULL || list->length < index) return;
    item->previous = getItemAtIndex(*list, index-1);
    item->next = getItemAtIndex(*list, index);

    if(item->previous != NULL)
        item->previous->next = item;
    if(item->next != NULL)
        item->next->previous = item;

    if(index == 0) list->first = item;
    if(index == list->length-1) list->last = item;
    list->length++;
    updateIndexMap(list, index-1);
}

void deleteItemAtIndex(list *list, int index){
    if(index < 0)
        printf("\033[1;31mSmartList_ERROR : Delete Item at negative index (%d)\n\033[0m", index);
    if(list->length < index) return;
    chainItem *item = getItemAtIndex(*list, index);
    if(item->previous)
        item->previous->next = item->next;
    if(item->next)
        item->next->previous = item->previous;
    if(index == 0) list->first = item->next;
    if(index == list->length-1) list->last = item->previous;
    list->length--;
    free(item);
    updateIndexMap(list, index);
}

void pushInList(list *list, void *data){
    chainItem *newItem = newChainItem(data);
    putItemAtIndex(list, newItem, 0);
}

void appendInList(list *list, void *data){
    chainItem *newItem = newChainItem(data);
    putItemAtIndex(list, newItem, list->length);
}

void appendAtInList(list *list, void *data, int index){
    chainItem *newItem = newChainItem(data);
    putItemAtIndex(list, newItem, index);
}

static chainItem *searchItemInList(list list, void *data){
    chainItem *item = list.first; 
    while(item != NULL){
        if(list.comparator(item->data, data) == 0) return item;
        item = item->next;
    }
    return NULL;
}

int searchIndexInList(list list, void *data){
    chainItem *item = list.first;
    int index = 0;
    while(item != NULL){
        if(list.comparator(item->data, data) == 0) return index;
        item = item->next;
        index++;
    }
    return -404;
}

void *searchDataInList(list list, void *data){
    chainItem *result = searchItemInList(list, data);
    if(result == NULL) return NULL;
    return result->data;
}

void deleteInList(list *list, void *data){
    int index = searchIndexInList(*list, data);
    if(index != -404) deleteItemAtIndex(list, index);
    else printf("\033[1;31mSmartList_ERROR : Item to Delete not found (%p)\n\033[0m", data);
}

void sortList(list *listToSort, compareTwoPointersFunction comparatorFunction, bool ascending){
    if(!comparatorFunction) comparatorFunction = listToSort->comparator;
    list *sortedList = newList(listToSort->comparator);
    chainItem *item = listToSort->first;
    while(item != NULL){
        chainItem *sortItem = sortedList->first;
        int indexFound = 0;
        while(sortItem != NULL){
            if(comparatorFunction(sortItem->data, item->data) > 0 && ascending || comparatorFunction(sortItem->data, item->data) < 0 && !ascending){
                break;
            }
            indexFound++;
            sortItem = sortItem->next;
        }
        chainItem *newSortedItem = newChainItem(item->data);
        putItemAtIndex(sortedList, newSortedItem, indexFound);
        item = item->next;
    }
    updateIndexMap(sortedList, 0);
    emptyList(listToSort);
    *listToSort = *sortedList;
    free(sortedList);
}

void emptyList(list *list){
    while(list->length > 0) deleteItemAtIndex(list, 0);
}

void freeList(list *list){
    emptyList(list);
    free(list->indexMap);
    free(list);
}