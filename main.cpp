#include "list.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    List* list = CreateList("List");
    int value = 0;
    PushBack(list, 10);
    PushBack(list, 15);
    PopFront(list, &value);
    PopBack(list, &value);
    PushFront(list, 1);
    PushBack(list, 2);
    PushFront(list, 3);
    PushBack(list, 4);
    PushFront(list, 5);
    PushBack(list, 6);
    PushFront(list, 7);
    PushBack(list, 8);
    PushFront(list, 9);
    PushBack(list, 10);
    PushFront(list, 11);
    PushBack(list, 12);
    Insert(list, 1, 15);
    RemoveElem(list, 1);
    PopBack(list, &value);
    PopBack(list, &value);
    CleanList(list);

    return 0;
}
