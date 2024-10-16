#ifndef STACK_ERROR_H
#define STACK_ERROR_H

#include "stack.h"

#ifdef DEBUG
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
    POP_EMPTY_STACK,
    REALLOC_ERROR,
    STACK_ELEMENT_NULL_ADDRESS,
    SIZE_MORE_CAPACITY,
    STACK_OVERFLOW,
    SIZE_BELOW_ZERO
    

#ifdef DEBUG

    ,CAPACITY_IS_ZERO,
    CAPACITY_BELOW_ZERO,
    CAPACITY_TOO_BIG
#endif

#ifdef CANARY1
    
    ,CHANGE_CANARY,
    SAME_CANARY_ADRESS,
    CANARY_IN_NULL
    
#endif

#ifdef HASH

    ,HASH_CHANGED    
    
#endif

};

static_assert(ERROR_NOT_FOUND == 0, "Something Wrong!");

#endif