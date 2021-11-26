#include "list.h"
#include "security.h"
#include <stdio.h>
#include <cassert>
#include <math.h>
#include <errno.h>
#include <stdarg.h>

//Canary's value
#define CANARY 0xFF

//Hash coefficient
#define HASH 0.618033

void FileLog(const char* format, ...)
{
    static FILE* log_file = fopen("list_log.txt", "w");
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
}


int HashFunc(void* inf, int size)
{
    char* point = (char*)inf;
    int result = 0;
    for (int i = 0; i < size; i++)
    {
        result += (int)((size)*fmod(*(point++), HASH));
    }
    return result;
}

void HashClear(List* list)
{
    list->hash = 0;
    list->data->hash = 0;
    list->hash_prev = 0;
    list->hash_next = 0;
    list->data->hash_data = 0;
}

void HashCalc(List* list)
{
    list->hash = HashFunc(list, sizeof(*list));
    list->data->hash = HashFunc(list->data, sizeof(*list->data));
    list->hash_prev = HashFunc(list->prev, list->data->capacity * sizeof(list->prev[0]));
    list->hash_next = HashFunc(list->next, list->data->capacity * sizeof(list->next[0]));
    list->data->hash_data = HashFunc(list->data->data, list->data->capacity * sizeof(list->data->data[0]));
}


bool CheckHash(List* list)
{
    int prev_h_list = list->hash;
    int prev_h_dat = list->data->hash;
    int prev_h_datp = list->hash_prev;
    int prev_h_datn = list->hash_next;
    int prev_h_arr = list->data->hash_data;

    list->hash = 0;
    list->data->hash = 0;
    list->hash_prev = 0;
    list->hash_next = 0;
    list->data->hash_data = 0;

    int a = HashFunc(list->data->data, list->data->capacity * sizeof(list->data->data[0]));

    bool ret = HashFunc(list, sizeof(*list)) == prev_h_list && HashFunc(list->data, sizeof(*(list->data))) == prev_h_dat &&
        HashFunc(list->prev, list->data->capacity * sizeof(list->prev[0])) == prev_h_datp &&
        HashFunc(list->next, list->data->capacity * sizeof(list->next[0])) == prev_h_datn && 
        HashFunc(list->data->data, list->data->capacity * sizeof(list->data->data[0])) == prev_h_arr;

    list->hash = prev_h_list;
    list->data->hash = prev_h_dat;
    list->hash_prev = prev_h_datp;
    list->hash_next = prev_h_datn;
    list->data->hash_data = prev_h_arr;
    return ret;
}

bool CheckCanaries(List* list)
{
    return ((list->can_l == CANARY) && (list->can_r == CANARY) &&
        (list->data->can_l == CANARY) && (list->data->can_r == CANARY) &&
        (*list->can_n_l == CANARY) && (*list->can_n_r == CANARY) &&
        (*list->can_p_l == CANARY) && (*list->can_p_r == CANARY) &&
        (*list->data->can_d_l == CANARY) && (*list->data->can_d_r == CANARY));
}

bool CheckSizes(List* list)
{
    return ((list->data->size <= list->data->capacity) && (list->data->size >= 0) && (list->data->capacity >= 0));
}


void Dump(const List* list_ptr, int line, const char* func, const char* file)
{
    if (list_ptr == NULL)
    {
        return;
    }
    FileLog("List<>[%p] \"%s\" in the \"%d\" line of the function \"%s\" in programm at \"%s\"\n", list_ptr, list_ptr->name, line, func, file);
    FileLog("Size = %d\nCapacity = %d\n", list_ptr->data->size, list_ptr->data->capacity);
    FileLog("data[%p]\n\t{\n", &list_ptr->data->data);
    for (int i = 0; i < list_ptr->data->capacity; i++)
    {
        if (i < list_ptr->data->size)
        {
            FileLog("\t\t[%d] %d\n", i, list_ptr->data->data[i]);
        }
        else
        {
            FileLog("\t\t[%d] (undefined value)\n", i);
        }
    }
    FileLog("\t}\n");
}

void ListExists(List* list_ptr)
{
    if (!list_ptr || !list_ptr->data || !list_ptr->data->mem_data ||
        !list_ptr->mem_next || !list_ptr->mem_prev)
    {
        FileLog("\n\n\t\tBAD LIST\n\n\n");
        errno = ErrorCodes::BADLIST;
    }
}


void AssertFunction(List* list, int line, const char* func, const char* file)
{
    ListExists(list);
    if (!errno)
    {
        if (!CheckCanaries(list)) {
            FileLog("\n\n\t\tCANARY DIED\n\n\n");
            Dump(list, line, func, file);
            errno = ErrorCodes::CANARYDIED;
        }
        if (!CheckSizes(list)) {
            FileLog("\n\n\t\tBAD SIZE\n\n\n");
            Dump(list, line, func, file);
            errno = ErrorCodes::BADSIZE;
        }
        if (!CheckHash(list))
        {
            FileLog("\n\n\t\tHASH ERROR\n\n\n");
            Dump(list, line, func, file);
            errno = ErrorCodes::BADHASH;
        }
    }
}