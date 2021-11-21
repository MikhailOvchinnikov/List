#pragma once

typedef struct List List;
typedef struct Data Data;

List* CreateList();

void PushBack(List* list, int value);

void PushFront(List* list, int value);

void Insert(List* list, int position, int value);

int PopBack(List* list);

int PopFront(List* list);

void RemoveElem(List* list, int n);

void Resize(List* list);

void CleanList(List* list);