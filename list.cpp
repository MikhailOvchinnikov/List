#define LIST 10

//Coefficient of resize stack's capacity
#define RESIZE_COEF 2

//Canary's value
#define CANARY 0xFF

#include "list.h"
#include "security.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct List
{
    int can_l;

    Data* data;

    int* can_n_l;
    int* mem_next;
    int* next;
    int* can_n_r;

    int* can_p_l;
    int* mem_prev;
    int* prev;
    int* can_p_r;

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
    int can_r;
} Data;


List* CreateList()
{
    List* list = (List*)malloc(sizeof(List));
    list->data = (Data*)malloc(sizeof(Data));
    list->data->mem_data = (int*)calloc(LIST + 2, sizeof(int));
    list->mem_next = (int*)calloc(LIST + 2, sizeof(int));
    list->mem_prev = (int*)calloc(LIST + 2, sizeof(int));

    list->can_n_l = &(list->mem_next[0]);
    list->can_p_l = &(list->mem_prev[0]);
    list->data->can_d_l = &(list->data->mem_data[0]);
    list->can_n_r = &(list->mem_next[LIST + 1]);
    list->can_p_r = &(list->mem_prev[LIST + 1]);
    list->data->can_d_r = &(list->data->mem_data[LIST + 1]);

    list->next = &(list->mem_next[1]);
    list->prev = &(list->mem_prev[1]);
    list->data->data = &(list->data->mem_data[1]);

    *(list->can_n_l) = CANARY;
    *(list->can_p_l) = CANARY;
    *(list->data->can_d_l) = CANARY;
    *(list->can_n_r) = CANARY;
    *(list->can_p_r) = CANARY;
    *(list->data->can_d_r) = CANARY;


    for (int i = 0; i < LIST; i++)
    {
        list->next[i] = -1;
        list->prev[i] = -1;
    }
    list->data->capacity = LIST;
    list->data->size = 0;
    list->data->head = 0;
    list->data->tail = 0;

    list->can_l = CANARY;
    list->can_r = CANARY;
    list->data->can_l = CANARY;
    list->data->can_r = CANARY;

    return list;
}


void PushBack(List* list, int value)
{
    if (list == NULL)
    {
        AssertFunction(list);
    }
    if (list->data->size >= list->data->capacity)
    {
        Resize(list);
    }

    if (list->data->size == 0)
    {
        list->data->data[0] = value;
        list->next[0] = 0;
        list->data->tail = 0;
        list->data->size++;
    }
    else
    {
        for (int i = 0; i < list->data->capacity; i++)
        {
            if (list->next[i] == -1)
            {
                list->data->data[i] = value;
                list->next[i] = list->data->head;

                int temp_next = list->data->tail;
                list->next[temp_next] = i;
                list->prev[i] = temp_next;
                list->data->tail = i;
                break;
            }
        }
        list->data->size++;
    }
}


void PushFront(List* list, int value)
{
    if (list == NULL)
    {
        AssertFunction(list);
    }
    if (list->data->size >= list->data->capacity)
    {
        Resize(list);
    }

    if (list->data->size == 0)
    {
        list->data->data[0] = value;
        list->next[0] = 0;
        list->data->tail = 0;
        list->data->size++;
    }
    else
    {
        for (int i = 0; i < list->data->capacity; i++)
        {
            if (list->next[i] == -1)
            {
                list->data->data[i] = value;

                int* temp_next = &(list->data->head);
                list->next[i] = *temp_next;
                list->next[list->data->tail] = i;
                list->prev[i] = 0;
                list->prev[*temp_next] = i;
                list->data->head = i;
                break;
            }
        }
        list->data->size++;
    }
}


void Insert(List* list, int position, int value)
{
    if (list == NULL)
    {
        AssertFunction(list);
    }
    if (list->data->size >= list->data->capacity)
    {
        Resize(list);
    }

    for (int i = 0; i < list->data->capacity; i++)
    {
        if (list->next[i] == -1)
        {
            int* lpp = &(list->prev[position]);
            list->data->data[i] = value;
            list->next[i] = list->next[*lpp];
            list->next[*lpp] = i;
            list->prev[i] = *lpp;
            *lpp = i;
            break;
        }
    }
    list->data->size++;
}


int PopBack(List* list)
{
    if (list == NULL)
    {
        AssertFunction(list);
    }
    int tail = list->data->tail;
    int value = list->data->data[tail];
    list->data->tail = list->prev[tail];
    list->next[tail] = -1;
    list->next[list->data->tail] = 0;
    list->prev[tail] = -1;
    list->data->size--;
    return value;
}


int PopFront(List* list)
{
    if (list == NULL)
    {
        AssertFunction(list);
    }
    int head = list->data->head;
    int value = list->data->data[head];
    list->data->head = list->next[head];
    list->prev[head] = -1;
    list->prev[list->data->head] = 0;
    list->next[head] = -1;
    list->data->size--;
    return value;
}


void RemoveElem(List* list, int n)
{
    if (list == NULL)
    {
        AssertFunction(list);
    }
    list->next[list->prev[n]] = list->next[n];
    list->prev[list->next[n]] = list->prev[n];
    list->next[n] = -1;
    list->prev[n] = -1;
    list->data->size--;
}


void Resize(List* list)
{
    if (list == NULL)
    {
        AssertFunction(list);
    }
    int old_capacity = list->data->capacity;
    list->data->capacity *= RESIZE_COEF;
    list->data->mem_data = (int*)realloc(list->data->mem_data, (list->data->capacity + 2) * sizeof(int)); 
    list->mem_next = (int*)realloc(list->mem_next, (list->data->capacity + 2) * sizeof(int));
    list->mem_prev = (int*)realloc(list->mem_prev, (list->data->capacity + 2) * sizeof(int));

    list->next = &(list->mem_next[1]);
    list->prev = &(list->mem_prev[1]);
    list->data->data = &(list->data->mem_data[1]);

    for (int i = old_capacity; i < list->data->capacity; i++)
    {
        list->next[i] = -1;
        list->prev[i] = -1;
    }

    *list->data->can_d_l = 0;
    *list->data->can_d_r = 0;
    *list->can_n_l = 0;
    *list->can_n_r = 0;
    *list->can_p_l = 0;
    *list->can_p_r = 0;
    list->data->mem_data[0] = CANARY;
    list->mem_next[0] = CANARY;
    list->mem_prev[0] = CANARY;
    list->data->mem_data[list->data->capacity + 1] = CANARY;
    list->mem_next[list->data->capacity + 1] = CANARY;
    list->mem_prev[list->data->capacity + 1] = CANARY;
    list->data->can_d_l = &(list->data->mem_data[0]);
    list->can_n_l = &(list->mem_next[0]);
    list->can_p_l = &(list->mem_prev[0]);
    list->data->can_d_r = &(list->data->mem_data[list->data->capacity + 1]);
    list->can_n_r = &(list->mem_next[list->data->capacity + 1]);
    list->can_p_r = &(list->mem_prev[list->data->capacity + 1]);
    /*for (int i = 0; i < list->data->capacity + 2; i++)
    {
        printf("%d ", list->data->mem_data[i]);
    }
    printf("\n");
    for (int i = 0; i < list->data->capacity + 2; i++)
    {
        printf("%d ", list->mem_next[i]);
    }
    printf("\n");*/
}


void CleanList(List* list)
{
    free(list->data->mem_data);
    free(list->data);
    free(list->mem_next);
    free(list->mem_prev);
    free(list);

}