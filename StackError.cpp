#include <stdio.h>
#include <assert.h>

#include "stack.h"
#include "StackError.h"
#include "UsefulPrints.h"

void PrintError(ON_DEBUG(Stack *SomeStack,) int ErrorCode)
{
    switch(ErrorCode)
    {
        case STACK_NULL_ADRESS:
            COLOR_PRINTF(RED, "Stack in null address!");
            ON_DEBUG(STACK_DUMP(SomeStack);)
            break;

        case STACK_ELEMENTS_ALLOCATION_ERROR:
            COLOR_PRINTF(RED, "Memory for elements in stack wasn`t allocated\n");
            ON_DEBUG(STACK_DUMP(SomeStack);)
            break;

        case CAPACITY_BELOW_ZERO:
            COLOR_PRINTF(ORANGE, "Capacity is below zero! ");
            COLOR_PRINTF(ORANGE, "For this situtation program allocated standart capacity");
            ON_DEBUG(STACK_DUMP(SomeStack);)
            break;

        case POP_EMPTY_STACK:
            COLOR_PRINTF(ORANGE, "Pops element, but stack is empty(There is nothing to pop)");
            ON_DEBUG(STACK_DUMP(SomeStack);)
            break;

        case REALLOC_ERROR:
            COLOR_PRINTF(RED, "Reallocation not work, because nothing to reallocate. ");
            COLOR_PRINTF(RED, "Mo free memory in heap");
            ON_DEBUG(STACK_DUMP(SomeStack);)
            break;
    }
}