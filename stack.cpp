#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <math.h>

#include "stack.h"
#include "CanaryProtection.h"
#include "UseFulPrints.h"
#include "HashProtection.h"
#include "StackError.h"

/*        CONSTS              */
const int POISON = 0xB00B5;
const size_t STDCAPACITY = 64;
const int ALLOCATION_COEF = 2;
const int MAX_ALLOCATION_COEF = 4; 
const size_t MAX_CAPACITY = 1431655769 / sizeof(Stack_t);
const size_t MAX_PRINT_ELEMENTS = 50;
const size_t STDOUT_ELEMENTS = 50;
const size_t MEMORY_FOR_CANARIES = sizeof(Stack_t) * 2;

/*--------------------------------------------------------------------------------------------------------------------------------------------*/

/*          STATIC FUNCTIONS            */
static size_t GetReallocMemory(Stack *stk, int mode)
{
    if (mode == increase)
        return (stk->capacity * sizeof(Stack_t)) * ALLOCATION_COEF ON_CANARY(+ MEMORY_FOR_CANARIES); 

    if (mode == decrease && stk->size != 0)
    {
        while(stk->capacity > stk->size * MAX_ALLOCATION_COEF)
            stk->capacity /= ALLOCATION_COEF;
        
        return stk->capacity * sizeof(Stack_t) ON_CANARY(+ 2 * sizeof(Stack_t));
    }

    return 0;
}

static ErrorCode StackRealloc(Stack *stk, size_t AllocatedMemory)
{
    STACK_ASSERT(stk);
    CHECK_STACK(stk);
    ON_CANARY(CHECK_CANARY(stk);)

/*For stack data safety. Realloc can return nullptr*/
    Stack_t *ReallocSafePtr = (Stack_t *) realloc(stk->StackElements ON_CANARY(- 1), AllocatedMemory) ON_CANARY(+ 1);

/*If AllocatedMemory too big and system can`t allocate so much memory*/
    while (ReallocSafePtr == nullptr)
    {
        AllocatedMemory /= ALLOCATION_COEF;
        if (AllocatedMemory == 0) 
          return REALLOC_ERROR;
      
        ReallocSafePtr = (Stack_t *) realloc(stk, AllocatedMemory);
    }
  
    stk->StackElements = ReallocSafePtr;
    stk->capacity = (AllocatedMemory ON_CANARY(- MEMORY_FOR_CANARIES)) / sizeof(Stack_t);

    CHECK_STACK(stk);

    return ERROR_NOT_FOUND;
}

static size_t RequiredCapacity(size_t capacity)
{   
    if ((int64_t) capacity <= 0)
        return STDCAPACITY;
    
    if (capacity > MAX_CAPACITY)
        return STDCAPACITY;

    return capacity;

}

/*--------------------------------------------------------------------------------------------------------------------------------------------*/


/*                Stack constructor and destructor                       */
ErrorCode StackCtor(Stack *stk, size_t capacity)
{    
    assert(stk);

    ON_CANARY 
    (
    
    stk->BOTTOM_CANARY = CANARY;
    stk->TOP_CANARY = CANARY; 
    
    ) 

    stk->capacity = RequiredCapacity(capacity);
    stk->size = 0;
    stk->StackElements = (Stack_t *) calloc(stk->capacity ON_CANARY(+2), sizeof(Stack_t)) ON_CANARY(+1);

    if (stk->StackElements == nullptr)
    {
        PRINT_ERROR(stk, STACK_ELEMENTS_ALLOCATION_ERROR);
        return STACK_ELEMENTS_ALLOCATION_ERROR;
    }
    
    ON_CANARY 
    (
    
    SetDataCanary(stk->StackElements, stk->capacity);
    CHECK_CANARY(stk);
    
    )
 
    ON_HASH
    (

    GetDataHash(stk, MurMurHash3);
    GetStackHash(stk, MurMurHash3);
    
    )
 
    CHECK_STACK(stk);

    return ERROR_NOT_FOUND;
}

ErrorCode StackDtor(Stack *stk)
{
    CHECK_STACK(stk);
    ON_CANARY(CHECK_CANARY(stk);)

    
    for (int i = 0; i < stk->size; i++)
        stk->StackElements[stk->size] = POISON;
    
    free(stk->StackElements ON_CANARY(-1));
    stk->StackElements = nullptr;
    stk->capacity = POISON;
    stk->size = POISON;

    ON_HASH
    (
    
    stk->DataHash = POISON;
    stk->StackHash = POISON;
    
    )

    stk = nullptr;

    return ERROR_NOT_FOUND;
}

/*----------------------------------------------------------------------------------------------------------------------------------------*/


/*             Stack functions:                    */
ErrorCode StackPush(Stack *stk, Stack_t element)
{   
    STACK_ASSERT(stk);
    ON_CANARY(CHECK_CANARY(stk);)
 
    if (stk->size >= stk->capacity)
    {
        size_t AllocatedMemory = GetReallocMemory(stk, increase);
        int ReallocError = StackRealloc(stk, AllocatedMemory);
        if (ReallocError)
        {
            PRINT_ERROR(stk, ReallocError);
            return ReallocError;
        }

        ON_CANARY
        (

        SetDataCanary(stk->StackElements, stk->capacity);
        CHECK_CANARY(stk);
        
        )
    }
    CHECK_STACK(stk);

    stk->StackElements[stk->size] = element;
    stk->size++;

    ON_HASH
    (

    GetDataHash(stk, MurMurHash3);
    GetStackHash(stk, MurMurHash3);

    )

    return ERROR_NOT_FOUND;
}

