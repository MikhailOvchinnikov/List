
#include "list.h"
#include "security.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LIST 10

//Coefficient of resize list's capacity
#define RESIZE_COEF 2

//Canary's value
#define CANARY 0xFF

#define NotNULL(list_ptr) \
ListExists(list_ptr)

//Def function for check list adequate
#define ListOK(list, line, func, file) { \
AssertFunction(list, line, func, file);	 \
}


List* CreateList(const char name[])
{
    List* list = (List*)calloc(1, sizeof(List));
    list->data = (Data*)calloc(1, sizeof(Data));
    list->data->mem_data = (int*)calloc(LIST + 2, sizeof(int));
    list->mem_next = (int*)calloc(LIST + 2, sizeof(int));
    list->mem_prev = (int*)calloc(LIST + 2, sizeof(int));

    NotNULL(list);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return nullptr;
    }

    list->can_n_l = &(list->mem_next[0]);
    list->can_p_l = &(list->mem_prev[0]);
    list->data->can_d_l = &(list->data->mem_data[0]);
    list->can_n_r = &(list->mem_next[LIST + 1]);
    list->can_p_r = &(list->mem_prev[LIST + 1]);
    list->data->can_d_r = &(list->data->mem_data[LIST + 1]);

    list->next = &(list->mem_next[1]);
    list->prev = &(list->mem_prev[1]);
    list->data->data = &(list->data->mem_data[1]);

    list->data->mem_data[0] = CANARY;
    list->mem_next[0] = CANARY;
    list->mem_prev[0] = CANARY;

    list->data->mem_data[LIST + 1] = CANARY;
    list->mem_next[LIST + 1] = CANARY;
    list->mem_prev[LIST + 1] = CANARY;


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
    NameInititialization(list->name, name);
    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return nullptr;
    }
    else
    {
        FileLog("List \"%s\" was created successfully\n", list->name);
        return list;
    }
}


int PushBack(List* list, int value)
{
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }
    if (list->data->size >= list->data->capacity)
    {
        if (Resize(list))
        {
            FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
            return errno;
        }
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
    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
    }
    else
    {
        FileLog("Command %s of the list \"%s\" was successful, value %d\n", __FUNCTION__, list->name, value);
    }
    return errno;
}


int PushFront(List* list, int value)
{
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }
    if (list->data->size >= list->data->capacity)
    {
        if (Resize(list))
        {
            return errno;
        }
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
    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
    }
    else
    {
        FileLog("Command %s of the list \"%s\" was successful, value %d\n", __FUNCTION__, list->name, value);
    }
    return errno;
}


int Insert(List* list, int position, int value)
{
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }
    if (list->data->size >= list->data->capacity)
    {
        if (Resize(list))
        {
            FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
            return errno;
        }
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
    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
    }
    else
    {
        FileLog("Command %s of the list \"%s\" was successful, value %d, position %d\n", __FUNCTION__, list->name, value, position);
    }
    return errno;
}


int PopBack(List* list, int* value)
{
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }
    int tail = list->data->tail;
    *value = list->data->data[tail];
    list->data->tail = list->prev[tail];
    list->next[tail] = -1;
    list->next[list->data->tail] = 0;
    list->prev[tail] = -1;
    list->data->size--;
    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
    }
    else
    {
        FileLog("Command %s of the list \"%s\" was successful, value %d\n", __FUNCTION__, list->name, *value);
    }
    return errno;
}


int PopFront(List* list, int* value)
{
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }
    int head = list->data->head;
    *value = list->data->data[head];
    list->data->head = list->next[head];
    list->prev[head] = -1;
    list->prev[list->data->head] = 0;
    list->next[head] = -1;
    list->data->size--;
    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
    }
    else
    {
        FileLog("Command %s of the list \"%s\" was successful, value %d\n", __FUNCTION__, list->name, *value);
    }
    return errno;
}


int RemoveElem(List* list, int n)
{
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }
    int value = list->data->data[n];
    list->next[list->prev[n]] = list->next[n];
    list->prev[list->next[n]] = list->prev[n];
    list->next[n] = -1;
    list->prev[n] = -1;
    list->data->size--;
    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
    }
    else
    {
        FileLog("Command %s of the list \"%s\" was successful, value %d, position %d\n", __FUNCTION__, list->name, value, n);
    }
    return errno;
}

int Resize(List* list)
{
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);

    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }

    int old_capacity = list->data->capacity;
    list->data->capacity *= RESIZE_COEF;
    list->data->mem_data = (int*)realloc(list->data->mem_data, (list->data->capacity + 2) * sizeof(int)); 
    list->mem_next = (int*)realloc(list->mem_next, (list->data->capacity + 2) * sizeof(int));
    list->mem_prev = (int*)realloc(list->mem_prev, (list->data->capacity + 2) * sizeof(int));

    NotNULL(list);

    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
        return errno;
    }

    list->next = &(list->mem_next[1]);
    list->prev = &(list->mem_prev[1]);
    list->data->data = &(list->data->mem_data[1]);

    for (int i = old_capacity; i < list->data->capacity; i++)
    {
        list->next[i] = -1;
        list->prev[i] = -1;
    }


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

    HashClear(list);
    HashCalc(list);
    ListOK(list, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list \"%s\"\n", __FUNCTION__, list->name);
    }
    else
    {
        FileLog("Command %s of the list \"%s\" was successful, value %d\n", __FUNCTION__, list->name, list->data->capacity);
    }
    return errno;
}


void NameInititialization(char target_name[], const char get_name[])
{
    size_t i = 0;
    for (i = 0; i < (strnlen(get_name, 31)); i++)
    {
        target_name[i] = get_name[i];
    }
    target_name[i] = '\0';
}


void CleanList(List* list)
{
    FileLog("List \"%s\" is deleting...\n", list->name);

    free(list->data->mem_data);
    free(list->data);
    free(list->mem_next);
    free(list->mem_prev);
    free(list);

    FileLog("Deleting was successful");
}