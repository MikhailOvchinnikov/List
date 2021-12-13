#pragma once

//Size of list name
#define ST_S 32

typedef struct Stack Stack;

typedef struct LSTelem
{
    int value;
    int prev;
    int next;
} LST;

typedef struct Data
{
    int can_l;

    int* can_d_l;
    LST* mem_data;
    LST* data;
    int* can_d_r;

    int head;
    int tail;

    int capacity;
    int size;

    unsigned int hash;
    unsigned int hash_data;

    int can_r;
} Data;

typedef struct List
{
    int can_l;

    char name[ST_S] = {};

    Data* data;

    Stack* free_p;

    unsigned int hash;

    int can_r;
} List;


/// <summary>
/// Set right positions of List elements
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <returns>0 if all operations were right, else -1</returns>
int Linearization(List* list_ptr);


/// <summary>
/// Draw grafic struct of List
/// </summary>
/// <param name="list">Transmitted List</param>
void FileGraf(List* list);

/// <summary>
/// Create and initializate new List
/// </summary>
/// <param name="name">Transmitted name of List</param>
/// <returns>Pointer to the initialized List or -1, if error has occured</returns>
List* CreateList(const char name[]);


/// <summary>
/// Push transmitted element at the end of transmitted List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">The value to put on the List</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
int PushBack(List* list_ptr, int value);


/// <summary>
/// Push transmitted element at the begining of transmitted List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">The value to put on the List</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
int PushFront(List* list_ptr, int value);


/// <summary>
/// Push transmitted element at the transmitted place of transmitted List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="position">Place at the List for insert the element</param>
/// <param name="value">The value to put on the List</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
int Insert(List* list_ptr, int position, int value);


/// <summary>
/// Reduce the List size by decrementation of element's size from the end
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">Transmitted pointer to the variable</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
int PopBack(List* list_ptr, int* value);


/// <summary>
/// Reduce the List size by decrementation of element's size from the begining
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="value">Transmitted pointer to the variable</param>
/// <returns>Return 0, if all actions were succesfull, else return e-1</returns>
int PopFront(List* list_ptr, int* value);


/// <summary>
/// Delete element from the List in transmitted position
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="n">Place at the List for delete element</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
int RemoveElem(List* list_ptr, int n);


/// <summary>
/// Return size of List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="dst">Transmitted pointer to the variable</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
int Size(List* stack, int* dst);

/// <summary>
/// Return capacity of List
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <param name="dst">Transmitted pointer to the variable</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
int Capacity(List* stack, int* dst);


/// <summary>
/// Increase list_ptr capacity of the List or do nothing, if list_ptr is empty
/// </summary>
/// <param name="list_ptr">Transmitted List</param>
/// <returns>Return 0, if all actions were succesfull, else return -1</returns>
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