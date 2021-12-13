#include "list.h"

#include <stdio.h>
#include <stdlib.h>

int main()
{
    List* list = CreateList("List");
    int value = 0;
    for (int i = 0; i < 20; i++)
    {
        PushBack(list, i);
    }
    Insert(list, 5, 500);
    RemoveElem(list, 2);
    RemoveElem(list, 8);
    PushBack(list, 400);
    PushBack(list, 300);
    PopFront(list, &value);
    PushBack(list, 200);
    FileGraf(list);
    Linearization(list);
    int size = list->data->size;
    for (int i = 0; i < size; i++)
    {
        PopBack(list, &value);
        printf("%d\n", value);
    }
    CleanList(list);

    return 0;
}
