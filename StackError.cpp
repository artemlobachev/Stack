#include <stdio.h>
#include <assert.h>

#include "stack.h"   
#include "StackError.h"
#include "UsefulPrints.h"

void StackAssert(Stack *SomeStack, ErrorCode error, const char *FileName, const char *FuncName, const int LineCall)
{
    if (error)
    {
        PrintError(SomeStack, error, FileName, FuncName, LineCall);
        assert(0 && "tut krch code error");
    }
}

ErrorCode StackVerif(Stack *SomeStack)
{
    if (SomeStack == nullptr)
        return STACK_NULL_ADRESS;

    if (SomeStack->StackElements)
        return STACK_ELEMENT_NULL_ADDRESS;

    if (SomeStack->capacity < 0)
        return CAPACITY_BELOW_ZERO;

    if (SomeStack->size > SomeStack->capacity)
        return SIZE_MORE_CAPACITY;

    return ERROR_NOT_FOUND;
}

void PrintError(ON_DEBUG(Stack *SomeStack,) ErrorCode error, const char *FileName, const char *FuncName, const int LineCall)
{
    switch(error)
    {
        case STACK_NULL_ADRESS:
            COLOR_PRINTF(RED, "Stack in null address!");
            ON_DEBUG(StackDump(SomeStack, FileName, FuncName, LineCall);)
            break;

        case STACK_ELEMENTS_ALLOCATION_ERROR:
            COLOR_PRINTF(RED, "Memory for elements in stack wasn`t allocated\n");
            ON_DEBUG(StackDump(SomeStack, FileName, FuncName, LineCall);)
            break;

        case CAPACITY_BELOW_ZERO:
            COLOR_PRINTF(ORANGE, "Capacity is below zero! ");
            COLOR_PRINTF(ORANGE, "For this situtation program allocated standart capacity");
            ON_DEBUG(StackDump(SomeStack, FileName, FuncName, LineCall);)
            break;

        case POP_EMPTY_STACK:
            COLOR_PRINTF(ORANGE, "Pops element, but stack is empty(There is nothing to pop)");
            ON_DEBUG(StackDump(SomeStack, FileName, FuncName, LineCall);)
            break;

        case REALLOC_ERROR:
            COLOR_PRINTF(RED, "Reallocation not work, because nothing to reallocate. ");
            COLOR_PRINTF(RED, "Mo free memory in heap");
            ON_DEBUG(StackDump(SomeStack, FileName, FuncName, LineCall);)
            break;

        case STACK_ELEMENT_NULL_ADDRESS:
            COLOR_PRINTF(RED, "Element in stack in null address!");
            ON_DEBUG(StackDump(SomeStack, FileName, FuncName, LineCall);)
            break;

        default:
            ON_DEBUG(COLOR_PRINTF(GREEN, "Error not found!");)
    };
}