#ifndef STACK_ERROR_H
#define STACK_ERROR_H

#include "stack.h"
#include "CanaryProtection.h"
#include "HashProtection.h"

#ifdef DEBUG
    #define STACK_ASSERT(STACK) StackAssert(STACK, StackVerify(STACK), __FILE__, __PRETTY_FUNCTION__, __LINE__)
    #define STACK_DUMP(STACK) StackDump(STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__)
    #define ON_DEBUG(...) __VA_ARGS__                                                    
#else  
    #define STACK_ASSERT(...)
    #define ON_DEBUG(...)
    #define STACK_DUMP(...)
#endif

enum Errors 
{
    ERROR_NOT_FOUND = 0,
    STACK_ELEMENTS_ALLOCATION_ERROR,
    STACK_NULL_ADRESS,
    CAPACITY_BELOW_ZERO,
    POP_EMPTY_STACK,
    REALLOC_ERROR,
    STACK_ELEMENT_NULL_ADDRESS,
    SIZE_MORE_CAPACITY,
    CAPACITY_TOO_BIG,
    STACK_OVERFLOW,
    SIZE_BELOW_ZERO
    
    ON_CANARY
    (
    ,CHANGE_CANARY,
    SAME_ADRESS,
    CANARY_IN_NULL
    
    )

    ON_HASH
    (

    ,HASH_CHANGED    
    
    )
};

static_assert(ERROR_NOT_FOUND == 0, "Something Wrong!");

#endif