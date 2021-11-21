#include "list.h"

#include <stdio.h>
#include <stdlib.h>


int main()
{
    List* list = CreateList();
    PushBack(list, 10);
    PushBack(list, 15);
    printf("%d\n",PopFront(list));
    printf("%d\n", PopBack(list));
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
    Insert(list, 1, 12);
    RemoveElem(list, 1);
    printf("%d\n", PopBack(list));
    printf("%d\n", PopBack(list));
    CleanList(list);
    return 0;
}
