
#include "list.h"
#include "security.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LIST 10

//Coefficient of resize list_ptr's capacity
#define RESIZE_COEF 2

//Canary's value
#define CANARY 0xFF

#define NotNULL(list_ptr) \
ListExists(list_ptr)

//Def function for check list_ptr adequate
#define ListOK(list_ptr, line, func, file) { \
AssertFunction(list_ptr, line, func, file);	 \
}


List* CreateList(const char name[])
{
    List* list_ptr = (List*)calloc(1, sizeof(List));
    list_ptr->data = (Data*)calloc(1, sizeof(Data));
    Data* data_struct = list_ptr->data;

    data_struct->mem_data = (int*)calloc(LIST + 2, sizeof(int));
    list_ptr->mem_next = (int*)calloc(LIST + 2, sizeof(int));
    list_ptr->mem_prev = (int*)calloc(LIST + 2, sizeof(int));

    NotNULL(list_ptr);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return nullptr;
    }

    data_struct->capacity = LIST;
    data_struct->size = 0;
    data_struct->head = 0;
    data_struct->tail = 0;

    data_struct->can_l = CANARY;
    data_struct->can_r = CANARY;
    list_ptr->can_l = CANARY;
    list_ptr->can_r = CANARY;

    DataInitialization(list_ptr);

    for (int i = 0; i < LIST; i++)
    {
        list_ptr->next[i] = -1;
        list_ptr->prev[i] = -1;
    }

    NameInititialization(list_ptr->name, name);
    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return nullptr;
    }
    else
    {
        FileLog("List \"%s\" was created successfully\n", list_ptr->name);
        return list_ptr;
    }
}


int PushBack(List* list_ptr, int value)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }

    Data* data_struct = list_ptr->data;
    int cap = data_struct->capacity;
    int* size = &data_struct->size;

    if (*size >= cap)
    {
        if (Resize(list_ptr))
        {
            FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
            return errno;
        }
    }

    if (list_ptr->data->size == 0)
    {
        data_struct->data[0] = value;
        list_ptr->next[0] = 0;
        data_struct->tail = 0;
        data_struct->size++;
    }
    else
    {
        for (int i = 0; i < cap; i++)
        {
            if (list_ptr->next[i] == -1)
            {
                data_struct->data[i] = value;
                list_ptr->next[i] = data_struct->head;

                int temp_next = data_struct->tail;
                list_ptr->next[temp_next] = i;
                list_ptr->prev[i] = temp_next;
                data_struct->tail = i;
                break;
            }
        }
        ++*size;
    }
    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, value);
    }
    return errno;
}


int PushFront(List* list_ptr, int value)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }

    Data* data_struct = list_ptr->data;
    int cap = data_struct->capacity;
    int *size = &data_struct->size;

    if (*size >= cap)
    {
        if (Resize(list_ptr))
        {
            return errno;
        }
    }

    if (size == 0)
    {
        data_struct->data[0] = value;
        list_ptr->next[0] = 0;
        data_struct->tail = 0;
        data_struct->size++;
    }
    else
    {
        for (int i = 0; i < cap; i++)
        {
            if (list_ptr->next[i] == -1)
            {
                data_struct->data[i] = value;

                int* temp_next = &(data_struct->head);
                list_ptr->next[i] = *temp_next;
                list_ptr->next[data_struct->tail] = i;
                list_ptr->prev[i] = 0;
                list_ptr->prev[*temp_next] = i;
                data_struct->head = i;
                break;
            }
        }
        ++*size;
    }
    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, value);
    }
    return errno;
}


int Insert(List* list_ptr, int position, int value)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }
    if (list_ptr->data->size >= list_ptr->data->capacity)
    {
        if (Resize(list_ptr))
        {
            FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
            return errno;
        }
    }
    Data* data_struct = list_ptr->data;

    for (int i = 0; i < data_struct->capacity; i++)
    {
        if (list_ptr->next[i] == -1)
        {
            int* lpp = &(list_ptr->prev[position]);
            data_struct->data[i] = value;
            list_ptr->next[i] = list_ptr->next[*lpp];
            list_ptr->next[*lpp] = i;
            list_ptr->prev[i] = *lpp;
            *lpp = i;
            break;
        }
    }
    data_struct->size++;
    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d, position %d\n", __FUNCTION__, list_ptr->name, value, position);
    }
    return errno;
}


