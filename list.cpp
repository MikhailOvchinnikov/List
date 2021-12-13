//stack.h frmo Stack progect
#include "..\..\Stack_dinamic\Stack_dinamic\stack.h"

#include "list.h"
#include "security.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//Initial size of list
#define INITIAL_SIZE 10

//Coefficient of resize list_ptr's capacity
#define RESIZE_COEF 2

//Canary's value
#define CANARY 0xFF

#define NotNULL(list_ptr) ListExists(list_ptr)

//Def function for check list_ptr adequate
#define ListOK(list_ptr, line, func, file) AssertFunction(list_ptr, line, func, file)


List* CreateList(const char name[])
{
    List* list_ptr = (List*)calloc(1, sizeof(List));

    if (NotNULL(list_ptr))
    {
        CleanList(list_ptr);
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return nullptr;
    }

    list_ptr->data = (Data*)calloc(1, sizeof(Data));

    if (NotNULL(list_ptr->data))
    {
        CleanList(list_ptr);
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return nullptr;
    }

    Data* data_ptr = list_ptr->data;

    data_ptr->mem_data = (LST*)calloc(sizeof(LST) * INITIAL_SIZE + 2 * sizeof(int), sizeof(char));


    if (NotNULL(data_ptr->mem_data))
    {
        CleanList(list_ptr);
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return nullptr;
    }

    data_ptr->capacity = INITIAL_SIZE;
    data_ptr->size = 0;
    data_ptr->head = 0;
    data_ptr->tail = 0;

    data_ptr->can_l = CANARY;
    data_ptr->can_r = CANARY;
    list_ptr->can_l = CANARY;
    list_ptr->can_r = CANARY;

    DataInitialization(list_ptr);

    for (int i = 0; i < INITIAL_SIZE; i++)
    {
        data_ptr->data[i].next = -1;
        data_ptr->data[i].prev = -1;
    }

    Stack* stack = CreateStack("Free_places");
    list_ptr->free_p = stack;

    for (int i = data_ptr->capacity - 1; i > 0; i--)
    {
        Push(list_ptr->free_p, i);
    }

    NameInititialization(list_ptr->name, name);
    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
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


void FileGraf(List* list_ptr)
{
    FILE* file = fopen("grafic_list.txt", "w");
    fprintf(file, "digraph List {\n");
    fprintf(file, "rankdir=TB;\n");
    Data* data_ptr = list_ptr->data;
    LST* data_struct = data_ptr->data;
    for (int i = 0; i < data_ptr->capacity; i++)
    {
        fprintf(file, "\t\"box %d\" [shape=plaintext, label=<\n<table>\n\t", i); 
        fprintf(file, "<tr><td port='adress'>adress % d</td><td rowspan='2' port='prev'>prev %d</td><td rowspan='2' port='next'>next %d</td></tr>\n\t",
            i, data_struct[i].prev, data_struct[i].next); 
        fprintf(file, "<tr><td port='data'>data %d</td></tr>\n</table>\n\t>]; \n", data_struct[i].value);
        if (i > 0)
        {
            fprintf(file, "\t\"box %d\":next->\"box %d\":prev [color=white];\n", i - 1, i);
        }
    }
    int i = data_ptr->head;
    while(data_struct[i].next != -2)
    {
        fprintf(file, "\t\"box %d\":next->\"box %d\":prev;\n", i, data_struct[i].next);
        i = data_struct[i].next;
    }
    fprintf(file, "}\n");
    fclose(file);
}


int PushBack(List* list_ptr, int value)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    Data* data_ptr = list_ptr->data;

    if (data_ptr->size == data_ptr->capacity)
    {
        if (Resize(list_ptr))
        {
            FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
            return -1;
        }
    }

    LST* data_struct = data_ptr->data;

    if (data_ptr->size == 0)
    {
        data_struct[0].value = value;
        data_struct[0].next = -2;
        data_struct[0].prev = -2;
        data_ptr->tail = 0;
        ++data_ptr->size;
    }
    else
    {
        float ti = 0;
        Pop(list_ptr->free_p, &ti); 
        int i = (int)ti;
        data_struct[i].value = value;
        data_struct[i].next = -2;

        int temp_next = data_ptr->tail;
        data_struct[temp_next].next = i;
        data_struct[i].prev = temp_next;
        data_ptr->tail = i;
        ++data_ptr->size;
    }
    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, value);
        return 0;
    }
}


