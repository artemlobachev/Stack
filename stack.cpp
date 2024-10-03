#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <malloc.h>
#include <math.h>

#include "stack.h"
#include "StackError.h"
#include "StackFunc.h"
#include "UseFulPrints.h"

const int POISON = 0xB00B5;
const int STDCAPACITY = sizeof(Stack_t) * 64;
const int ALLOCATION_COEF = 2;
const int DECREASE_ALLOCATION_COEF = 4; 

ErrorCode StackCtor(Stack *Stack, size_t capacity)
{    
    STACK_ASSERT(Stack);
    
    Stack->capacity = (capacity > 0) ? capacity : STDCAPACITY;   //log

    Stack->StackElements = (Stack_t *) calloc(Stack->capacity, sizeof(Stack_t));

    if (Stack->StackElements == nullptr)
    {
        PRINT_ERROR(Stack, STACK_ELEMENTS_ALLOCATION_ERROR);
        return STACK_ELEMENTS_ALLOCATION_ERROR;
    }

    return ERROR_NOT_FOUND;
}

ErrorCode StackPush(Stack *Stack, Stack_t element)
{   
    STACK_ASSERT(Stack);

    if (Stack->size >= Stack->capacity)
    {
        int ReallocError = StackRealloc(Stack, increase);
        if (ReallocError)
        {
            PRINT_ERROR(Stack, ReallocError);
            return ReallocError;
        }
    }

    STACK_ASSERT(Stack);

    Stack->StackElements[Stack->size] = element;
    Stack->size++;

    return ERROR_NOT_FOUND;
}

ErrorCode StackDump(Stack *Stack, const char *FileName, const char *FromFunc, const int LineCall)
{
    assert(Stack && FileName && FromFunc && LineCall > 0);

    PRINT_STARS;

    ON_DEBUG(puts("\t\tStack Dump");)

    printf("%s:  in function: %s line: %d\n", FileName, FromFunc, LineCall);

    ON_DEBUG
    (
    printf("Stack adress: %p\n", Stack->StackElements);
    printf("Stack size: %lu\n", Stack->size);
    printf("Stack capacity: %lu\n", Stack->capacity);
    )

    PRINT_STARS;
}

ErrorCode StackPop(Stack *Stack)
{   
    STACK_ASSERT(Stack);

    if (Stack->size == 0)
    {
        PRINT_ERROR(Stack, POP_EMPTY_STACK);    
        return POP_EMPTY_STACK;
    }

    if (Stack->size <= Stack->capacity / DECREASE_ALLOCATION_COEF)
    {
        int ReallocError = StackRealloc(Stack, decrease);
        if (ReallocError)
        {
            PRINT_ERROR(Stack, ReallocError);
            return ReallocError;
        }
    }

    return ERROR_NOT_FOUND;
}

ErrorCode StackDtor(Stack *Stack)
{
    assert(Stack);
    
    for (int i = 0; i < Stack->size; i++)
        Stack->StackElements[Stack->size] = POISON;

    free(Stack->StackElements);
    Stack = {0};
    Stack = nullptr;

    return ERROR_NOT_FOUND;
}

static ErrorCode StackRealloc(Stack *Stack, size_t AllocatedMemory, int mode)
{
    assert(Stack);
  
    Stack_t *ReallocSafePtr = (Stack_t *) realloc(Stack, AllocatedMemory);
    while (ReallocSafePtr == nullptr)
    {
        AllocatedMemory /= ALLOCATION_COEF;
        if (AllocatedMemory == 0)
          return REALLOC_ERROR;
      
        ReallocSafePtr = (Stack_t *) realloc(Stack, AllocatedMemory);
    }
    
    FillPoison(Stack, AllocatedMemory / sizeof(Stack_t), mode);
    Stack->StackElements = ReallocSafePtr;

    return ERROR_NOT_FOUND;
}

static void FillPoison(Stack *Stack, size_t ElementsAfterAlloc, int mode)
{
    size_t ElementsBeforeAlloc = Stack->capacity;
    
    if (decrease)
    {
        for (int i = )

    }     
}

void PrintError(ON_DEBUG(Stack *Stack,) ErrorCode error, const char *FileName, const char *FuncName, const int LineCall)
{
    switch(error)
    {
        case STACK_NULL_ADRESS:
            COLOR_PRINTF(RED, "Stack in null address!");
            StackDump(Stack, FileName, FuncName, LineCall);
            abort();

        case STACK_ELEMENTS_ALLOCATION_ERROR:
            COLOR_PRINTF(RED, "Memory for elements in stack wasn`t allocated\n");
            StackDump(Stack, FileName, FuncName, LineCall);
            abort();

        case CAPACITY_BELOW_ZERO:
            COLOR_PRINTF(ORANGE, "Capacity is below zero! ");
            COLOR_PRINTF(ORANGE, "For this situtation program allocated standart capacity");
            StackDump(Stack, FileName, FuncName, LineCall);
            abort();

        case POP_EMPTY_STACK:
            COLOR_PRINTF(ORANGE, "Pops element, but stack is empty(There is nothing to pop)");
            StackDump(Stack, FileName, FuncName, LineCall);
            abort();

        case REALLOC_ERROR:
            COLOR_PRINTF(RED, "Reallocation not work, because nothing to reallocate. ");
            COLOR_PRINTF(RED, "Mo free memory in heap");
            StackDump(Stack, FileName, FuncName, LineCall);
            abort();

        case STACK_ELEMENT_NULL_ADDRESS:
            COLOR_PRINTF(RED, "Element in stack in null address!");
            StackDump(Stack, FileName, FuncName, LineCall); 
            abort();
        
        case CAPACITY_TOO_BIG:
            COLOR_PRINTF(RED, "");
            StackDump(Stack, FileName, FuncName, LineCall);
            abort();

        default:
            ON_DEBUG(COLOR_PRINTF(GREEN, "Error not found!");)
    };
}

ErrorCode StackVerif(Stack *Stack)
{
    if (Stack == nullptr)
        return STACK_NULL_ADRESS;

    if (Stack->StackElements)
        return STACK_ELEMENT_NULL_ADDRESS;

    if (Stack->capacity < 0)
        return CAPACITY_BELOW_ZERO;

    if (Stack->size > Stack->capacity)
        return SIZE_MORE_CAPACITY;
    
    if (Stack->capacity == MAX_CAPACITY && Stack->size == MAX_CAPACITY)
        return STACK_OVERFLOW;

    return ERROR_NOT_FOUND;
}