int PopBack(List* list_ptr, int* value)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }

    Data* data_struct = list_ptr->data;
    int tail = data_struct->tail;
    *value = data_struct->data[tail];

    data_struct->tail = list_ptr->prev[tail];
    list_ptr->next[tail] = -1;
    list_ptr->next[data_struct->tail] = 0;
    list_ptr->prev[tail] = -1;
    data_struct->size--;

    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, *value);
    }
    return errno;
}


int PopFront(List* list_ptr, int* value)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }
    int head = list_ptr->data->head;
    *value = list_ptr->data->data[head];
    list_ptr->data->head = list_ptr->next[head];
    list_ptr->prev[head] = -1;
    list_ptr->prev[list_ptr->data->head] = 0;
    list_ptr->next[head] = -1;
    list_ptr->data->size--;
    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, *value);
    }
    return errno;
}


int RemoveElem(List* list_ptr, int n)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }
    int value = list_ptr->data->data[n];
    list_ptr->next[list_ptr->prev[n]] = list_ptr->next[n];
    list_ptr->prev[list_ptr->next[n]] = list_ptr->prev[n];
    list_ptr->next[n] = -1;
    list_ptr->prev[n] = -1;
    list_ptr->data->size--;
    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d, position %d\n", __FUNCTION__, list_ptr->name, value, n);
    }
    return errno;
}

int Resize(List* list_ptr)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);

    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }

    Data* data_struct = list_ptr->data;

    int old_capacity = data_struct->capacity;
    data_struct->capacity *= RESIZE_COEF;
    int cap = data_struct->capacity;

    data_struct->mem_data = (int*)realloc(data_struct->mem_data, (cap + 2) * sizeof(int)); 
    list_ptr->mem_next = (int*)realloc(list_ptr->mem_next, (cap + 2) * sizeof(int));
    list_ptr->mem_prev = (int*)realloc(list_ptr->mem_prev, (cap + 2) * sizeof(int));

    NotNULL(list_ptr);

    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return errno;
    }

    DataInitialization(list_ptr);

    for (int i = old_capacity; i < cap; i++)
    {
        list_ptr->next[i] = -1;
        list_ptr->prev[i] = -1;
    }

    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, cap);
    }
    return errno;
}


void NameInititialization(char target_name[], const char get_name[])
{
    strncpy(target_name, get_name, ST_S);
}


void DataInitialization(List* list_ptr)
{
    Data* data_struct = list_ptr->data;
    int* ptr_data = data_struct->mem_data;
    int cap = data_struct->capacity;


    ptr_data[0] = CANARY;
    list_ptr->mem_next[0] = CANARY;
    list_ptr->mem_prev[0] = CANARY;

    ptr_data[cap + 1] = CANARY;
    list_ptr->mem_next[cap + 1] = CANARY;
    list_ptr->mem_prev[cap + 1] = CANARY;

    list_ptr->can_n_l = &list_ptr->mem_next[0];
    list_ptr->can_p_l = &list_ptr->mem_prev[0];
    list_ptr->can_n_r = &list_ptr->mem_next[cap + 1];
    list_ptr->can_p_r = &list_ptr->mem_prev[cap + 1];
    data_struct->can_d_r = &ptr_data[cap + 1];
    data_struct->can_d_l = &ptr_data[0];

    list_ptr->next = &list_ptr->mem_next[1];
    list_ptr->prev = &list_ptr->mem_prev[1];
    data_struct->data = &ptr_data[1];
}


void CleanList(List* list_ptr)
{
    FileLog("List \"%s\" is deleting...\n", list_ptr->name);

    free(list_ptr->data->mem_data);
    free(list_ptr->data);
    free(list_ptr->mem_next);
    free(list_ptr->mem_prev);
    free(list_ptr);

    FileLog("Deleting was successful");
}