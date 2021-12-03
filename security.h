#pragma once
#include "list.h"

/// <summary>
/// Error codes
/// </summary>
enum ErrorCodes
{
	//list_ptr pointer has a NULL pointer or not valid fields
	BADLIST = 1,

	//canary data was corrupted
	CANARYDIED,

	//size of data has negative length or 0 length at the pop command
	BADSIZE,

	//different hash of something data
	BADHASH,

	//error memory allocation
	MEMORYERROR
};


/// <summary>
/// Report about list_ptr actions
/// </summary>
/// <param name="format">Format writing data</param>
/// <param name="">Writing values</param>
void FileLog(const char* format, ...);


/// <summary>
/// Count hash of data
/// </summary>
/// <param name="inf">Pointer to data</param>
/// <param name="size">Size of one data element</param>
/// <returns>Unsigned int value of hash</returns>
int HashFunc(void* inf, int size);


/// <summary>
/// Nullify hash of data
/// </summary>
/// <param name="list_ptr">Pointer on List data</param>
void HashClear(List* list_ptr);


/// <summary>
/// Calculate hasg for all data of List
/// </summary>
/// <param name="list_ptr">Pointer on List data</param>
void HashCalc(List* list_ptr);


/// <summary>
/// Check correct hash value of list pointer and array
/// </summary>
/// <param name="list_ptr">Pointer on List data</param>
/// <returns>1 if hash value is correct, else 0</returns>
bool CheckHash(List* list_ptr);


/// <summary>
/// Check canaries' values
/// </summary>
/// <param name="list_ptr">Transmitted stack_ptr</param>
/// <returns>1 if nothing has been changed, else 0</returns>
bool CheckCanaries(List* list_ptr);


/// <summary>
/// Check list_ptr sizes corectness
/// </summary>
/// <param name="list_ptr">Transmitted list_ptr</param>
/// <returns>1 if all sizes are right, else 0</returns>
bool CheckSizes(List* list_ptr);


/// <summary>
/// Print information about list_ptr: size, capacity, element and errors, if they exist
/// </summary>
/// <param name="list_ptr">Trnsmitted list_ptr</param>
/// <param name="errtxt">Description of error</param>
/// <param name="line">Error occurence line</param>
/// <param name="func">Error occurence function</param>
/// <param name="file">Error occurence file</param>
void Dump(const List* list_ptr, int line, const char* func, const char* file);

/// <summary>
/// Detect valid or not pointer to the List
/// </summary>
/// <param name="list_ptr">Pointer to the List</param>
void ListExists(List* list_ptr);


/// <summary>
/// Find errors on transmitted List and write error code to the errno
/// </summary>
/// <param name="list_ptr">Transmitted stack_ptr</param>
/// <param name="line">Error occurence line</param>
/// <param name="func">Error occurence function</param>
/// <param name="file">Error occurence file</param>
void AssertFunction(List* list_ptr, int line, const char* func, const char* file);