ErrorCode StackPop(Stack *stk)
{   
    STACK_ASSERT(stk);

    if (stk->size == 0)
    {
        PRINT_ERROR(stk, POP_EMPTY_STACK);    
        return POP_EMPTY_STACK;
    }

    if (stk->size <= stk->capacity / MAX_ALLOCATION_COEF)
    {
        size_t AllocatedMemory = GetReallocMemory(stk, decrease);
        int ReallocError = StackRealloc(stk, AllocatedMemory ON_CANARY(+ 2 * sizeof(Stack_t)));
        if (ReallocError)
        {
            PRINT_ERROR(stk, ReallocError);
            return ReallocError;
        }
        ON_CANARY
        (

        SetDataCanary(stk->StackElements, stk->capacity);
        CHECK_CANARY(stk);
        
        )
    }

    stk->size--;
    stk->StackElements[stk->size] = POISON;

    ON_HASH
    (
    
    GetDataHash(stk, MurMurHash3);
    GetStackHash(stk, MurMurHash3);
    
    )

    CHECK_STACK(stk);

    return ERROR_NOT_FOUND;
}

/*---------------------------------------------------------------------------------------------------------------------------------*/

/*              Functions for handling errors in stack                    */
ErrorCode StackDump(Stack *stk, const char *FileName, const char *FromFunc, const int LineCall)
{
    assert(stk && FileName && FromFunc && LineCall > 0);

    PRINT_STARS;

    ON_DEBUG(COLOR_PRINTF(GREEN,"\t\tStack Dump\n");)

    printf("%s:  in function: %s line: %d\n", FileName, FromFunc, LineCall);

    ON_DEBUG
    (
    
    printf("Stack adress: %#p\n", stk->StackElements);
    printf("Stack size: %lu\n", stk->size);
    printf("Stack capacity: %lu\n", stk->capacity);
    
    )

    ON_HASH
    (
    
    printf("Hash for data in stack: %llx\n", stk->DataHash);
    printf("Hash for stack: %llx\n", stk->StackHash);
    
    )

    ON_DEBUG(if (stk->StackElements) GetStackElements(*stk);)

    PRINT_STARS;
}


ErrorCode StackVerify(Stack *stk)
{   
    if (stk == nullptr)
        return STACK_NULL_ADRESS;

    if (stk->size < 0)
        return SIZE_BELOW_ZERO;

    if (&stk->StackElements[stk->size] == nullptr)
        return STACK_ELEMENT_NULL_ADDRESS;

    if (stk->size > stk->capacity)
        return SIZE_MORE_CAPACITY;

    if (stk->capacity == MAX_CAPACITY && stk->size >= MAX_CAPACITY)
        return STACK_OVERFLOW;

    ON_DEBUG
    (

    if (stk->capacity > MAX_CAPACITY)
        return CAPACITY_TOO_BIG;

    if (stk->capacity < 0)
        return CAPACITY_BELOW_ZERO;

    )

    ON_HASH
    (
    
   // if (IS_HASH_CHANGED(stk, MurMurHash3))
      //  return HASH_CHANGED;

    )

    return ERROR_NOT_FOUND;
}

void PrintError(Stack *stk, ErrorCode error, const char *FileName, const char *FuncName, const int LineCall)
{
    ON_DEBUG(StackDump(stk, FileName, FuncName, LineCall);)
    
    switch(error)
    {
        case ERROR_NOT_FOUND:
            ON_DEBUG(COLOR_PRINTF(GREEN, "ALL FINE!\n");)
            return;

        case STACK_NULL_ADRESS:
            COLOR_PRINTF(RED, "Stack in null address!\n");
            break;

        case STACK_ELEMENTS_ALLOCATION_ERROR:
            COLOR_PRINTF(RED, "Memory for elements in stack wasn`t allocated\n");
            break;

        case CAPACITY_BELOW_ZERO:
            COLOR_PRINTF(ORANGE, "Capacity is below zero! ");
            COLOR_PRINTF(ORANGE, "For this situtation program allocated standart capacity\n");
            break;

        case SIZE_BELOW_ZERO:
            COLOR_PRINTF(RED, "SIZE is below zero!");
            break;

        case POP_EMPTY_STACK:
            COLOR_PRINTF(ORANGE, "Pops element, but stack is empty(There is nothing to pop)\n");
            break;

        case REALLOC_ERROR:
            COLOR_PRINTF(RED, "Reallocation not work, because nothing to reallocate. ");
            COLOR_PRINTF(RED, "No free memory in heap");
            COLOR_PRINTF(RED, " or pointer in realloc not on allocated memory\n");
            break;

        case STACK_ELEMENT_NULL_ADDRESS:
            COLOR_PRINTF(RED, "Element in stack in null address!\n");
            break;

        case CAPACITY_TOO_BIG:
            COLOR_PRINTF(RED, "STACK CAPACITY TOO BIG!!\n");
            COLOR_PRINTF(RED, "CAN`T ALLOCATE SO MUCH MEMORY!!\n");
            break;
        
        case HASH_CHANGED:
            COLOR_PRINTF(RED, "HASH WAS CHANGED!\n");
            break;
    };

    COLOR_PRINTF(RED, "ABORTED\n");
    abort();
}

void GetStackElements(Stack *stk)
{   
    CHECK_STACK(stk);
    ON_CANARY(CHECK_CANARY(stk);)
    
    size_t elements = (stk->size < MAX_PRINT_ELEMENTS) ? stk->size : STDOUT_ELEMENTS;

    for (size_t i = 0; i < elements; i++)
        printf("stack[%d] = %lf\n", i, stk->StackElements[i]);   
}

/*-------------------------------------------------------------------------------------------------------------------------------*/