int PushFront(List* list_ptr, int value)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    Data* data_ptr = list_ptr->data;

    if (data_ptr->size == data_ptr->capacity)
    {
        if (Resize(list_ptr))
        {
            return -1;
        }
    }

    LST* data_struct = data_ptr->data;

    if (data_ptr->size == 0)
    {
        data_struct[0].value = value;
        data_struct[0].next = -2;
        data_struct[0].prev = -2;
        data_ptr->tail = 0;
        ++data_ptr->size;
    }
    else
    {
        float ti = 0;
        Pop(list_ptr->free_p, &ti);
        int i = (int)ti;
        data_struct[i].value = value;

        int temp_next = data_ptr->head;
        data_struct[i].next = temp_next;
        data_struct[i].prev = 0;
        data_struct[temp_next].prev = i;
        data_ptr->head = i;
        ++data_ptr->size;
    }
    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, value);
        return 0;
    }
}


int Insert(List* list_ptr, int position, int value)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    Data* data_ptr = list_ptr->data;

    if (data_ptr->size == data_ptr->capacity)
    {
        if (Resize(list_ptr))
        {
            FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
            return -1;
        }
    }

    LST* data_struct = data_ptr->data;

    float ti = 0;
    Pop(list_ptr->free_p, &ti);
    int i = (int)ti;

    if (position > data_ptr->size)
    {
        position = data_ptr->tail;
    }
    data_struct[i].value = value;
    data_struct[i].next = position;
    data_struct[data_struct[position].prev].next = i;
    data_struct[i].prev = data_struct[position].prev;
    data_struct[position].prev = i; 
    ++data_ptr->size;
    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d, position %d\n", __FUNCTION__, list_ptr->name, value, position);
        return 0;
    }
}


int PopBack(List* list_ptr, int* value)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    Data* data_ptr = list_ptr->data;
    LST* data_struct = data_ptr->data;
    int last_tail = data_ptr->tail;

    *value = data_struct[last_tail].value;

    data_ptr->tail = data_struct[last_tail].prev;
    data_struct[last_tail].next = -1;
    data_struct[last_tail].prev = -1;
    data_ptr->size--;

    Push(list_ptr->free_p, last_tail);
    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, *value);
        return 0;
    }
}


int PopFront(List* list_ptr, int* value)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    Data* data_ptr = list_ptr->data;
    LST* data_struct = data_ptr->data;
    int last_head = data_ptr->head;
    *value = data_struct[last_head].value;

    data_ptr->head = data_struct[last_head].next;
    data_struct[last_head].prev = -1;
    data_struct[last_head].next = -1;
    --data_ptr->size;

    Push(list_ptr->free_p, last_head);
    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, *value);
        return 0;
    }
}


int RemoveElem(List* list_ptr, int n)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    Data* data_ptr = list_ptr->data;
    LST* data_struct = data_ptr->data;

    if (data_struct[n].next == -1)
    {
        return 0;
    }
    int value = data_struct[n].value;

    if (n == data_ptr->head)
    {
        data_ptr->head = data_struct[n].next;
        data_struct[data_ptr->head].prev = -2;
    }
    if (n == data_ptr->tail)
    {
        data_ptr->tail = data_struct[n].prev;
        data_struct[data_ptr->tail].next = -2;
    }
    if (data_struct[n].prev != -2)
    {
        data_struct[data_struct[n].prev].next = data_struct[n].next;
    }
    if (data_struct[n].next != -2)
    {
        data_struct[data_struct[n].next].prev = data_struct[n].prev;
    }
    data_struct[n].next = -1;
    data_struct[n].prev = -1;
    --data_ptr->size;
    float pos = 0;
    Push(list_ptr->free_p, n);
    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d, position %d\n", __FUNCTION__, list_ptr->name, value, n);
        return 0;
    }
}


int Size(List* list_ptr, int* dst)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the stack \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    Data* data_ptr = list_ptr->data;

    *dst = data_ptr->size;

    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the stack \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the stack \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, *dst);
        return 0;
    }
}


int Capacity(List* list_ptr, int* dst)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the stack \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    Data* data_ptr = list_ptr->data;

    *dst = data_ptr->capacity;

    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the stack \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the stack \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, *dst);
        return 0;
    }
}


