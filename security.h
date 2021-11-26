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

void FileLog(const char* format, ...);

int HashFunc(void* inf, int size);

void HashClear(List* list);

void HashCalc(List* list);

void Dump(const List* list_ptr, int line, const char* func, const char* file);

void ListExists(List* list_ptr);

void AssertFunction(List* list, int line, const char* func, const char* file);

bool CheckHash(List* list);

bool CheckCanaries(List* list);

bool CheckSizes(List* list);