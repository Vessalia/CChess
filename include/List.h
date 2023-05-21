#pragma once
#include "core.h"

typedef struct Node
{
    void* item;
    struct Node* next;
} Node;

typedef struct List
{
    Node* top;
    unsigned int count;
} List;

List* newList();
void deleteList(List* list);
void insert(List* list, void* item);
void* getItem(List* list, int index);
bool listRemove(List* list, int index);
bool contains(List* list, void* item);