int Resize(List* list_ptr)
{
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);

    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    Data* data_ptr = list_ptr->data;

    int old_capacity = data_ptr->capacity;
    data_ptr->capacity *= RESIZE_COEF;
    int cap = data_ptr->capacity;

    data_ptr->mem_data = (LST*)realloc(data_ptr->mem_data, sizeof(LST) * cap + 2 * sizeof(int));

    if (NotNULL(data_ptr->mem_data))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    DataInitialization(list_ptr);

    LST* data_struct = data_ptr->data;

    for (int i = old_capacity; i < cap; i++)
    {
        data_struct[i].next = -1;
        data_struct[i].prev = -1;
    }

    for (int i = data_ptr->capacity - 1; i > old_capacity - 1; i--)
    {
        Push(list_ptr->free_p, i);
    }

    HashClear(list_ptr);
    HashCalc(list_ptr);
    ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__);
    if (errno)
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful, value %d\n", __FUNCTION__, list_ptr->name, cap);
        return 0;
    }
}


void NameInititialization(char target_name[], const char get_name[])
{
    strncpy(target_name, get_name, ST_S);
}


void DataInitialization(List* list_ptr)
{
    Data* data_ptr = list_ptr->data;

    int* p_data = (int*)data_ptr->mem_data;
    data_ptr->data = (LST*)(p_data + 1);

    LST* data_struct = data_ptr->data;
    int cap = data_ptr->capacity;

    int* beg_data = (int*)data_ptr->mem_data;
    *beg_data = CANARY;

    int* end_data = (int*)&data_ptr->data[cap];
    *end_data = CANARY;

    data_ptr->can_d_r = end_data;
    data_ptr->can_d_l = beg_data;
}


int Compare(const void* first,const void* second)
{
    LST* n_f = (LST*)first;
    LST* n_s = (LST*)second;
    return (n_f->next - n_s->next);
}


int Linearization(List* list_ptr)
{
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }

    Data* data_ptr = list_ptr->data;
    LST* data_struct = data_ptr->data;

    int pos = 0;
    int n_ind = data_ptr->head;
    int val = 0;
    int next = 0;
    int prev = 0;
    for (int i = 0; i < list_ptr->data->capacity; i++)
    {
        if (pos != n_ind)
        {
            val = data_struct[pos].value;
            next = data_struct[pos].next;
            prev = data_struct[pos].prev;
            if (data_struct[n_ind].next != -1 && data_struct[n_ind].next != -2)
            {
                data_struct[data_struct[n_ind].next].prev = pos;
            }
            if (data_struct[n_ind].prev != -1 && data_struct[n_ind].prev != -2)
            {
                data_struct[data_struct[n_ind].prev].next = pos;
            }
            if (next != -1 && next != -2)
            {
                data_struct[next].prev = n_ind;
            }
            if (prev != -1 && prev != -2)
            {
                data_struct[prev].next = n_ind;
            }
            prev = data_struct[pos].prev;
            data_struct[pos].value = data_struct[n_ind].value;
            data_struct[pos].next = data_struct[n_ind].next;
            data_struct[pos].prev = data_struct[n_ind].prev;
            data_struct[n_ind].value = val;
            data_struct[n_ind].next = next;
            data_struct[n_ind].prev = prev;
        }
        n_ind = list_ptr->data->data[pos].next;
        pos++;
        if (n_ind == -2)
        {
            data_ptr->head = 0;
            data_ptr->tail = pos - 1;
            break;
        }
    }
    CleanDataStk(list_ptr->free_p);
    for (int i = data_ptr->capacity - 1; i > data_ptr->size; i--)
    {
        Push(list_ptr->free_p, i);
    }


    HashClear(list_ptr);
    HashCalc(list_ptr);
    if (ListOK(list_ptr, __LINE__, __FUNCTION__, __FILE__))
    {
        FileLog("Error command %s of the list_ptr \"%s\"\n", __FUNCTION__, list_ptr->name);
        return -1;
    }
    else
    {
        FileLog("Command %s of the list_ptr \"%s\" was successful\n", __FUNCTION__, list_ptr->name);
        return 0;
    }
}


void CleanList(List* list_ptr)
{
    FileLog("List \"%s\" is deleting...\n", list_ptr->name);

    free(list_ptr->data->mem_data);
    free(list_ptr->data);
    free(list_ptr);

    FileLog("Deleting was successful");
}