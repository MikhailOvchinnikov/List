#pragma once

//Size of stack name
#define ST_S 32

typedef struct List List;
typedef struct Data Data;
typedef struct List
{
    int can_l;

    char name[ST_S] = {};

    Data* data;

    int* can_n_l;
    int* mem_next;
    int* next;
    int* can_n_r;

    int* can_p_l;
    int* mem_prev;
    int* prev;
    int* can_p_r;

    unsigned int hash;
    unsigned int hash_prev;
    unsigned int hash_next;

    int can_r;
} List;

typedef struct Data
{
    int can_l;

    int* can_d_l;
    int* mem_data;
    int* data;
    int* can_d_r;

    int head;
    int tail;

    int capacity;
    int size;

    unsigned int hash;
    unsigned int hash_data;

    int can_r;
} Data;

List* CreateList(const char name[]);

int PushBack(List* list, int value);

int PushFront(List* list, int value);

int Insert(List* list, int position, int value);

int PopBack(List* list, int* value);

int PopFront(List* list, int* value);

int RemoveElem(List* list, int n);

int Resize(List* list);

void NameInititialization(char target_name[], const char get_name[]);

void DataInitialization(List* list);

void CleanList(List* list);