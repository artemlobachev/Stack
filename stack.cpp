#include <stdio.h>
#include <stdlib.h>
#include <assert.h> 
#include <math.h>

#include "stack.h"
#include "UseFulPrints.h"

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

ON_CANARY
(
    const CanaryType CANARY = 0xCAEFF64;

    static void SetDataCanary(Stack_t *StackElements, size_t capacity)
    {
        assert(StackElements);
        
        CanaryType BottomDataCanary = CANARY;
        CanaryType TopDataCanary = CANARY;         

        *(CanaryType *)((char *) StackElements - sizeof(CanaryType)) = TopDataCanary; 
        *(CanaryType *)(StackElements + capacity) = BottomDataCanary;
    }

)

ON_HASH
(
    static HashType IsDataHashChange(Stack *stk, HashType (*HashFunction) (const void *StackElements, size_t len, HashType seed))
{
    return (stk->DataHash != HashFunction((const void*)stk->StackElements, stk->capacity, 0));
}  

    static  HashType IsStackHashChange(Stack *stk, HashType (*HashFunction) (const void *Stack, size_t len, HashType seed))
{
    return (stk->StackHash != HashFunction((const void*)stk, 1, 0));
}

    static void GetDataHash(Stack *stk, HashType (*HashFunction) (const void *StackElements, size_t len, HashType seed))
{
    stk->DataHash = HashFunction((const void*)stk->StackElements, stk->capacity, 0);
}

    static void GetStackHash(Stack *stk, HashType (*HashFunction) (const void *Stack, size_t len, HashType seed))
{
    stk->StackHash = HashFunction((const void*)stk, 1, 0);
}

)


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
    CHECK_STACK(stk);

    Stack_t *ReallocSafePtr = (Stack_t *) realloc(stk->StackElements ON_CANARY(- 1), AllocatedMemory) ON_CANARY(+ 1);

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
    CHECK_STACK(stk);
 
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
        
        )

        CHECK_STACK(stk);
    }

    stk->StackElements[stk->size] = element;
    stk->size++;

    ON_HASH
    (

    GetDataHash(stk, MurMurHash3);
    GetStackHash(stk, MurMurHash3);

    )

    CHECK_STACK(stk);

    return ERROR_NOT_FOUND;
}

ErrorCode StackPop(Stack *stk)
{   
    CHECK_STACK(stk);

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
        
        )

        CHECK_STACK(stk);
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

    ON_CANARY
    (

    CanaryType *TopDataCanary = (CanaryType *) ((char *) stk->StackElements - sizeof(CanaryType));
    CanaryType *BottomDataCanary = (CanaryType *) (stk->StackElements + stk->capacity);

    printf("Bottom canary for stack stored at this address: %#p\n", stk->BOTTOM_CANARY);
    if (&stk->BOTTOM_CANARY) printf("Value at this address: %llu\n\n", stk->BOTTOM_CANARY);
    
    printf("Top canary for stack stored at this address: %#p\n", &stk->TOP_CANARY);  
    if (&stk->TOP_CANARY) printf("Value at this address: %llu\n\n", stk->TOP_CANARY);

    printf("Bottom canary for data in stack stored at this address: %#p\n", BottomDataCanary);
    if (BottomDataCanary) printf("Value at this address: %llu\n\n", *BottomDataCanary);

    printf("Top canary for data in stack stored at this address: %#p\n", TopDataCanary);
    if (TopDataCanary) printf("Value at thus address: %llu\n\n", *TopDataCanary);

    )

    ON_HASH
    (
    
    printf("Hash for data in stack: %llx\n", stk->DataHash);
    printf("Hash for stack: %llx\n", stk->StackHash);
    
    )

    PRINT_STARS;
}


