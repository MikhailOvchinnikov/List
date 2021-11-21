#include "list.h"
#include <stdio.h>
#include <cassert>

void AssertFunction(List* list)
{

    if (list == NULL)
    {
        printf("\n\n\t\tSTACK DOES NOT EXIST\n\n\n");
        assert(0);
    }
}