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

void HashClear(List* list_ptr)
{
    Data* data_struct = list_ptr->data;

    list_ptr->hash = 0;
    data_struct->hash = 0;
    list_ptr->hash_prev = 0;
    list_ptr->hash_next = 0;
    data_struct->hash_data = 0;
}

void HashCalc(List* list_ptr)
{
    Data* data_struct = list_ptr->data;
    int cap = list_ptr->data->capacity;

    list_ptr->hash = HashFunc(list_ptr, sizeof(*list_ptr));
    data_struct->hash = HashFunc(data_struct, sizeof(*data_struct));
    list_ptr->hash_prev = HashFunc(list_ptr->prev, cap * sizeof(list_ptr->prev[0]));
    list_ptr->hash_next = HashFunc(list_ptr->next, cap * sizeof(list_ptr->next[0]));
    data_struct->hash_data = HashFunc(data_struct->data, cap * sizeof(data_struct->data[0]));
}


bool CheckHash(List* list_ptr)
{
    Data* data_struct = list_ptr->data;
    int cap = list_ptr->data->capacity;

    int prev_h_list = list_ptr->hash;
    int prev_h_dat = data_struct->hash;
    int prev_h_datp = list_ptr->hash_prev;
    int prev_h_datn = list_ptr->hash_next;
    int prev_h_arr = data_struct->hash_data;

    list_ptr->hash = 0;
    data_struct->hash = 0;
    list_ptr->hash_prev = 0;
    list_ptr->hash_next = 0;
    data_struct->hash_data = 0;

    int a = HashFunc(data_struct->data, cap * sizeof(data_struct->data[0]));

    bool ret_1 = HashFunc(list_ptr, sizeof(*list_ptr)) == prev_h_list;
    bool ret_2 = HashFunc(list_ptr->data, sizeof(*(list_ptr->data))) == prev_h_dat;
    bool ret_3 = HashFunc(list_ptr->prev, cap * sizeof(list_ptr->prev[0])) == prev_h_datp;
    bool ret_4 = HashFunc(list_ptr->next, cap * sizeof(list_ptr->next[0])) == prev_h_datn;
    bool ret_5 = HashFunc(data_struct->data, cap * sizeof(data_struct->data[0])) == prev_h_arr;

    list_ptr->hash = prev_h_list;
    data_struct->hash = prev_h_dat;
    list_ptr->hash_prev = prev_h_datp;
    list_ptr->hash_next = prev_h_datn;
    data_struct->hash_data = prev_h_arr;
    return ret_1 && ret_2 && ret_3 && ret_4 && ret_5;
}

bool CheckCanaries(List* list_ptr)
{
    Data* data_struct = list_ptr->data;
    bool ret_1 = list_ptr->can_l == CANARY && list_ptr->can_r == CANARY;
    bool ret_2 = data_struct->can_l == CANARY && data_struct->can_r == CANARY;
    bool ret_3 = *list_ptr->can_n_l == CANARY && *list_ptr->can_n_r == CANARY;
    bool ret_4 = *list_ptr->can_p_l == CANARY && *list_ptr->can_p_r == CANARY;
    bool ret_5 = *data_struct->can_d_l == CANARY && *data_struct->can_d_r == CANARY;

    return ret_1 && ret_2 && ret_3 && ret_4 && ret_5;
}

bool CheckSizes(List* list_ptr)
{
    int cap = list_ptr->data->capacity;
    int size = list_ptr->data->size;
    return ((size <= cap) && (size >= 0) && (cap >= 0));
}


void Dump(const List* list_ptr, int line, const char* func, const char* file)
{
    if (list_ptr == NULL)
    {
        return;
    }

    Data* data_struct = list_ptr->data;
    int cap = list_ptr->data->capacity;
    int size = list_ptr->data->size;

    FileLog("List<>[%p] \"%s\" in the \"%d\" line of the function \"%s\" in programm at \"%s\"\n", list_ptr, list_ptr->name, line, func, file);
    FileLog("Size = %d\nCapacity = %d\n", size, cap);
    FileLog("data[%p]\n\t{\n", &data_struct->data);
    for (int i = 0; i < cap; i++)
    {
        if (i < size)
        {
            FileLog("\t\t[%d] %d\n", i, data_struct->data[i]);
        }
        else
        {
            FileLog("\t\t[%d] (undefined value)\n", i);
        }
    }
    FileLog("\t}\n");
}

int ListExists(void* ptr)
{
    if (!ptr)
    {
        FileLog("\n\n\t\tBAD LIST\n\n\n");
        errno = ErrorCodes::BADLIST;
        return -1;
    }
    return 0;
}


int AssertFunction(List* list_ptr, int line, const char* func, const char* file)
{
    ListExists(list_ptr);
    if (!errno)
    {
        if (!CheckCanaries(list_ptr)) {
            FileLog("\n\n\t\tCANARY DIED\n\n\n");
            Dump(list_ptr, line, func, file);
            errno = ErrorCodes::CANARYDIED;
        }
        if (!CheckSizes(list_ptr)) {
            FileLog("\n\n\t\tBAD SIZE\n\n\n");
            Dump(list_ptr, line, func, file);
            errno = ErrorCodes::BADSIZE;
        }
        if (!CheckHash(list_ptr))
        {
            FileLog("\n\n\t\tHASH ERROR\n\n\n");
            Dump(list_ptr, line, func, file);
            errno = ErrorCodes::BADHASH;
        }
    }

    if (errno)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}