ErrorCode StackVerify(Stack *stk)
{   
    ON_DEBUG
    (

    if (stk->capacity > MAX_CAPACITY)
        return CAPACITY_TOO_BIG;

    if (stk->capacity == 0)
        return CAPACITY_IS_ZERO;

    if ((int64_t)stk->capacity < 0)
        return CAPACITY_BELOW_ZERO;

    )

    if (stk->capacity > MAX_CAPACITY || stk->capacity == 0 || (int64_t)stk->capacity < 0)
    {
        stk->capacity = STDCAPACITY;
        return ERROR_NOT_FOUND;
    }
    
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

    ON_CANARY
    (

    if (&stk->TOP_CANARY == NULL || &stk->BOTTOM_CANARY == NULL)
        return CANARY_IN_NULL;
    
    if (stk->TOP_CANARY != CANARY || stk->BOTTOM_CANARY != CANARY)       
        return CHANGE_CANARY;

    if (&stk->TOP_CANARY == &stk->BOTTOM_CANARY)
        return SAME_CANARY_ADRESS;

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
        ON_DEBUG
        (

        case CAPACITY_TOO_BIG:
            COLOR_PRINTF(RED, "STACK CAPACITY TOO BIG!!\n");
            COLOR_PRINTF(RED, "CAN`T ALLOCATE SO MUCH MEMORY!!\n");
            break;

        case CAPACITY_IS_ZERO:
            COLOR_PRINTF(RED, "CAPACITY IS ZERO!!");
            break;

        case CAPACITY_BELOW_ZERO:
            COLOR_PRINTF(ORANGE, "Capacity is below zero! ");
            COLOR_PRINTF(ORANGE, "For this situtation program allocated standart capacity\n");
            break;

        )
        
        case ERROR_NOT_FOUND:
            ON_DEBUG(COLOR_PRINTF(GREEN, "ALL FINE!\n");)
            return;

        case STACK_NULL_ADRESS:
            COLOR_PRINTF(RED, "Stack in null address!\n");
            break;

        case STACK_ELEMENTS_ALLOCATION_ERROR:
            COLOR_PRINTF(RED, "Memory for elements in stack wasn`t allocated\n");
            break;

        case SIZE_BELOW_ZERO:
            COLOR_PRINTF(RED, "SIZE is below zero!");
            break;

        case POP_EMPTY_STACK:
            COLOR_PRINTF(ORANGE, "Pops element, but stack is empty(There is nothing to pop)\n");
            break;

        case STACK_ELEMENT_NULL_ADDRESS:
            COLOR_PRINTF(RED, "Element in stack in null address!\n");
            break;

        case REALLOC_ERROR:
            COLOR_PRINTF(RED, "Reallocation not work, because nothing to reallocate. ");
            COLOR_PRINTF(RED, "No free memory in heap");
            COLOR_PRINTF(RED, " or pointer in realloc not on allocated memory\n");
            break;
        
        ON_CANARY
        (

        case CANARY_IN_NULL:
            COLOR_PRINTF(RED, "CANARY POINTED IN NULL ADRESS\n");
            break;

        case CHANGE_CANARY:                          
            COLOR_PRINTF(RED, "TRY OVERFLOW BUFFER!\n");                                                                         
            break;

        case SAME_CANARY_ADRESS:                                                
            COLOR_PRINTF(RED, "CANARY HAVE SAME ADRESS ");                     
            COLOR_PRINTF(RED, "MAYBE CANARY-ADRESS WAS CHANGED\n");             
            break;

        )
        
        ON_HASH
        (
        
        case HASH_CHANGED:
            COLOR_PRINTF(RED, "HASH WAS CHANGED!\n");
            break;
        
        )
    };

    COLOR_PRINTF(RED, "ABORTED\n");
    abort();
}

void GetStackElements(Stack *stk)
{   
    CHECK_STACK(stk);
    
    size_t elements = (stk->size < MAX_PRINT_ELEMENTS) ? stk->size : STDOUT_ELEMENTS;

    for (size_t i = 0; i < elements; i++)
        printf("stack[%d] = %lf\n", i, stk->StackElements[i]);   
}

/*-------------------------------------------------------------------------------------------------------------------------------*/