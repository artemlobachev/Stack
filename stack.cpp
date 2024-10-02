#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <malloc.h>
#include <math.h>

#include "stack.h"
#include "StackError.h"
#include "StackFunc.h"
#include "UseFulPrints.h"

ErrorCode StackCtor(Stack *SomeStack, size_t capacity)
{    
    SomeStack->capacity = (capacity > 0) ? capacity : STDCAPACITY;

    SomeStack->StackElements = (Stack_t *) calloc(SomeStack->capacity, sizeof(Stack_t));
    
    if (SomeStack->StackElements == nullptr)
    {
        PRINT_ERROR(SomeStack, STACK_ELEMENTS_ALLOCATION_ERROR);
        return STACK_ELEMENTS_ALLOCATION_ERROR;
    }

    return ERROR_NOT_FOUND;
}

ErrorCode StackPush(Stack *SomeStack, Stack_t element)
{   
    ON_DEBUG(STACK_ASSERT(SomeStack);)

    if (SomeStack->size >= SomeStack->capacity)
    {
        int ReallocError = StackRealloc(SomeStack, increase);
        if (ReallocError)
        {
            PRINT_ERROR(SomeStack, ReallocError);
            return ReallocError;
        }
    }

    if (SomeStack->StackElements == nullptr)
    {
        PRINT_ERROR(SomeStack, STACK_ELEMENT_NULL_ADDRESS);
        return STACK_ELEMENT_NULL_ADDRESS;
    }

    SomeStack->StackElements[SomeStack->size] = element;
    SomeStack->size++;

    return ERROR_NOT_FOUND;
}

ErrorCode StackDump(Stack *SomeStack, const char *FileName, const char *FromFunc, const int LineCall)
{
    assert(SomeStack && FileName && FromFunc && LineCall > 0);

    PRINT_STARS;

    ON_DEBUG(puts("\t\tStack Dump");)

    printf("%s:  in function: %s line: %d\n", FileName, FromFunc, LineCall);

    ON_DEBUG
    (
    printf("Stack adress: %p\n", SomeStack->StackElements);
    printf("Stack size: %lu\n", SomeStack->size);
    printf("Stack capacity: %lu\n", SomeStack->capacity);
    )

    PRINT_STARS;
}

ErrorCode StackPop(Stack *SomeStack)
{   
    assert(SomeStack);

    if (SomeStack->size == 0)
        return POP_EMPTY_STACK;

    if (SomeStack->size <= SomeStack->capacity / DECREASE_ALLOCATION_COEF)
    {
        int ReallocError = StackRealloc(SomeStack, decrease);
        if (ReallocError)
        {
            PRINT_ERROR(SomeStack, ReallocError);
            return ReallocError;
        }
    }

    return ERROR_NOT_FOUND;
}

ErrorCode StackDtor(Stack *SomeStack)
{
    assert(SomeStack);
    
    for (int i = 0; i < SomeStack->size; i++)
        SomeStack->StackElements[SomeStack->size] = POISON;

    free(SomeStack->StackElements);
    SomeStack = {0};
    SomeStack = nullptr;

    return ERROR_NOT_FOUND;
}