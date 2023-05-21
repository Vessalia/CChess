#include "List.h"

Node* newNode(void* item, Node* next)
{
    Node* node = (Node*)malloc(sizeof(Node));
    node->item = item;
    node->next = next;

    return node;
}

void deleteNode(Node* node)
{
    free(node->item);
    free(node);
}

List* newList()
{
    List* list = (List*)malloc(sizeof(List));
    list->top = NULL;
    list->count = 0;

    return list;
}

void deleteList(List* list)
{
    if(!list)
    {
        return;
    }

    Node* curr = list->top;
    while(curr)
    {
        Node* temp = curr;
        curr = curr->next;
        deleteNode(temp);
    }

    free(list);
}

void insert(List* list, void* item)
{
    list->top = newNode(item, list->top);
    list->count++;
}

Node* getNode(List* list, int index)
{
    Node* curr = NULL;
    int currIndex = 0;
    if(index >= 0 && index < (int)list->count)
    {
        curr = list->top;
        while (currIndex < index)
        {
            curr = curr->next;
            currIndex++;
        }
    }

    return curr;
}

void* getItem(List* list, int index)
{
    return getNode(list, index)->item;
}

static const int INVALID_INDEX = -1;
int indexOf(List* list, void* item)
{
    int index = INVALID_INDEX;

    for(unsigned int i = 0; i < list->count && index == INVALID_INDEX; ++i)
    {
        if (item == getItem(list, i))
        {
            index = i;
        }
    }

    return index;
}

bool listRemove(List* list, int index)
{
    if(list->count == 0 || index < 0 || index >= (int)list->count)
    {
        return false;
    }

    Node* prev = getNode(list, index - 1);
    Node* curr = getNode(list, index);

    if(!prev) // must be at top node
    {
        list->top = list->top->next;
    }
    else
    {
        prev->next = curr->next;
    }

    list->count--;

    return true;
}

bool contains(List* list, void* item)
{
    return indexOf(list, item) != INVALID_INDEX;
}
