#pragma once

//Size of list name
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


/// <summary>
/// Create and initializate new List
/// </summary>
/// <param name="name">Transmitted name of List</param>
/// <returns>Pointer to the initialized List</returns>
List* CreateList(const char name[]);


/// <summary>
/// Push transmitted element at the end of transmitted List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">The value to put on the List</param>
/// <returns>Return 0, if all actions were succesfull, else return error code</returns>
int PushBack(List* list_ptr, int value);


/// <summary>
/// Push transmitted element at the begining of transmitted List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">The value to put on the List</param>
/// <returns>Return 0, if all actions were succesfull, else return error code</returns>
int PushFront(List* list_ptr, int value);


/// <summary>
/// Push transmitted element at the transmitted place of transmitted List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="position">Place at the List for insert the element</param>
/// <param name="value">The value to put on the List</param>
/// <returns>Return 0, if all actions were succesfull, else return error code</returns>
int Insert(List* list_ptr, int position, int value);


/// <summary>
/// Reduce the List size by decrementation of element's size from the end
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">Transmitted pointer to the variable</param>
/// <returns>Return 0, if all actions were succesfull, else return error code</returns>
int PopBack(List* list_ptr, int* value);


/// <summary>
/// Reduce the List size by decrementation of element's size from the begining
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">Transmitted pointer to the variable</param>
/// <returns>Return 0, if all actions were succesfull, else return error code</returns>
int PopFront(List* list_ptr, int* value);


/// <summary>
/// Delete element from the List in transmitted position
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="n">Place at the List for delete element</param>
/// <returns>Return 0, if all actions were succesfull, else return error code</returns>
int RemoveElem(List* list_ptr, int n);


/// <summary>
/// Increase list_ptr capacity of the List or do nothing, if list_ptr is empty
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <returns>Return 0, if all actions were succesfull, else return error code</returns>
int Resize(List* list_ptr);


/// <summary>
/// Initialization List's name by transmitted name
/// </summary>
/// <param name="target_name">Char array get transmitted name of List</param>
/// <param name="get_name">Transmitted name</param>
void NameInititialization(char target_name[], const char get_name[]);


/// <summary>
/// Initial pointers to data array, canaries and sizes 
/// </summary>
/// <param name="stack_ptr">Transmitted List</param>
void DataInitialization(List* list_ptr);


/// <summary>
/// Delete List by free memory
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
void CleanList(List* list_ptr);