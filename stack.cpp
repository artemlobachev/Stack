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
    STACK_ASSERT();

    SomeStack->capacity = (capacity > 0) ? capacity : STDCAPACITY;

    SomeStack->StackElements = (Stack_t *) calloc(SomeStack->capacity, sizeof(Stack_t));

    if (SomeStack->StackElements == nullptr)
        return STACK_ELEMENTS_ALLOCATION_ERROR;

    return ERROR_NOT_FOUND;
}

ErrorCode StackPush(Stack *SomeStack, Stack_t element)
{   
    assert(SomeStack);

    if (SomeStack->size >= SomeStack->capacity)
        STACK_ASSERT(StackRealloc(SomeStack, increase));

    SomeStack->StackElements[SomeStack->size] = element;
    SomeStack->size++;

    return ERROR_NOT_FOUND;
}

ErrorCode StackDump(Stack *SomeStack, ErrorCode error, const char *FileName, const char *FromFunc, const int LineCall)
{
    assert(SomeStack && FileName && FromFunc && LineCall > 0);

    PRINT_STARS;

    puts("\t\tStack Dump");
    printf("%s:  in function: %s line: %d\n", FileName, FromFunc, LineCall);
    printf("Stack adress: %p\n", SomeStack->StackElements);
    printf("Stack size: %lu\n", SomeStack->size);
    printf("Stack capacity: %lu\n", SomeStack->capacity);

    PRINT_STARS;
}

ErrorCode StackPop(Stack *SomeStack)
{   
    if (SomeStack->size == 0)
        return POP_EMPTY_STACK;

    if (SomeStack->size <= SomeStack->capacity / 4)
        STACK_ASSERT(StackRealloc(SomeStack, decrease));

    return ERROR_NOT_FOUND;
}

ErrorCode StackDtor(Stack *SomeStack)
{
    for (int i = 0; i < SomeStack->size; i++)
        SomeStack->StackElements[SomeStack->size] = POISON;

    free(SomeStack->StackElements);
    SomeStack = {0};
    SomeStack = nullptr;

    return ERROR_NOT_FOUND;
}